

#include "GameEntity.h"
#include "State.h"

using namespace SSL;

int main()
{	
	Miner player(1, LOCATION::HOME, EnterMineAndDigForNugget::GetInstance());
		
	DWORD startTick = GetTickCount();

	EnterMineAndDigForNugget test1;
	EnterMineAndDigForNugget test2;

	//test1 = test2;

	while ( 1 )
	{
		if ( GetTickCount() - startTick < 1000 )
		{
			continue;
		}

		startTick = GetTickCount();

		player.Update();
	}

	return 0;
}

