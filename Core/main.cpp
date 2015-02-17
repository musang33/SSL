#include "Miner.h"
#include "HouseWife.h"
#include "StateMiner.h"
#include "StateHouseWife.h"
#include "EntityManager.h"
#include "MessageManager.h"

using namespace SSL;

int main()
{	
	Miner* miner = new Miner(ID_MINER, LOCATION::HOME, EnterMineAndDigForNugget::GetInstance());
	HouseWife* houseWife = new HouseWife(ID_HOUSE_WIFE, LOCATION::HOME, LivingRoom::GetInstance());
		
	EntityManager::GetInstance()->RegisterEntity(miner);
	EntityManager::GetInstance()->RegisterEntity(houseWife);
	
	while ( 1 )
	{
		miner->Update();
		houseWife->Update();

		MessageManager::GetInstance()->DispatchDelayedMessage();

		Sleep(1000);
	}

	return 0;
}

