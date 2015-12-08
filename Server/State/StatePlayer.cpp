#include <iostream>
#include "CommonData.h"
#include "StatePlayer.h"
#include "Entity.h"
#include "ActionAI.h"
#include "ActionState.h"
#include "ActionPlayerFight.h"
#include "HFSM.h"
#include "MessageManager.h"
namespace SSL
{
	void PlayerAlive::onEnter(const Entity* player)
	{
		std::cout << "[INFO][Alive][onEnter]" << std::endl;
	}

	void PlayerAlive::OnTick(const Entity* player)
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

	void PlayerAlive::onExit(const Entity* player)
	{
		std::cout << "[INFO][Alive][onExit]" << std::endl;
	}

	void PlayerAlive::OnMessage(const Entity* player, const ST_MESSAGE_INFO& messageInfo) const
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

	void PlayerDead::onEnter(const Entity* player)
	{
		std::cout << "[INFO][PlayerDead][onEnter]" << std::endl;

	}

	void PlayerDead::OnTick(const Entity* player)
	{
		std::cout << "[INFO][PlayerDead][OnTick]" << std::endl;

		// player 메모리 해제
	}

	void PlayerDead::onExit(const Entity* player)
	{
		std::cout << "[INFO][PlayerDead][onExit]" << std::endl;
	}

	// Peace

	void PlayerPeace::onEnter(const Entity* player)
	{
		std::cout << "[INFO][PlayerPeace][onEnter]" << std::endl;
	}

	void PlayerPeace::OnTick(const Entity* player)
	{
		std::cout << "[INFO][PlayerPeace][OnTick]" << std::endl;
	}

	void PlayerPeace::onExit(const Entity* player)
	{
		std::cout << "[INFO][PlayerPeace][onExit]" << std::endl;
	}

	void PlayerPeace::OnMessage(const Entity* player, const ST_MESSAGE_INFO& messageInfo) const
	{
		std::cout << "[INFO][PlayerPeace][OnMessage]" << std::endl;
	}

	// Engage

	void PlayerEngage::onEnter(const Entity* player)
	{
		std::cout << "[INFO][PlayerEngage][onEnter]" << std::endl;
	}

	void PlayerEngage::OnTick(const Entity* player)
	{
		std::cout << "[INFO][PlayerEngage][OnTick]" << std::endl;		
	}

	void PlayerEngage::onExit(const Entity* player)
	{
		std::cout << "[INFO][PlayerEngage][onExit]" << std::endl;
	}

	// Attack

	void PlayerAttack::onEnter(const Entity* player)
	{
		std::cout << "[INFO][PlayerAttack][onEnter]" << std::endl;
	}

	void PlayerAttack::OnTick(const Entity* player)
	{
		std::cout << "[INFO][PlayerAttack][OnTick]" << std::endl;		
	}

	void PlayerAttack::onExit(const Entity* player)
	{
		std::cout << "[INFO][PlayerAttack][onExit]" << std::endl;
	}	
}