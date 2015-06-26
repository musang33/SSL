#pragma once

#include <iostream>
#include <Windows.h>

namespace SSL
{
	const static int MAX_VALID_ENTITY_ID = 100000;
	const static int MAX_NUGGET_SIZE = 100;

	const static int AIType = 2;
	
	enum enAIType
	{
		AITYPE_FSM = 0,
		AITYPE_HFSM,
		AITYPE_BT,
	};

	enum EntityID
	{
		ID_NONE,
		ID_NPC = 1,
		ID_PLAYER = 1000,
	};

	enum LOCATION
	{
		NONE = 0,
		SAFTY_ZONE,
		BATTLEFIELD,
	};

	enum ENTITY_DIRECTION
	{
		// VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN °°À½
		DIRECTION_UP = 0x26,
		DIRECTION_DOWN = 0x28,
		DIRECTION_RIGHT = 0x27,
		DIRECTION_LEFT = 0x25
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

	enum BEHAVIOR_STATE
	{
		BH_INVALID,
		BH_SUCCESS,
		BH_FAILURE,
		BH_RUNNING,
		BH_SUSPENED
	};
		
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

	struct Location
	{
		int x;
		int y;
		Location() :x( 0 ), y( 0 ) {}
	};
}

