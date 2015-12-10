#include "stdafx.h"

#include <random>
#include <chrono>
#include <functional>

#include <Action/ActionPlayerFight.h>

namespace SSL
{

	ActionPlayerFight::ActionPlayerFight( Entity* entity )
		:Action( entity, EN_ACTION_TYPE::ACTION_PLAYER_FIGHT )
	{
	}


	ActionPlayerFight::~ActionPlayerFight()
	{
	}

	bool ActionPlayerFight::HasFoundEnemy()
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		auto dice_rand = std::bind( std::uniform_int_distribution<int>( 1, 100 ),
									std::mt19937_64( seed ) );

		if ( dice_rand() < 30 )
		{
			return false;
		}

		return true;
	}

	bool ActionPlayerFight::IsTargetInSkillDistance()
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

	bool ActionPlayerFight::HasEnemyInAggroList()
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