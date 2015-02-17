#include <iostream>
#include "CommonData.h"
#include "StateMiner.h"
#include "Miner.h"

namespace SSL
{	
	void EnterMineAndDigForNugget::Enter(Miner* miner)
	{
		std::cout << "[INFO][EnterMineAndDigForNugget][Enter]" << std::endl;

		if (false == miner->IsCurrentLocation(LOCATION::COALMINE))
		{
			miner->GotoLocation(LOCATION::COALMINE);
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

	void EnterMineAndDigForNugget::OnMessage(Miner* miner, const MessageInfo& messageInfo) const
	{
		std::cout << "[INFO][EnterMineAndDigForNugget][OnMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
			case MESSAGE_TYPE::MSG_COOKING_COMPLETE:
			{			
				miner->GetStateManager()->ChangeState(Hungry::GetInstance());

				break;
			}
		}
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

		MessageInfo messageInfo;
		messageInfo.senderID = EntityID::ID_MINER;
		messageInfo.receiverID = EntityID::ID_HOUSE_WIFE;
		messageInfo.delayTime = 0;
		messageInfo.messageType = MESSAGE_TYPE::MSG_IM_HUMGRY;

		MessageManager::GetInstance()->Dispatch(messageInfo);

		miner->GetStateManager()->ChangeState(EnterMineAndDigForNugget::GetInstance());
	}

	void GotoBankAndDeposit::Exit(Miner* miner)
	{
		std::cout << "[INFO][GotoBankAndDeposit][Exit]" << std::endl;
	}

	void Hungry::Enter(Miner* miner)
	{
		std::cout << "[INFO][Hungry][Enter]" << std::endl;

		if ( false == miner->IsCurrentLocation(LOCATION::HOME_KITCHEN) )
		{
			miner->GotoLocation(LOCATION::HOME_KITCHEN);
		}

		MessageInfo messageInfo;
		messageInfo.senderID = EntityID::ID_MINER;
		messageInfo.receiverID = EntityID::ID_MINER;
		messageInfo.delayTime = 5000;
		messageInfo.messageType = MESSAGE_TYPE::MSG_EATING_COMPLETE;

		MessageManager::GetInstance()->Dispatch(messageInfo);
	}

	void Hungry::OnTick(Miner* miner)
	{
		std::cout << "[INFO][Hungry][OnTick]" << std::endl;		
	}

	void Hungry::Exit(Miner* miner)
	{
		std::cout << "[INFO][Hungry][Exit]" << std::endl;
	}

	void Hungry::OnMessage(Miner* miner, const MessageInfo& messageInfo) const
	{
		std::cout << "[INFO][Hungry][OnMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
		case MESSAGE_TYPE::MSG_EATING_COMPLETE:
		{
			miner->GetStateManager()->ChangeState(EnterMineAndDigForNugget::GetInstance());

			break;
		}
		}
	}
}