#pragma once
#include "CommonData.h"

#include "Action.h"

namespace SSL
{
	class ActionBT : public Action
	{
	public:
		static const INT32 ID = EN_ACTION_TYPE::ACTION_BT;

	public:
		ActionBT(Entity* entity);
		~ActionBT();

	public:
		EN_BEHAVIOR_STATE FindEnemy();
		EN_BEHAVIOR_STATE AttackEnemy();
		EN_BEHAVIOR_STATE Patrol();
		EN_BEHAVIOR_STATE CheckHP();
	};
}

