#include "ActionBT.h"
#include "ActionAI.h"
#include "ActionMove.h"
#include "ActionState.h"
#include "Entity.h"
#include "EntityManager.h"

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
		SSL::ST_COORDINATE curLocation = am->GetCurLocation();
		int loopCount = 0;
		INT32 result = 1;
		SSL::ST_COORDINATE tempLocation;
		while ( result )
		{
			tempLocation = curLocation;
			tempLocation.x += rand() % 3 - 1;
			tempLocation.y += rand() % 3 - 1;
			result = am->IsEntityAt( tempLocation.x, tempLocation.y );
			if ( loopCount++ > 10 )
			{
				return BH_SUCCESS;
			}
		}

		curLocation = tempLocation;

		if ( curLocation.x < 0 )
		{
			curLocation.x = 0;
		}
		else if ( curLocation.x > 14 )
		{
			curLocation.x = 14;
		}

		if ( curLocation.y < 0 )
		{
			curLocation.y = 0;
		}
		else if ( curLocation.y > 14 )
		{
			curLocation.y = 14;
		}

		am->SetCurLocation( curLocation.x, curLocation.y );

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