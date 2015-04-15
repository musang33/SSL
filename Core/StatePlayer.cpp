#include <iostream>
#include "CommonData.h"
#include "StatePlayer.h"
#include "Player.h"

namespace SSL
{
	void PlayerAlive::Enter(Player* player)
	{
		std::cout << "[INFO][Alive][Enter]" << std::endl;
	}

	void PlayerAlive::OnTick(Player* player)
	{
		std::cout << "[INFO][Alive][OnTick]" << std::endl;
		player->AddHPByRate(50);
		if ( player->IsDead() )
		{
			player->GetHFSM()->ChangeState(PlayerDead::GetInstance());
		}
	}

	void PlayerAlive::Exit(Player* player)
	{
		std::cout << "[INFO][Alive][Exit]" << std::endl;
	}

	void PlayerAlive::OnMessage(Player* player, const MessageInfo& messageInfo) const
	{
		std::cout << "[INFO][Alive][OnMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
		case MESSAGE_TYPE::MSG_SUBTRACTION_HP:
		{
			INT32 subHP = reinterpret_cast<INT32>(messageInfo.extraInfo);

			player->AddHPByRate(subHP);

			break;
		}
		}
	}

	// Dead

	void PlayerDead::Enter(Player* player)
	{
		std::cout << "[INFO][PlayerDead][Enter]" << std::endl;

	}

	void PlayerDead::OnTick(Player* player)
	{
		std::cout << "[INFO][PlayerDead][OnTick]" << std::endl;

		// player 메모리 해제
	}

	void PlayerDead::Exit(Player* player)
	{
		std::cout << "[INFO][PlayerDead][Exit]" << std::endl;
	}

	// Peace

	void PlayerPeace::Enter(Player* player)
	{
		std::cout << "[INFO][PlayerPeace][Enter]" << std::endl;
	}

	void PlayerPeace::OnTick(Player* player)
	{
		std::cout << "[INFO][PlayerPeace][OnTick]" << std::endl;
	}

	void PlayerPeace::Exit(Player* player)
	{
		std::cout << "[INFO][PlayerPeace][Exit]" << std::endl;
	}

	void PlayerPeace::OnMessage(Player* player, const MessageInfo& messageInfo) const
	{
		std::cout << "[INFO][PlayerPeace][OnMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
			case MESSAGE_TYPE::MSG_FIND_ENEMY:
			{
				player->GetHFSM()->ChangeState(PlayerThink::GetInstance());

				break;
			}
		}
	}

	// Engage

	void PlayerEngage::Enter(Player* player)
	{
		std::cout << "[INFO][PlayerEngage][Enter]" << std::endl;
	}

	void PlayerEngage::OnTick(Player* player)
	{
		std::cout << "[INFO][PlayerEngage][OnTick]" << std::endl;

		if ( false == player->HasEnemyInAggroList() )
		{
			player->GetHFSM()->ChangeState(PlayerGuard::GetInstance());
		}
	}

	void PlayerEngage::Exit(Player* player)
	{
		std::cout << "[INFO][PlayerEngage][Exit]" << std::endl;
	}

	// Patrol

	void PlayerPatrol::Enter(Player* player)
	{
		std::cout << "[INFO][PlayerPatrol][Enter]" << std::endl;
	}

	void PlayerPatrol::OnTick(Player* player)
	{
		std::cout << "[INFO][PlayerPatrol][OnTick]" << std::endl;
		if ( player->HasEnemyInAggroList() )
		{
			player->GetHFSM()->ChangeState( PlayerThink::GetInstance() );
			return;
		}
	}

	void PlayerPatrol::Exit(Player* player)
	{
		std::cout << "[INFO][PlayerPatrol][Exit]" << std::endl;
	}

	// Guard

	void PlayerGuard::Enter(Player* player)
	{
		std::cout << "[INFO][PlayerGuard][Enter]" << std::endl;
	}

	void PlayerGuard::OnTick(Player* player)
	{
		std::cout << "[INFO][PlayerGuard][OnTick]" << std::endl;
		if ( player->HasEnemyInAggroList() )
		{
			player->GetHFSM()->ChangeState( PlayerThink::GetInstance() );
			return;
		}
	}

	void PlayerGuard::Exit(Player* player)
	{
		std::cout << "[INFO][PlayerGuard][Exit]" << std::endl;
	}

	// Think

	void PlayerThink::Enter(Player* player)
	{
		std::cout << "[INFO][PlayerThink][Enter]" << std::endl;
	}

	void PlayerThink::OnTick(Player* player)
	{
		std::cout << "[INFO][PlayerThink][OnTick]" << std::endl;

		if ( player->GetCurrentHPRate() < 3000 )
		{
			player->GetHFSM()->ChangeState(PlayerFlee::GetInstance());
			return;
		}

		if ( player->IsTargetInSkillDistance() )
		{
			player->GetHFSM()->ChangeState(PlayerAttack::GetInstance());
			return;
		}
	}

	void PlayerThink::Exit(Player* player)
	{
		std::cout << "[INFO][PlayerThink][Exit]" << std::endl;
	}

	// Attack

	void PlayerAttack::Enter(Player* player)
	{
		std::cout << "[INFO][PlayerAttack][Enter]" << std::endl;
	}

	void PlayerAttack::OnTick(Player* player)
	{
		std::cout << "[INFO][PlayerAttack][OnTick]" << std::endl;

		MessageInfo messageInfo;
		messageInfo.senderID = EntityID::ID_PLAYER;
		messageInfo.receiverID = EntityID::ID_NPC;
		messageInfo.delayTime = 0;
		messageInfo.messageType = MESSAGE_TYPE::MSG_SUBTRACTION_HP;
		messageInfo.extraInfo = reinterpret_cast<void*>(-5000);

		MessageManager::GetInstance()->Dispatch(messageInfo);

		player->GetHFSM()->ChangeState(PlayerThink::GetInstance());
		return;
	}

	void PlayerAttack::Exit(Player* player)
	{
		std::cout << "[INFO][PlayerAttack][Exit]" << std::endl;
	}

	// Flee

	void PlayerFlee::Enter(Player* player)
	{
		std::cout << "[INFO][PlayerFlee][Enter]" << std::endl;
	}

	void PlayerFlee::OnTick(Player* player)
	{
		std::cout << "[INFO][PlayerFlee][OnTick]" << std::endl;

		if ( player->GetCurrentHPRate() > 3000 )
		{
			player->GetHFSM()->ChangeState(PlayerThink::GetInstance());
			return;
		}
	}

	void PlayerFlee::Exit(Player* player)
	{
		std::cout << "[INFO][PlayerFlee][Exit]" << std::endl;
	}

}