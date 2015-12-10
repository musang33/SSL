#include "stdafx.h"

#include <random>
#include <chrono>
#include <functional>

#include <Action/ActionMove.h>
#include <Action/ActionNpcFight.h>

#include <Manager/EntityManager.h>

namespace SSL
{

	ActionNpcFight::ActionNpcFight( Entity* entity )
		:Action( entity, EN_ACTION_TYPE::ACTION_NPC_FIGHT )
	{
	}


	ActionNpcFight::~ActionNpcFight()
	{
	}

	bool ActionNpcFight::HasFoundEnemy()
	{
		ActionMove* am = GetEntityAction( GetOwner( ) );

		const EntityManager::ENTITY_MAP& entityMap = EntityManager::GetInstance( )->GetEntityMap( );
		for( auto &it : entityMap )
		{
			if( it.first != GetOwner( )->ID( ) && it.second->Type( ) == EN_ENTITY_TYPE::ENTITY_PLAYER_TYPE )
			{
				ActionMove* amTarget = GetEntityAction( it.second );
				SSL::ST_COORDINATE playerLocation = amTarget->GetCurLocation( );
				if( am->IsNear( playerLocation.x, playerLocation.y ) )
				{
					return true;
				}
			}
		}

		return false;
	}

	bool ActionNpcFight::IsTargetInSkillDistance()
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		auto dice_rand = std::bind( std::uniform_int_distribution<int>( 1, 100 ),
									std::mt19937_64( seed ) );

		if ( dice_rand() < 50 )
		{
			return false;
		}

		return true;
	}

	bool ActionNpcFight::HasEnemyInAggroList()
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		auto dice_rand = std::bind( std::uniform_int_distribution<int>( 1, 100 ),
									std::mt19937_64( seed ) );

		if ( dice_rand() < 50 )
		{
			return false;
		}

		return true;
	}

}