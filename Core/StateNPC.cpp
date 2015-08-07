#include <iostream>
#include "CommonData.h"
#include "StateNPC.h"
#include "Entity.h"
#include "Action.h"
#include "ActionAI.h"
#include "ActionNpcFight.h"
#include "ActionState.h"

#include "HFSM.h"
#include "MessageManager.h"

namespace SSL
{	
	void NPCBaseState::OnEnter(Entity* npc)
	{
		std::cout << "[INFO][NPCBaseState][onEnter]" << std::endl;
		ActionAI* aa = GetEntityAction(npc);

		aa->ScriptEnter( GetID() );

		State::OnEnter(npc);
	}

	void NPCBaseState::OnTick(Entity* npc)
	{
		std::cout << "[INFO][NPCBaseState][onTick]" << std::endl;
		ActionAI* aa = GetEntityAction( npc );
		aa->ScriptOnTick( GetID() );

		State::OnTick(npc);
	}

	void NPCBaseState::OnExit(Entity* npc)
	{
		std::cout << "[INFO][NPCBaseState][onExit]" << std::endl;
		ActionAI* aa = GetEntityAction( npc );
		aa->ScriptExit( GetID() );

		State::OnExit(npc);
	}

	void NPCBaseState::OnMessage( Entity* npc, const ST_MESSAGE_INFO& messageInfo )
	{
		std::cout << "[INFO][NPCBaseState][OnMessage]" << std::endl;
		ActionAI* aa = GetEntityAction( npc );
		aa->ScriptExit( GetID() );

		State::OnMessage( npc, messageInfo );
	}

	void NPCAlive::onEnter(Entity* npc)
	{
		std::cout << "[INFO][Alive][onEnter]" << std::endl;		
	}

	void NPCAlive::onTick(Entity* npc)
	{
		std::cout << "[INFO][Alive][onTick]" << std::endl;
		ActionState* as = GetEntityAction( npc );
		ActionAI* aa = GetEntityAction( npc );
		as->AddHPByRate( 50 );
		if ( as->IsDead() )
		{
			aa->GetHFSM()->ChangeState( NPCDead::GetInstance() );
		}		
	}

	void NPCAlive::onExit(Entity* npc)
	{
		std::cout << "[INFO][Alive][onExit]" << std::endl;
	}

	void NPCAlive::onMessage(Entity* npc, const ST_MESSAGE_INFO& messageInfo) const
	{
		std::cout << "[INFO][Alive][onMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
			case EN_MESSAGE_TYPE::MSG_SUBTRACTION_HP:
			{		
				INT32 subHP = reinterpret_cast<INT32>(messageInfo.extraInfo);
				ActionState* as = GetEntityAction( npc );
				as->AddHPByRate( subHP );

				break;
			}
		}
	}

	// Dead
	
	void NPCDead::onEnter(Entity* npc)
	{
		std::cout << "[INFO][NPCDead][onEnter]" << std::endl;
		
	}

	void NPCDead::onTick(Entity* npc)
	{
		std::cout << "[INFO][NPCDead][onTick]" << std::endl;
		
		// npc 메모리 해제
	}

	void NPCDead::onExit(Entity* npc)
	{
		std::cout << "[INFO][NPCDead][onExit]" << std::endl;
	}

	// Peace

	void NPCPeace::onEnter(Entity* npc)
	{
		std::cout << "[INFO][NPCPeace][onEnter]" << std::endl;		
	}

	void NPCPeace::onTick(Entity* npc)
	{
		std::cout << "[INFO][NPCPeace][onTick]" << std::endl;		
	}

	void NPCPeace::onExit(Entity* npc)
	{
		std::cout << "[INFO][NPCPeace][onExit]" << std::endl;
	}

	void NPCPeace::onMessage(Entity* npc, const ST_MESSAGE_INFO& messageInfo) const
	{
		std::cout << "[INFO][NPCPeace][onMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
			case EN_MESSAGE_TYPE::MSG_FIND_ENEMY:
			{
				ActionAI* aa = GetEntityAction( npc );
				aa->GetHFSM()->ChangeState( NPCThink::GetInstance() );

				break;
			}
		}
	}

	// Engage

	void NPCEngage::onEnter(Entity* npc)
	{
		std::cout << "[INFO][NPCEngage][onEnter]" << std::endl;		
	}

	void NPCEngage::onTick(Entity* npc)
	{
		std::cout << "[INFO][NPCEngage][onTick]" << std::endl;
		ActionNpcFight* anf = GetEntityAction( npc );
		if ( false == anf->HasEnemyInAggroList() )
		{
			ActionAI* aa = GetEntityAction( npc );
			aa->GetHFSM()->ChangeState( NPCGuard::GetInstance() );
		}	
	}

	void NPCEngage::onExit(Entity* npc)
	{
		std::cout << "[INFO][NPCEngage][onExit]" << std::endl;
	}	

	// Patrol

	void NPCPatrol::onEnter(Entity* npc)
	{
		std::cout << "[INFO][NPCPatrol][onEnter]" << std::endl;
	}

	void NPCPatrol::onTick(Entity* npc)
	{
		std::cout << "[INFO][NPCPatrol][onTick]" << std::endl;	
		ActionNpcFight* anf = GetEntityAction( npc );
		if ( anf->HasFoundEnemy() )
		{
			ActionAI* aa = GetEntityAction( npc );
			aa->GetHFSM()->ChangeState( NPCThink::GetInstance() );
			return;
		}
	}

	void NPCPatrol::onExit(Entity* npc)
	{
		std::cout << "[INFO][NPCPatrol][onExit]" << std::endl;
	}

	// Guard

	void NPCGuard::onEnter(Entity* npc)
	{
		std::cout << "[INFO][NPCGuard][onEnter]" << std::endl;
	}

	void NPCGuard::onTick(Entity* npc)
	{
		std::cout << "[INFO][NPCGuard][onTick]" << std::endl;		
		ActionNpcFight* anf = GetEntityAction( npc );
		if ( anf->HasFoundEnemy() )
		{
			ActionAI* aa = GetEntityAction( npc );
			aa->GetHFSM()->ChangeState( NPCThink::GetInstance() );
			return;
		}
	}

	void NPCGuard::onExit(Entity* npc)
	{
		std::cout << "[INFO][NPCGuard][onExit]" << std::endl;
	}

	// Think

	void NPCThink::onEnter(Entity* npc)
	{
		std::cout << "[INFO][NPCThink][onEnter]" << std::endl;
	}

	void NPCThink::onTick(Entity* npc)
	{
		std::cout << "[INFO][NPCThink][onTick]" << std::endl;
		ActionAI* aa = GetEntityAction( npc );
		ActionNpcFight* anf = GetEntityAction( npc );
		ActionState* as = GetEntityAction( npc );

		if ( as->GetCurrentHPRate() < 3000 )
		{
			aa->GetHFSM()->ChangeState( NPCFlee::GetInstance() );
			return;
		}
		
		if ( anf->IsTargetInSkillDistance() )
		{
			aa->GetHFSM()->ChangeState( NPCAttack::GetInstance() );
			return;
		}
	}

	void NPCThink::onExit(Entity* npc)
	{
		std::cout << "[INFO][NPCThink][onExit]" << std::endl;
	}

	// Attack

	void NPCAttack::onEnter(Entity* npc)
	{
		std::cout << "[INFO][NPCAttack][onEnter]" << std::endl;
	}

	void NPCAttack::onTick(Entity* npc)
	{
		std::cout << "[INFO][NPCAttack][onTick]" << std::endl;
		
		ST_MESSAGE_INFO messageInfo;
		messageInfo.senderID = npc->ID();
		messageInfo.receiverID = 0;
		messageInfo.delayTime = 0;
		messageInfo.messageType = EN_MESSAGE_TYPE::MSG_SUBTRACTION_HP;
		messageInfo.extraInfo = reinterpret_cast<void*>(-5000);

		MessageManager::GetInstance()->Dispatch(messageInfo);

		ActionAI* aa = GetEntityAction( npc );
		aa->GetHFSM()->ChangeState( NPCThink::GetInstance() );
		return;
	}

	void NPCAttack::onExit(Entity* npc)
	{
		std::cout << "[INFO][NPCAttack][onExit]" << std::endl;
	}

	// Flee

	void NPCFlee::onEnter(Entity* npc)
	{		
		std::cout << "[INFO][NPCFlee][onEnter]" << std::endl;
	}

	void NPCFlee::onTick(Entity* npc)
	{
		std::cout << "[INFO][NPCFlee][onTick]" << std::endl;
		ActionState* as = GetEntityAction( npc );
		if ( as->GetCurrentHPRate() > 3000 )
		{
			ActionAI* aa = GetEntityAction( npc );
			aa->GetHFSM()->ChangeState( NPCThink::GetInstance() );
			return;
		}
	}

	void NPCFlee::onExit(Entity* npc)
	{
		std::cout << "[INFO][NPCFlee][onExit]" << std::endl;
	}

}