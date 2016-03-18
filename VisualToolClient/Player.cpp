#include "stdafx.h"
#include "Player.h"

namespace SSL
{

	Player::Player( unsigned int id )
		: Entity( id, 0, SSL::EN_ENTITY_TYPE::ENTITY_PLAYER_TYPE )
	{
	}


	Player::~Player( )
	{
	}

}