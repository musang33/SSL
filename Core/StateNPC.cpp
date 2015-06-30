#include <iostream>
#include "CommonData.h"
#include "StateNPC.h"
#include "NPC.h"

namespace SSL
{	
	void NPCBaseState::OnEnter(NPC* npc)
	{
		std::cout << "[INFO][NPCBaseState][onEnter]" << std::endl;
		npc->ScriptEnter( GetID() );

		State<NPC>::OnEnter(npc);
	}

	void NPCBaseState::OnTick(NPC* npc)
	{
		std::cout << "[INFO][NPCBaseState][onTick]" << std::endl;
		npc->ScriptOnTick( GetID() );

		State<NPC>::OnTick(npc);
	}

	void NPCBaseState::OnExit(NPC* npc)
	{
		std::cout << "[INFO][NPCBaseState][onExit]" << std::endl;
		npc->ScriptExit( GetID() );

		State<NPC>::OnExit(npc);
	}

	void NPCBaseState::OnMessage( NPC* npc, const ST_MESSAGE_INFO& messageInfo )
	{
		std::cout << "[INFO][NPCBaseState][OnMessage]" << std::endl;
		npc->ScriptExit( GetID() );

		State<NPC>::OnMessage( npc, messageInfo );
	}

	void NPCAlive::onEnter(NPC* npc)
	{
		std::cout << "[INFO][Alive][onEnter]" << std::endl;		
	}

	void NPCAlive::onTick(NPC* npc)
	{
		std::cout << "[INFO][Alive][onTick]" << std::endl;
		npc->AddHPByRate(50);
		if ( npc->IsDead() )
		{
			npc->GetHFSM()->ChangeState(NPCDead::GetInstance());
		}		
	}

	void NPCAlive::onExit(NPC* npc)
	{
		std::cout << "[INFO][Alive][onExit]" << std::endl;
	}

	void NPCAlive::onMessage(NPC* npc, const ST_MESSAGE_INFO& messageInfo) const
	{
		std::cout << "[INFO][Alive][onMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
			case EN_MESSAGE_TYPE::MSG_SUBTRACTION_HP:
			{		
				INT32 subHP = reinterpret_cast<INT32>(messageInfo.extraInfo);

				npc->AddHPByRate(subHP);

				break;
			}
		}
	}

	// Dead
	
	void NPCDead::onEnter(NPC* npc)
	{
		std::cout << "[INFO][NPCDead][onEnter]" << std::endl;
		
	}

	void NPCDead::onTick(NPC* npc)
	{
		std::cout << "[INFO][NPCDead][onTick]" << std::endl;
		
		// npc 메모리 해제
	}

	void NPCDead::onExit(NPC* npc)
	{
		std::cout << "[INFO][NPCDead][onExit]" << std::endl;
	}

	// Peace

	void NPCPeace::onEnter(NPC* npc)
	{
		std::cout << "[INFO][NPCPeace][onEnter]" << std::endl;		
	}

	void NPCPeace::onTick(NPC* npc)
	{
		std::cout << "[INFO][NPCPeace][onTick]" << std::endl;		
	}

	void NPCPeace::onExit(NPC* npc)
	{
		std::cout << "[INFO][NPCPeace][onExit]" << std::endl;
	}

	void NPCPeace::onMessage(NPC* npc, const ST_MESSAGE_INFO& messageInfo) const
	{
		std::cout << "[INFO][NPCPeace][onMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
			case EN_MESSAGE_TYPE::MSG_FIND_ENEMY:
			{
				npc->GetHFSM()->ChangeState(NPCThink::GetInstance());

				break;
			}
		}
	}

	// Engage

	void NPCEngage::onEnter(NPC* npc)
	{
		std::cout << "[INFO][NPCEngage][onEnter]" << std::endl;		
	}

	void NPCEngage::onTick(NPC* npc)
	{
		std::cout << "[INFO][NPCEngage][onTick]" << std::endl;

		if ( false == npc->HasEnemyInAggroList() )
		{
			npc->GetHFSM()->ChangeState(NPCGuard::GetInstance());
		}	
	}

	void NPCEngage::onExit(NPC* npc)
	{
		std::cout << "[INFO][NPCEngage][onExit]" << std::endl;
	}	

	// Patrol

	void NPCPatrol::onEnter(NPC* npc)
	{
		std::cout << "[INFO][NPCPatrol][onEnter]" << std::endl;
	}

	void NPCPatrol::onTick(NPC* npc)
	{
		std::cout << "[INFO][NPCPatrol][onTick]" << std::endl;	
		if ( npc->HasFoundEnemy() )
		{
			npc->GetHFSM()->ChangeState(NPCThink::GetInstance());
			return;
		}
	}

	void NPCPatrol::onExit(NPC* npc)
	{
		std::cout << "[INFO][NPCPatrol][onExit]" << std::endl;
	}

	// Guard

	void NPCGuard::onEnter(NPC* npc)
	{
		std::cout << "[INFO][NPCGuard][onEnter]" << std::endl;
	}

	void NPCGuard::onTick(NPC* npc)
	{
		std::cout << "[INFO][NPCGuard][onTick]" << std::endl;		
		if ( npc->HasFoundEnemy() )
		{
			npc->GetHFSM()->ChangeState(NPCThink::GetInstance());
			return;
		}
	}

	void NPCGuard::onExit(NPC* npc)
	{
		std::cout << "[INFO][NPCGuard][onExit]" << std::endl;
	}

	// Think

	void NPCThink::onEnter(NPC* npc)
	{
		std::cout << "[INFO][NPCThink][onEnter]" << std::endl;
	}

	void NPCThink::onTick(NPC* npc)
	{
		std::cout << "[INFO][NPCThink][onTick]" << std::endl;

		if ( npc->GetCurrentHPRate() < 3000 )
		{
			npc->GetHFSM()->ChangeState(NPCFlee::GetInstance());
			return;
		}
		
		if ( npc->IsTargetInSkillDistance() )
		{
			npc->GetHFSM()->ChangeState(NPCAttack::GetInstance());
			return;
		}
	}

	void NPCThink::onExit(NPC* npc)
	{
		std::cout << "[INFO][NPCThink][onExit]" << std::endl;
	}

	// Attack

	void NPCAttack::onEnter(NPC* npc)
	{
		std::cout << "[INFO][NPCAttack][onEnter]" << std::endl;
	}

	void NPCAttack::onTick(NPC* npc)
	{
		std::cout << "[INFO][NPCAttack][onTick]" << std::endl;

		ST_MESSAGE_INFO messageInfo;
		messageInfo.senderID = EN_ENTITY_ID_RANGE::ID_RANGE_NPC;
		messageInfo.receiverID = EN_ENTITY_ID_RANGE::ID_RANGE_PLAYER;
		messageInfo.delayTime = 0;
		messageInfo.messageType = EN_MESSAGE_TYPE::MSG_SUBTRACTION_HP;
		messageInfo.extraInfo = reinterpret_cast<void*>(-5000);

		MessageManager::GetInstance()->Dispatch(messageInfo);

		npc->GetHFSM()->ChangeState(NPCThink::GetInstance());
		return;
	}

	void NPCAttack::onExit(NPC* npc)
	{
		std::cout << "[INFO][NPCAttack][onExit]" << std::endl;
	}

	// Flee

	void NPCFlee::onEnter(NPC* npc)
	{		
		std::cout << "[INFO][NPCFlee][onEnter]" << std::endl;
	}

	void NPCFlee::onTick(NPC* npc)
	{
		std::cout << "[INFO][NPCFlee][onTick]" << std::endl;

		if ( npc->GetCurrentHPRate() > 3000 )
		{
			npc->GetHFSM()->ChangeState(NPCThink::GetInstance());
			return;
		}
	}

	void NPCFlee::onExit(NPC* npc)
	{
		std::cout << "[INFO][NPCFlee][onExit]" << std::endl;
	}

}