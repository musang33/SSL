#include "ActionAI.h"
#include "CommonData.h"

#include "Entity.h"
#include "FSM.h"
#include "HFSM.h"
#include "BTM.h"
#include "LuaManager.h"

#include "StateNpc.h"

namespace SSL
{
	ActionAI::ActionAI( Entity* entity )
		:Action( entity, EN_ACTION_TYPE::ACTION_AI )
	{		
		char tempAIIndex[128] = {};
		sprintf_s( tempAIIndex, sizeof( tempAIIndex ), "AI_%d", entity->ID() );
		m_npcAIIndex.assign( tempAIIndex );

		char tempInstanceId[128] = {};
		sprintf_s( tempInstanceId, sizeof( tempInstanceId ), "Instance_%d", entity->ID() );
		m_npcInstanceIndex.assign( tempInstanceId );

		LuaManager::GetInstance()->setglobal( m_npcInstanceIndex.c_str(), this );

		SetInitialAI( nullptr );

	}

	ActionAI::~ActionAI()
	{
	}

	void ActionAI::SetInitialAI( State* state )
	{
		if ( EN_AI_TYPE::AI_TYPE_HFSM == AIType )
		{
			m_hfsm = new HFSM( GetOwner() );
			initHFSMState();
			m_hfsm->SetCurrentState( NPCPatrol::GetInstance( ) );
		}
		else if ( EN_AI_TYPE::AI_TYPE_BT == AIType )
		{
			m_behaviorTree = new BTM( GetOwner() );
			if ( nullptr != m_behaviorTree )
			{
				if ( GetOwner()->Type() == EN_ENTITY_TYPE::ENTITY_NPC_TYPE )
				{
					m_behaviorTree->SetBehaviorTreeNpc();
				}
				else if ( GetOwner()->Type() == EN_ENTITY_TYPE::ENTITY_PLAYER_TYPE )
				{
					m_behaviorTree->SetBehaviorTreePlayer();
				}				
			}
			else
			{
				exit( 0 );
			}
		}
		else
		{
			m_fsm = new FSM( GetOwner() );
			m_fsm->SetCurrentState( state );
		}
	}

	void ActionAI::initHFSMState()
	{
		m_hfsm->RegisterState( nullptr, NPCRoot::GetInstance() );

		m_hfsm->RegisterState( NPCRoot::GetInstance(), NPCAlive::GetInstance() );
		m_hfsm->RegisterState( NPCRoot::GetInstance(), NPCDead::GetInstance() );

		m_hfsm->RegisterState( NPCAlive::GetInstance(), NPCPeace::GetInstance() );
		m_hfsm->RegisterState( NPCAlive::GetInstance(), NPCEngage::GetInstance() );

		m_hfsm->RegisterState( NPCPeace::GetInstance(), NPCPatrol::GetInstance() );
		m_hfsm->RegisterState( NPCPeace::GetInstance(), NPCGuard::GetInstance() );

		m_hfsm->RegisterState( NPCEngage::GetInstance(), NPCThink::GetInstance() );
		m_hfsm->RegisterState( NPCEngage::GetInstance(), NPCAttack::GetInstance() );
		m_hfsm->RegisterState( NPCEngage::GetInstance(), NPCFlee::GetInstance() );
	}

	void ActionAI::Update()
	{
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

	void ActionAI::DealWithMessage( const ST_MESSAGE_INFO& messageInfo ) const
	{
		if ( EN_AI_TYPE::AI_TYPE_HFSM == AIType )
		{
			m_hfsm->DealWithMessage( messageInfo );
		}
		else if ( EN_AI_TYPE::AI_TYPE_BT == AIType )
		{
			m_behaviorTree->DealWithMessage( messageInfo );
		}
		else
		{
			m_fsm->DealWithMessage( messageInfo );
		}
	}

	EN_STATE_ID ActionAI::GetCurrentStateID()
	{
		if ( EN_AI_TYPE::AI_TYPE_HFSM == AIType )
		{
			return m_hfsm->GetCurrentState();
		}
		else if ( EN_AI_TYPE::AI_TYPE_BT == AIType )
		{
			return GetCurrentState();
		}
		else
		{
			return m_fsm->GetCurrentState();
		}

		return EN_STATE_ID::STATE_NONE;
	}

	void ActionAI::SetCurrentStateIDInBehaviorTree( EN_STATE_ID stateId )
	{
		SetCurrentState( stateId );
	}

	void ActionAI::ScriptEnter( UINT32 stateID )
	{
		//LuaManager::GetInstance()->CallLuaFunction( stateID, m_npcAIIndex.c_str(), m_npcInstanceIndex.c_str(), "Enter" );
	}

	void ActionAI::ScriptOnTick( UINT32 stateID )
	{
		//LuaManager::GetInstance()->CallLuaFunction( stateID, m_npcAIIndex.c_str(), m_npcInstanceIndex.c_str(), "Decide" );
	}

	void ActionAI::ScriptExit( UINT32 stateID )
	{
		//LuaManager::GetInstance()->CallLuaFunction( stateID, m_npcAIIndex.c_str(), m_npcInstanceIndex.c_str(), "Exit" );
	}
}