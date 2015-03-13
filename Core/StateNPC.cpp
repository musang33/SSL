#include <iostream>
#include "CommonData.h"
#include "StateNPC.h"
#include "NPC.h"

namespace SSL
{	
	void NPCAlive::Enter(NPC* npc)
	{
		std::cout << "[INFO][Alive][Enter]" << std::endl;		
	}

	void NPCAlive::OnTick(NPC* npc)
	{
		std::cout << "[INFO][Alive][OnTick]" << std::endl;
		npc->AddHPByRate(50);
		if ( npc->IsDead() )
		{
			npc->GetHFSM()->ChangeState(NPCDead::GetInstance());
		}		
	}

	void NPCAlive::Exit(NPC* npc)
	{
		std::cout << "[INFO][Alive][Exit]" << std::endl;
	}

	void NPCAlive::OnMessage(NPC* npc, const MessageInfo& messageInfo) const
	{
		std::cout << "[INFO][Alive][OnMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
			case MESSAGE_TYPE::MSG_SUBTRACTION_HP:
			{		
				INT32 subHP = reinterpret_cast<INT32>(messageInfo.extraInfo);

				npc->AddHPByRate(subHP);

				break;
			}
		}
	}

	// Dead
	
	void NPCDead::Enter(NPC* npc)
	{
		std::cout << "[INFO][NPCDead][Enter]" << std::endl;
		
	}

	void NPCDead::OnTick(NPC* npc)
	{
		std::cout << "[INFO][NPCDead][OnTick]" << std::endl;
		
		// npc 메모리 해제
	}

	void NPCDead::Exit(NPC* npc)
	{
		std::cout << "[INFO][NPCDead][Exit]" << std::endl;
	}

	// Peace

	void NPCPeace::Enter(NPC* npc)
	{
		std::cout << "[INFO][NPCPeace][Enter]" << std::endl;		
	}

	void NPCPeace::OnTick(NPC* npc)
	{
		std::cout << "[INFO][NPCPeace][OnTick]" << std::endl;		
	}

	void NPCPeace::Exit(NPC* npc)
	{
		std::cout << "[INFO][NPCPeace][Exit]" << std::endl;
	}

	void NPCPeace::OnMessage(NPC* npc, const MessageInfo& messageInfo) const
	{
		std::cout << "[INFO][NPCPeace][OnMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
			case MESSAGE_TYPE::MSG_FIND_ENEMY:
			{
				npc->GetHFSM()->ChangeState(NPCThink::GetInstance());

				break;
			}
		}
	}

	// Engage

	void NPCEngage::Enter(NPC* npc)
	{
		std::cout << "[INFO][NPCEngage][Enter]" << std::endl;		
	}

	void NPCEngage::OnTick(NPC* npc)
	{
		std::cout << "[INFO][NPCEngage][OnTick]" << std::endl;

		if ( false == npc->HasEnemyInAggroList() )
		{
			npc->GetHFSM()->ChangeState(NPCGuard::GetInstance());
		}	
	}

	void NPCEngage::Exit(NPC* npc)
	{
		std::cout << "[INFO][NPCEngage][Exit]" << std::endl;
	}	

	// Patrol

	void NPCPatrol::Enter(NPC* npc)
	{
		std::cout << "[INFO][NPCPatrol][Enter]" << std::endl;
	}

	void NPCPatrol::OnTick(NPC* npc)
	{
		std::cout << "[INFO][NPCPatrol][OnTick]" << std::endl;	
		if ( npc->HasFoundEnemy() )
		{
			npc->GetHFSM()->ChangeState(NPCThink::GetInstance());
			return;
		}
	}

	void NPCPatrol::Exit(NPC* npc)
	{
		std::cout << "[INFO][NPCPatrol][Exit]" << std::endl;
	}

	// Guard

	void NPCGuard::Enter(NPC* npc)
	{
		std::cout << "[INFO][NPCGuard][Enter]" << std::endl;
	}

	void NPCGuard::OnTick(NPC* npc)
	{
		std::cout << "[INFO][NPCGuard][OnTick]" << std::endl;		
		if ( npc->HasFoundEnemy() )
		{
			npc->GetHFSM()->ChangeState(NPCThink::GetInstance());
			return;
		}
	}

	void NPCGuard::Exit(NPC* npc)
	{
		std::cout << "[INFO][NPCGuard][Exit]" << std::endl;
	}

	// Think

	void NPCThink::Enter(NPC* npc)
	{
		std::cout << "[INFO][NPCThink][Enter]" << std::endl;
	}

	void NPCThink::OnTick(NPC* npc)
	{
		std::cout << "[INFO][NPCThink][OnTick]" << std::endl;

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

	void NPCThink::Exit(NPC* npc)
	{
		std::cout << "[INFO][NPCThink][Exit]" << std::endl;
	}

	// Attack

	void NPCAttack::Enter(NPC* npc)
	{
		std::cout << "[INFO][NPCAttack][Enter]" << std::endl;
	}

	void NPCAttack::OnTick(NPC* npc)
	{
		std::cout << "[INFO][NPCAttack][OnTick]" << std::endl;

		MessageInfo messageInfo;
		messageInfo.senderID = EntityID::ID_NPC;
		messageInfo.receiverID = EntityID::ID_PLAYER;
		messageInfo.delayTime = 0;
		messageInfo.messageType = MESSAGE_TYPE::MSG_SUBTRACTION_HP;
		messageInfo.extraInfo = reinterpret_cast<void*>(-5000);

		MessageManager::GetInstance()->Dispatch(messageInfo);

		npc->GetHFSM()->ChangeState(NPCThink::GetInstance());
		return;
	}

	void NPCAttack::Exit(NPC* npc)
	{
		std::cout << "[INFO][NPCAttack][Exit]" << std::endl;
	}

	// Flee

	void NPCFlee::Enter(NPC* npc)
	{		
		std::cout << "[INFO][NPCFlee][Enter]" << std::endl;
	}

	void NPCFlee::OnTick(NPC* npc)
	{
		std::cout << "[INFO][NPCFlee][OnTick]" << std::endl;

		if ( npc->GetCurrentHPRate() > 3000 )
		{
			npc->GetHFSM()->ChangeState(NPCThink::GetInstance());
			return;
		}
	}

	void NPCFlee::Exit(NPC* npc)
	{
		std::cout << "[INFO][NPCFlee][Exit]" << std::endl;
	}

}