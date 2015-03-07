#pragma once

#include <iostream>
#include <Windows.h>

namespace SSL
{
	const static int MAX_VALID_ENTITY_ID = 100000;
	const static int MAX_NUGGET_SIZE = 100;

	enum EntityID
	{
		ID_NONE,
		ID_MINER = 1,
		ID_HOUSE_WIFE,
	};

	enum LOCATION
	{
		NONE = 0,
		BANK = 10,
		COALMINE = 100,
		HOME = 200,
		HOME_KITCHEN = 201,
		HOME_LIVING_ROOM = 202,
		HOME_BATH_ROOM = 202,
	};

	enum MESSAGE_TYPE
	{
		MSG_NONE = 0,
		MSG_GET_MONEY,
		MSG_IM_HUMGRY,
		MSG_COOKING_COMPLETE,		
		MSG_EATING,
		MSG_EATING_COMPLETE,
		MSG_PASS_WATER_COMPLETE,
		
	};

	enum STATE_ID
	{
		STATE_ROOT = 1,

		STATE_HOUSEWIFE_COOKING		= 100,
		STATE_HOUSEWIFE_BATHROOM,
		STATE_HOUSEWIFE_LIVINGROOM,

		STATE_MINER_ENTER_MINE_AND_DIG_FOR_NUGGET = 200,
		STATE_MINER_GOTO_BAND_AND_DEPOSIT,
		STATE_MINER_HUNGRY,
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

