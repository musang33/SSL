#include "CommonData.h"
#include "NPC.h"
#include "StateNpc.h"
#include "LuaManager.h"
#include "WorldManager.h"

#include <random>
#include <chrono>
#include <functional>

namespace SSL
{
	UINT32 NPC::static_instance_id = 0;

	NPC::NPC(int id, LOCATION location, State<NPC>* state)
		:BaseEntity(id)
		, m_currentLocation(location)		
		, m_maxHP(1000)
		, m_currentHP(1000)
		, m_strikingPower(0)
		, m_npcAIIndex()
		, m_npcInstanceIndex()
	{
		if ( enAIType::AITYPE_HFSM == AIType )
		{
			m_hfsm = new HFSM<NPC>(this);
			initState();
			m_hfsm->SetCurrentState(state);
		}
		else if ( enAIType::AITYPE_BT == AIType )
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
		sprintf_s(tempInstanceId, sizeof(tempInstanceId), "%d", static_instance_id++);
		m_npcInstanceIndex.assign(tempInstanceId);

		LuaManager::GetInstance()->setglobal(tempInstanceId, this);

		SetCurLocation( rand() % 15, rand() % 15 );

		std::cout << "[INFO][NPC][$$:create new NPC]" << std::endl;
	}

	void NPC::initState()
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
		if ( enAIType::AITYPE_HFSM == AIType )
		{
			m_hfsm->Update();
		}
		else if ( enAIType::AITYPE_BT == AIType )
		{
			m_behaviorTree->Update();
		}
		else
		{
			m_fsm->Update();
		}		
	}

	void NPC::DealWithMessage(const MessageInfo& messageInfo) const
	{
		if ( enAIType::AITYPE_HFSM == AIType )
		{
			m_hfsm->DealWithMessage(messageInfo);
		}
		else if ( enAIType::AITYPE_BT == AIType )
		{
			;
		}
		else
		{
			m_fsm->DealWithMessage(messageInfo);
		}		
	}
		
	STATE_ID NPC::GetCurrentStateID()
	{
		if ( enAIType::AITYPE_HFSM == AIType )
		{
			return m_hfsm->GetCurrentState();
		}
		else if ( enAIType::AITYPE_BT == AIType )
		{
			return m_behaviorTree->GetCurrentState();
		}
		else
		{
			return m_fsm->GetCurrentState();
		}

		return STATE_ID::STATE_NONE;
	}

	void NPC::SetCurrentStateIDInBehaviorTree( STATE_ID stateId )
	{
		m_behaviorTree->SetCurrentStateID( stateId );
	}
	
	void NPC::GotoLocation(LOCATION location)
	{
		m_currentLocation = location;
		std::cout << "[INFO][NPC][$$:Goto : " << location << "]" << std::endl;
	}

	bool NPC::IsCurrentLocation(LOCATION location) const
	{
		if ( m_currentLocation == location )
		{
			return true;
		}

		return false;
	}

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

	UINT32 NPC::GetCurrentHPRate()
	{
		if ( m_maxHP == 0 )
		{
			return 0;
		}
			 
		return m_currentHP * 10000 / m_maxHP;
	}

	bool NPC::IsDead()
	{
		if ( 0 > m_currentHP )
		{
			return true;
		}

		return false;
	}

	void NPC::RandomMove()
	{
		int move_x = rand() % 3 - 1;
		int move_y = rand() % 3 - 1;

		Location curLocation = GetCurLocation();
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

	BEHAVIOR_STATE NPC::FindEnemy()
	{
		SSL::Location myLocation = GetCurLocation();

		const WorldManager::ENTITY_MAP& entityMap = WorldManager::GetInstance()->GetWorldEntityMap();
		for ( auto &it : entityMap )
		{
			if ( it.first != BaseEntity::ID() && it.first >= EntityID::ID_PLAYER )
			{
				SSL::Location playerLocation = it.second->GetCurLocation();
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
		const WorldManager::ENTITY_MAP& entityMap = WorldManager::GetInstance()->GetWorldEntityMap();
		for ( auto &it : entityMap )
		{
			if ( it.first != BaseEntity::ID() )
			{
				SSL::Location location = it.second->GetCurLocation();
				if ( location.x == x && location.y == y )
				{
					return true;
				}
			}
		}

		return false;
	}

	BEHAVIOR_STATE NPC::AttackEnemy()
	{ 
		SetCurrentStateIDInBehaviorTree( STATE_ID::STATE_NPC_ATTACK );
		return BH_SUCCESS; 
	}

	BEHAVIOR_STATE NPC::Patrol() 
	{ 
		SSL::Location curLocation = GetCurLocation();
		int loopCount = 0;
		bool result = true;
		SSL::Location tempLocation;
		while ( result )
		{
			tempLocation = curLocation;
			tempLocation.x += rand() % 3 - 1;
			tempLocation.y += rand() % 3 - 1;
			result = IsEntityAt( tempLocation.x, tempLocation.y );
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

		SetCurrentStateIDInBehaviorTree( STATE_ID::STATE_NPC_PATROL );

		return BH_SUCCESS; 
	}
}