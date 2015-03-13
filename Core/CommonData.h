#pragma once

#include <iostream>
#include <Windows.h>

namespace SSL
{
	const static int MAX_VALID_ENTITY_ID = 100000;
	const static int MAX_NUGGET_SIZE = 100;

	const static int USE_HSM = 1;

	enum EntityID
	{
		ID_NONE,
		ID_NPC = 1,
		ID_PLAYER,
	};

	enum LOCATION
	{
		NONE = 0,
		SAFTY_ZONE,
		BATTLEFIELD,
	};

	enum MESSAGE_TYPE
	{
		MSG_NONE = 0,
		
		MSG_SUBTRACTION_HP,
		
		MSG_FIND_ENEMY,			
	};

	enum STATE_ID
	{
		STATE_NONE = 0,
		STATE_NPC_ROOT = 1,
		STATE_PLAYER_ROOT = 2,

		// NPC		

		STATE_NPC_ALIVE		= 100,
		STATE_NPC_DEAD,

		STATE_NPC_PEACE,
		STATE_NPC_ENGAGE,

		STATE_NPC_PATROL	= 200,
		STATE_NPC_GUARD,

		STATE_NPC_THINK,
		STATE_NPC_ATTACK,
		STATE_NPC_FLEE,

		///  Player
		STATE_PLAYER_ALIVE = 10000,
		STATE_PLAYER_DEAD,

		STATE_PLAYER_PEACE,
		STATE_PLAYER_ENGAGE,

		STATE_PLAYER_PATROL = 10100,
		STATE_PLAYER_GUARD,

		STATE_PLAYER_THINK,
		STATE_PLAYER_ATTACK,
		STATE_PLAYER_FLEE,
	};

	class BaseEntity;
	struct MessageInfo
	{
		INT32 senderID;
		INT32 receiverID;
		MESSAGE_TYPE messageType;
		double delayTime;
		void* extraInfo;

		MessageInfo()
			: senderID(0)
			, receiverID(0)
			, messageType(MESSAGE_TYPE::MSG_NONE)
			, delayTime(0)
			, extraInfo(nullptr)
		{}
	};
}

