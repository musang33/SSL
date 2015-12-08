#pragma once

#include <windows.h>
#include <assert.h>

namespace SSL
{
	const static int MAX_VALID_ENTITY_ID = 100000;
	const static int MAX_NUGGET_SIZE = 100;

	const static int AIType = 1;
	
	enum EN_ENTITY_TYPE
	{
		ENTITY_NONE_TYPE = 0,
		ENTITY_NPC_TYPE,
		ENTITY_PLAYER_TYPE
	};

	enum EN_ENTITY_STATE
	{
		STATE_DEAD = 0,
		STATE_ALIVE	
	};
	
	enum EN_AI_TYPE
	{
		AI_TYPE_FSM = 0,
		AI_TYPE_HFSM,
		AI_TYPE_BT,
	};

	enum EN_ENTITY_DIRECTION
	{
		// VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN ����
		DIRECTION_UP = 0x26,
		DIRECTION_DOWN = 0x28,
		DIRECTION_RIGHT = 0x27,
		DIRECTION_LEFT = 0x25
	};

	enum EN_MESSAGE_TYPE
	{
		MSG_NONE = 0,
		
		MSG_SUBTRACTION_HP,
		
		MSG_FIND_ENEMY,			
	};

	enum EN_STATE_ID
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

	enum EN_BEHAVIOR_STATE
	{
		BH_INVALID,
		BH_SUCCESS,
		BH_FAILURE,
		BH_RUNNING,
		BH_SUSPENED,
		BH_ABORTED
	};

	enum EN_ACTION_TYPE
	{
		ACTION_NPC = 0,		
		ACTION_PLAYER,
		ACTION_AI,
		ACTION_BT,
		ACTION_MOVE,
		ACTION_STATE,
		ACTION_NPC_FIGHT,
		ACTION_PLAYER_FIGHT,
		ACTION_LUA_SCRIPT,

		MAX_ACTION_COUNT,
	};
		
	struct ST_MESSAGE_INFO
	{
		int senderID;
		int receiverID;
		EN_MESSAGE_TYPE messageType;
		double delayTime;
		void* extraInfo;

		ST_MESSAGE_INFO()
			: senderID(0)
			, receiverID(0)
			, messageType(EN_MESSAGE_TYPE::MSG_NONE)
			, delayTime(0)
			, extraInfo(nullptr)
		{}
	};

	struct ST_COORDINATE
	{
		int x;
		int y;
		ST_COORDINATE() :x( 0 ), y( 0 ) {}
	};	
}
