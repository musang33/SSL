#pragma once

#include "Action.h"

namespace SSL
{
	class ActionNpcFight : public Action
	{
	public:
		static const INT32 ID = EN_ACTION_TYPE::ACTION_NPC_FIGHT;

	public:
		ActionNpcFight( Entity* entity );
		~ActionNpcFight();

	public:
		bool IsTargetInSkillDistance();
		bool HasFoundEnemy();
		bool HasEnemyInAggroList();
	};

}