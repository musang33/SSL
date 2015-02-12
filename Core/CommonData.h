#ifndef COMMONDATA_H
#define COMMONDATA_H

#include <iostream>
#include <Windows.h>

namespace SSL
{

	const static int MAX_VALID_ENTITY_ID = 100000;
	const static int MAX_NUGGET_SIZE = 100;

	enum LOCATION
	{
		NONE = 0,
		BANK = 10,
		GOLDMINE = 100,
		HOME = 200,
	};
}

#endif