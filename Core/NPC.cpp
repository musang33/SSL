#include "CommonData.h"
#include "NPC.h"
#include "StateNpc.h"
#include "LuaManager.h"
#include "WorldManager.h"
#include "EventQueue.h"

#include <random>
#include <chrono>
#include <functional>

namespace SSL
{
	NPC::NPC(int id, State<NPC>* state)
		:BaseEntity(id)
		, m_maxHP(5)
		, m_currentHP(5)		
		, m_npcAIIndex()
		, m_npcInstanceIndex()
	{
		if ( EN_AI_TYPE::AI_TYPE_HFSM == AIType )
		{
			m_hfsm = new HFSM<NPC>(this);
			initHFSMState();
			m_hfsm->SetCurrentState(state);
		}
		else if ( EN_AI_TYPE::AI_TYPE_BT == AIType )
		{
			m_behaviorTree = new BehaviorTreeManager<NPC>( this );
			if ( nullptr != m_behaviorTree )
			{
				m_behaviorTree->SetBehaviorTree();
			}
			else
			{
				exit( 0 );
			}
		}
		else
		{
			m_fsm = new FSM < NPC >( this );
			m_fsm->SetCurrentState( state );
		}	

		char tempAIIndex[128] = {};
		sprintf_s(tempAIIndex, sizeof(tempAIIndex), "AI_%d", id);
		m_npcAIIndex.assign(tempAIIndex);

		char tempInstanceId[128] = {};
		sprintf_s( tempInstanceId, sizeof( tempInstanceId ), "Instance_%d", id );
		m_npcInstanceIndex.assign(tempInstanceId);

		LuaManager::GetInstance()->setglobal( m_npcInstanceIndex.c_str(), this );

		SetCurLocation( rand() % 15, rand() % 15 );		

		std::cout << "[INFO][NPC][$$:create new NPC]" << std::endl;
	}

	void NPC::initHFSMState()
	{
		m_hfsm->RegisterState(nullptr, NPCRoot::GetInstance());

		m_hfsm->RegisterState(NPCRoot::GetInstance(), NPCAlive::GetInstance());
		m_hfsm->RegisterState(NPCRoot::GetInstance(), NPCDead::GetInstance());

		m_hfsm->RegisterState(NPCAlive::GetInstance(), NPCPeace::GetInstance());
		m_hfsm->RegisterState(NPCAlive::GetInstance(), NPCEngage::GetInstance());

		m_hfsm->RegisterState(NPCPeace::GetInstance(), NPCPatrol::GetInstance());
		m_hfsm->RegisterState(NPCPeace::GetInstance(), NPCGuard::GetInstance());

		m_hfsm->RegisterState(NPCEngage::GetInstance(), NPCThink::GetInstance());
		m_hfsm->RegisterState(NPCEngage::GetInstance(), NPCAttack::GetInstance());
		m_hfsm->RegisterState(NPCEngage::GetInstance(), NPCFlee::GetInstance());
	}

	void NPC::Update()
	{			
		UpdateQueue();

		if ( EN_AI_TYPE::AI_TYPE_HFSM == AIType )
		{
			m_hfsm->Update();
		}
		else if ( EN_AI_TYPE::AI_TYPE_BT == AIType )
		{
			m_behaviorTree->Update();
		}
		else
		{
			m_fsm->Update();
		}		
	}

	//entity
	//	worldjob->notify( EntityMangeer::Add, entity );
	//WorldJob::Notify()
	//{
	//	for ( auto& thread : threads )
	//	{
	//		thread->Notify();
	//		stD::ppl::aplqueue.push();
	//		pop()


	//void NPC::dispatch()
	//{
	//	for ( auto& event : m_eventQ )
	//	{
	//		if ( !ai->Dispatch( event ) )
	//		{
	//			process(event);
	//		}
	//	}
	//}


	void NPC::DealWithMessage(const ST_MESSAGE_INFO& messageInfo) const
	{
		if ( EN_AI_TYPE::AI_TYPE_HFSM == AIType )
		{
			m_hfsm->DealWithMessage(messageInfo);
		}
		else if ( EN_AI_TYPE::AI_TYPE_BT == AIType )
		{
			m_behaviorTree->DealWithMessage(messageInfo);
		}
		else
		{
			m_fsm->DealWithMessage(messageInfo);
		}		
	}
		
	EN_STATE_ID NPC::GetCurrentStateID()
	{
		if ( EN_AI_TYPE::AI_TYPE_HFSM == AIType )
		{
			return m_hfsm->GetCurrentState();
		}
		else if ( EN_AI_TYPE::AI_TYPE_BT == AIType )
		{
			return m_behaviorTree->GetCurrentState();
		}
		else
		{
			return m_fsm->GetCurrentState();
		}

		return EN_STATE_ID::STATE_NONE;
	}

	void NPC::SetCurrentStateIDInBehaviorTree( EN_STATE_ID stateId )
	{
		m_behaviorTree->SetCurrentStateID( stateId );
	}

	// ===================================================================================

	UINT32 NPC::GetCurrentHPRate()
	{
		if ( m_maxHP == 0 )
		{
			return 0;
		}

		return m_currentHP * 10000 / m_maxHP;
	}

	// ===================================================================================

	bool NPC::IsDead()
	{
		if ( 0 > m_currentHP )
		{
			return true;
		}

		return false;
	}

	// ===================================================================================
	
	void NPC::AddHPByRate(INT32 addHPRate)
	{
		std::cout << "[INFO][NPC][AddHP]" << std::endl;		

		INT32 addHP = m_maxHP * addHPRate / 10000;
		m_currentHP += addHP;

		if ( m_currentHP >= m_maxHP )
		{
			m_currentHP = m_maxHP;
		}
	}

	void NPC::AddHP( INT32 addHP )
	{
		m_currentHP += addHP;
		if ( m_currentHP > m_maxHP )
		{
			m_currentHP = m_maxHP;
		}
		else if ( m_currentHP < 0 )
		{
			m_currentHP = 0;
		}
	}

	void NPC::RandomMove()
	{
		int move_x = rand() % 3 - 1;
		int move_y = rand() % 3 - 1;

		ST_COORDINATE curLocation = GetCurLocation();
		curLocation.x += move_x;
		curLocation.y += move_y;
		SetCurLocation( curLocation.x, curLocation.y );
	}	

	bool NPC::HasFoundEnemy()
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		auto dice_rand = std::bind(std::uniform_int_distribution<int>(1, 100),
			std::mt19937_64(seed));

		if ( dice_rand() < 30 )
		{
			return false;
		}

		return true;
	}

	bool NPC::IsTargetInSkillDistance()
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		auto dice_rand = std::bind(std::uniform_int_distribution<int>(1, 100),
			std::mt19937_64(seed));

		if ( dice_rand() < 50 )
		{
			return false;
		}

		return true;
	}

	bool NPC::HasEnemyInAggroList()
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		auto dice_rand = std::bind(std::uniform_int_distribution<int>(1, 100),
			std::mt19937_64(seed));

		if ( dice_rand() < 50 )
		{
			return false;
		}

		return true;
	}

	void NPC::PrintLog(const char* msg)
	{
		if ( nullptr == msg || 0 == strlen(msg) )
		{
			return;
		}

		std::cout << msg << std::endl;
	}

	void NPC::ScriptEnter(UINT32 stateID)
	{
		LuaManager::GetInstance()->CallLuaFunction( stateID, m_npcAIIndex.c_str(), m_npcInstanceIndex.c_str(), "Enter" );
	}

	void NPC::ScriptOnTick( UINT32 stateID )
	{
		LuaManager::GetInstance()->CallLuaFunction( stateID, m_npcAIIndex.c_str(), m_npcInstanceIndex.c_str(), "Decide" );
	}

	void NPC::ScriptExit( UINT32 stateID )
	{
		LuaManager::GetInstance()->CallLuaFunction( stateID, m_npcAIIndex.c_str(), m_npcInstanceIndex.c_str(), "Exit" );
	}

	EN_BEHAVIOR_STATE NPC::FindEnemy()
	{
		SSL::ST_COORDINATE myLocation = GetCurLocation();

		const EntityManager::ENTITY_MAP& entityMap = EntityManager::GetInstance()->GetEntityMap();
		for ( auto &it : entityMap )
		{
			if ( it.first != BaseEntity::ID() && it.first >= EN_ENTITY_ID_RANGE::ID_RANGE_PLAYER )
			{
				SSL::ST_COORDINATE playerLocation = it.second->GetCurLocation();
				if ( WorldManager::GetInstance()->IsNear( myLocation.x, myLocation.y, playerLocation.x, playerLocation.y ) )
				{
					return BH_SUCCESS;
				}
			}
		}

		return BH_FAILURE;
	}

	bool NPC::IsEntityAt( int x, int y )
	{
		const EntityManager::ENTITY_MAP& entityMap = EntityManager::GetInstance()->GetEntityMap();
		for ( auto &it : entityMap )
		{
			if ( it.first != BaseEntity::ID() )
			{
				SSL::ST_COORDINATE location = it.second->GetCurLocation();
				if ( location.x == x && location.y == y )
				{
					return true;
				}
			}
		}

		return false;
	}

	EN_BEHAVIOR_STATE NPC::AttackEnemy()
	{ 
		SetCurrentStateIDInBehaviorTree( EN_STATE_ID::STATE_NPC_ATTACK );
		return BH_SUCCESS; 
	}

	EN_BEHAVIOR_STATE NPC::Patrol() 
	{ 
		SSL::ST_COORDINATE curLocation = GetCurLocation();
		int loopCount = 0;
		INT32 result = 1;
		SSL::ST_COORDINATE tempLocation;
		while ( result )
		{
			tempLocation = curLocation;
			tempLocation.x += rand() % 3 - 1;
			tempLocation.y += rand() % 3 - 1;
			result = WorldManager::GetInstance()->IsEntityAt( tempLocation.x, tempLocation.y );
			if ( loopCount++ > 10 )
			{
				return BH_SUCCESS;
			}
		}

		curLocation = tempLocation;
		
		if ( curLocation.x < 0 )
		{
			curLocation.x = 0;
		}
		else if ( curLocation.x > 14 )
		{
			curLocation.x = 14;
		}

		if ( curLocation.y < 0 )
		{
			curLocation.y = 0;
		}
		else if ( curLocation.y > 14 )
		{
			curLocation.y = 14;
		}
		
		SetCurLocation( curLocation.x, curLocation.y );

		SetCurrentStateIDInBehaviorTree( EN_STATE_ID::STATE_NPC_PATROL );

		return BH_SUCCESS; 
	}

	EN_BEHAVIOR_STATE NPC::CheckHP()
	{
		if ( GetCurrentHP() < 1 )
		{
			SetEntityState( EN_ENTITY_STATE::STATE_DEAD );
			return BH_INVALID;
		}

		return BH_SUCCESS;
	}

}