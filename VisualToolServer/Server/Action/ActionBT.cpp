#include "stdafx.h"

#include <../FrameWork/Entity/Entity.h>

#include <Action/ActionBT.h>
#include <Action/ActionAI.h>
#include <Action/ActionMove.h>
#include <Action/ActionState.h>
#include <Manager/EntityManager.h>

namespace SSL
{

	ActionBT::ActionBT( Entity* entity )
		:Action( entity, EN_ACTION_TYPE::ACTION_BT )
	{
	}


	ActionBT::~ActionBT()
	{
	}

	EN_BEHAVIOR_STATE ActionBT::AttackEnemy()
	{
		ActionAI* aa = GetEntityAction( GetOwner() );
		aa->SetCurrentStateIDInBehaviorTree( EN_STATE_ID::STATE_NPC_ATTACK );
		return BH_SUCCESS;
	}

	EN_BEHAVIOR_STATE ActionBT::Patrol()
	{
		ActionMove* am = GetEntityAction( GetOwner() );
		am->RandomMove();

		ActionAI* aa = GetEntityAction( GetOwner() );
		aa->SetCurrentStateIDInBehaviorTree( EN_STATE_ID::STATE_NPC_PATROL );

		return BH_SUCCESS;
	}

	EN_BEHAVIOR_STATE ActionBT::CheckHP()
	{
		ActionState* as = GetEntityAction( GetOwner() );
		if ( as->IsDead() )
		{			
			ActionAI* aa = GetEntityAction( GetOwner() );
			aa->SetCurrentState( EN_STATE_ID::STATE_NPC_DEAD );
			return BH_FAILURE;
		}

		return BH_SUCCESS;
	}

	EN_BEHAVIOR_STATE ActionBT::FindEnemy()
	{
		ActionMove* am = GetEntityAction( GetOwner() );		

		const EntityManager::ENTITY_MAP& entityMap = EntityManager::GetInstance()->GetEntityMap();
		for ( auto &it : entityMap )
		{
			if ( it.first != GetOwner()->ID() && it.second->Type() == EN_ENTITY_TYPE::ENTITY_PLAYER_TYPE )
			{
				ActionMove* amTarget = GetEntityAction( it.second );
				SSL::ST_COORDINATE playerLocation = amTarget->GetCurLocation();
				if ( am->IsNear( playerLocation.x, playerLocation.y ) )
				{
					return BH_SUCCESS;
				}
			}
		}

		return BH_FAILURE;
	}
}