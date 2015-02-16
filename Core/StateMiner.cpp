#include <iostream>
#include "CommonData.h"
#include "StateMiner.h"
#include "GameEntity.h"

namespace SSL
{	
	void EnterMineAndDigForNugget::Enter(Miner* miner)
	{
		std::cout << "[INFO][EnterMineAndDigForNugget][Enter]" << std::endl;

		if (false == miner->IsCurrentLocation(LOCATION::GOLDMINE))
		{
			miner->GotoLocation(LOCATION::GOLDMINE);
		}
	}

	void EnterMineAndDigForNugget::OnTick(Miner* miner)
	{
		std::cout << "[INFO][EnterMineAndDigForNugget][OnTick]" << std::endl;
		miner->MineGold();
		if (miner->IsFullNugget())
		{
			miner->GetStateManager()->ChangeState(GotoBankAndDeposit::GetInstance());
		}		
	}

	void EnterMineAndDigForNugget::Exit(Miner* miner)
	{
		std::cout << "[INFO][EnterMineAndDigForNugget][Exit]" << std::endl;
	}
	
	void GotoBankAndDeposit::Enter(Miner* miner)
	{
		std::cout << "[INFO][GotoBankAndDeposit][Enter]" << std::endl;

		if (false == miner->IsCurrentLocation(LOCATION::BANK))
		{
			miner->GotoLocation(LOCATION::BANK);
		}
	}

	void GotoBankAndDeposit::OnTick(Miner* miner)
	{
		std::cout << "[INFO][GotoBankAndDeposit][OnTick]" << std::endl;
		miner->ChangeNuggetToGoldAndSave();
		miner->GetStateManager()->ChangeState(EnterMineAndDigForNugget::GetInstance());
	}

	void GotoBankAndDeposit::Exit(Miner* miner)
	{
		std::cout << "[INFO][GotoBankAndDeposit][Exit]" << std::endl;
	}
}