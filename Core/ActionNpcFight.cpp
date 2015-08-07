#include "ActionNpcFight.h"

#include <random>
#include <chrono>
#include <functional>

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
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		auto dice_rand = std::bind( std::uniform_int_distribution<int>( 1, 100 ),
									std::mt19937_64( seed ) );

		if ( dice_rand() < 30 )
		{
			return false;
		}

		return true;
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