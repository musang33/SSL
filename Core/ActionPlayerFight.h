#pragma once

#include "Action.h"

namespace SSL
{

	class ActionPlayerFight : public Action
	{
	public:
		static const INT32 ID = EN_ACTION_TYPE::ACTION_PLAYER_FIGHT;

	public:
		ActionPlayerFight( Entity* entity );
		~ActionPlayerFight();

	public:
		bool IsTargetInSkillDistance();
		bool HasFoundEnemy();
		bool HasEnemyInAggroList();
	};

}