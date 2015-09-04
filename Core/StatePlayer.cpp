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

		// player �޸� ����
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
	}

	// Engage

	void PlayerEngage::onEnter(Entity* player)
	{
		std::cout << "[INFO][PlayerEngage][onEnter]" << std::endl;
	}

	void PlayerEngage::OnTick(Entity* player)
	{
		std::cout << "[INFO][PlayerEngage][OnTick]" << std::endl;		
	}

	void PlayerEngage::onExit(Entity* player)
	{
		std::cout << "[INFO][PlayerEngage][onExit]" << std::endl;
	}

	// Attack

	void PlayerAttack::onEnter(Entity* player)
	{
		std::cout << "[INFO][PlayerAttack][onEnter]" << std::endl;
	}

	void PlayerAttack::OnTick(Entity* player)
	{
		std::cout << "[INFO][PlayerAttack][OnTick]" << std::endl;		
	}

	void PlayerAttack::onExit(Entity* player)
	{
		std::cout << "[INFO][PlayerAttack][onExit]" << std::endl;
	}	
}