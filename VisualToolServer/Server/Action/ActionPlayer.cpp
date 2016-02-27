#include "stdafx.h"

#include <random>
#include <chrono>
#include <functional>

#include <Action/ActionPlayer.h>

namespace SSL
{

	ActionPlayer::ActionPlayer( Entity* entity )
		:Action( entity, EN_ACTION_TYPE::ACTION_PLAYER )
	{
	}


	ActionPlayer::~ActionPlayer()
	{
	}
}