#include <iostream>
#include "CommonData.h"
#include "StatePlayer.h"
#include "Entity.h"
#include "Action.h"
#include "ActionAI.h"
#include "ActionState.h"
#include "ActionPlayerFight.h"
#include "HFSM.h"
#include "MessageManager.h"
namespace SSL
{
	void PlayerAlive::onEnter(Entity* player)
	{
		std::cout << "[INFO][Alive][onEnter]" << std::endl;
	}

	void PlayerAlive::OnTick(Entity* player)
	{
		std::cout << "[INFO][Alive][OnTick]" << std::endl;
		ActionState* as = GetEntityAction( player );
		as->AddHPByRate( 50 );
		if ( as->IsDead() )
		{
			ActionAI* aa = GetEntityAction( player );
			aa->GetHFSM()->ChangeState( PlayerDead::GetInstance() );
		}
	}

	void PlayerAlive::onExit(Entity* player)
	{
		std::cout << "[INFO][Alive][onExit]" << std::endl;
	}

	void PlayerAlive::OnMessage(Entity* player, const ST_MESSAGE_INFO& messageInfo) const
	{
		std::cout << "[INFO][Alive][OnMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
		case EN_MESSAGE_TYPE::MSG_SUBTRACTION_HP:
		{
			INT32 subHP = reinterpret_cast<INT32>(messageInfo.extraInfo);
			ActionState* as = GetEntityAction( player );
			as->AddHPByRate( subHP );

			break;
		}
		}
	}

	// Dead

	void PlayerDead::onEnter(Entity* player)
	{
		std::cout << "[INFO][PlayerDead][onEnter]" << std::endl;

	}

	void PlayerDead::OnTick(Entity* player)
	{
		std::cout << "[INFO][PlayerDead][OnTick]" << std::endl;

		// player 메모리 해제
	}

	void PlayerDead::onExit(Entity* player)
	{
		std::cout << "[INFO][PlayerDead][onExit]" << std::endl;
	}

	// Peace

	void PlayerPeace::onEnter(Entity* player)
	{
		std::cout << "[INFO][PlayerPeace][onEnter]" << std::endl;
	}

	void PlayerPeace::OnTick(Entity* player)
	{
		std::cout << "[INFO][PlayerPeace][OnTick]" << std::endl;
	}

	void PlayerPeace::onExit(Entity* player)
	{
		std::cout << "[INFO][PlayerPeace][onExit]" << std::endl;
	}

	void PlayerPeace::OnMessage(Entity* player, const ST_MESSAGE_INFO& messageInfo) const
	{
		std::cout << "[INFO][PlayerPeace][OnMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
			case EN_MESSAGE_TYPE::MSG_FIND_ENEMY:
			{
				ActionAI* aa = GetEntityAction( player );
				aa->GetHFSM()->ChangeState( PlayerThink::GetInstance() );

				break;
			}
		}
	}

	// Engage

	void PlayerEngage::onEnter(Entity* player)
	{
		std::cout << "[INFO][PlayerEngage][onEnter]" << std::endl;
	}

	void PlayerEngage::OnTick(Entity* player)
	{
		std::cout << "[INFO][PlayerEngage][OnTick]" << std::endl;
		ActionPlayerFight* apf = GetEntityAction( player );
		if ( false == apf->HasEnemyInAggroList() )
		{
			ActionAI* aa = GetEntityAction( player );
			aa->GetHFSM()->ChangeState( PlayerGuard::GetInstance() );
		}
	}

	void PlayerEngage::onExit(Entity* player)
	{
		std::cout << "[INFO][PlayerEngage][onExit]" << std::endl;
	}

	// Patrol

	void PlayerPatrol::onEnter(Entity* player)
	{
		std::cout << "[INFO][PlayerPatrol][onEnter]" << std::endl;
	}

	void PlayerPatrol::OnTick(Entity* player)
	{
		std::cout << "[INFO][PlayerPatrol][OnTick]" << std::endl;
		ActionPlayerFight* apf = GetEntityAction( player );
		if ( apf->HasEnemyInAggroList() )
		{
			ActionAI* aa = GetEntityAction( player );
			aa->GetHFSM()->ChangeState( PlayerThink::GetInstance() );
			return;
		}
	}

	void PlayerPatrol::onExit(Entity* player)
	{
		std::cout << "[INFO][PlayerPatrol][onExit]" << std::endl;
	}

	// Guard

	void PlayerGuard::onEnter(Entity* player)
	{
		std::cout << "[INFO][PlayerGuard][onEnter]" << std::endl;
	}

	void PlayerGuard::OnTick(Entity* player)
	{
		std::cout << "[INFO][PlayerGuard][OnTick]" << std::endl;
		ActionPlayerFight* apf = GetEntityAction( player );
		if ( apf->HasEnemyInAggroList() )
		{
			ActionAI* aa = GetEntityAction( player );
			aa->GetHFSM()->ChangeState( PlayerThink::GetInstance() );
			return;
		}
	}

	void PlayerGuard::onExit(Entity* player)
	{
		std::cout << "[INFO][PlayerGuard][onExit]" << std::endl;
	}

	// Think

	void PlayerThink::onEnter(Entity* player)
	{
		std::cout << "[INFO][PlayerThink][onEnter]" << std::endl;
	}

	void PlayerThink::OnTick(Entity* player)
	{
		std::cout << "[INFO][PlayerThink][OnTick]" << std::endl;
		ActionAI* aa = GetEntityAction( player );
		ActionState* as = GetEntityAction( player );
		if ( as->GetCurrentHPRate() < 3000 )
		{
			aa->GetHFSM()->ChangeState( PlayerFlee::GetInstance() );
			return;
		}

		ActionPlayerFight* apf = GetEntityAction( player );
		if ( apf->IsTargetInSkillDistance() )
		{
			aa->GetHFSM()->ChangeState( PlayerAttack::GetInstance() );
			return;
		}
	}

	void PlayerThink::onExit(Entity* player)
	{
		std::cout << "[INFO][PlayerThink][onExit]" << std::endl;
	}

	// Attack

	void PlayerAttack::onEnter(Entity* player)
	{
		std::cout << "[INFO][PlayerAttack][onEnter]" << std::endl;
	}

	void PlayerAttack::OnTick(Entity* player)
	{
		std::cout << "[INFO][PlayerAttack][OnTick]" << std::endl;

		ST_MESSAGE_INFO messageInfo;
		messageInfo.senderID = player->ID();;
		messageInfo.receiverID = 0;
		messageInfo.delayTime = 0;
		messageInfo.messageType = EN_MESSAGE_TYPE::MSG_SUBTRACTION_HP;
		messageInfo.extraInfo = reinterpret_cast<void*>(-5000);

		MessageManager::GetInstance()->Dispatch(messageInfo);

		ActionAI* aa = GetEntityAction( player );
		aa->GetHFSM()->ChangeState( PlayerThink::GetInstance() );
		return;
	}

	void PlayerAttack::onExit(Entity* player)
	{
		std::cout << "[INFO][PlayerAttack][onExit]" << std::endl;
	}

	// Flee

	void PlayerFlee::onEnter(Entity* player)
	{
		std::cout << "[INFO][PlayerFlee][onEnter]" << std::endl;
	}

	void PlayerFlee::OnTick(Entity* player)
	{
		std::cout << "[INFO][PlayerFlee][OnTick]" << std::endl;

		ActionState* as = GetEntityAction( player );
		if ( as->GetCurrentHPRate() > 3000 )
		{
			ActionAI* aa = GetEntityAction( player );
			aa->GetHFSM()->ChangeState( PlayerThink::GetInstance() );
			return;
		}
	}

	void PlayerFlee::onExit(Entity* player)
	{
		std::cout << "[INFO][PlayerFlee][onExit]" << std::endl;
	}

}