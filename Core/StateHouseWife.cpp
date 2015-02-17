#include <iostream>
#include "CommonData.h"
#include "StateHouseWife.h"
#include "HouseWife.h"
#include "MessageManager.h"

namespace SSL
{	
	void Cooking::Enter(HouseWife* houseWife)
	{
		std::cout << "[INFO][Cooking][Enter]" << std::endl;

		if (false == houseWife->IsCurrentLocation(LOCATION::HOME_KITCHEN))
		{
			houseWife->GotoLocation(LOCATION::HOME_KITCHEN);
		}

		MessageInfo messageInfo;
		messageInfo.senderID = EntityID::ID_HOUSE_WIFE;
		messageInfo.receiverID = EntityID::ID_HOUSE_WIFE;
		messageInfo.delayTime = 5000;
		messageInfo.messageType = MESSAGE_TYPE::MSG_COOKING_COMPLETE;

		MessageManager::GetInstance()->Dispatch(messageInfo);
	}

	void Cooking::OnTick(HouseWife* houseWife)
	{
		std::cout << "[INFO][Cooking][OnTick]" << std::endl;			
	}

	void Cooking::Exit(HouseWife* houseWife)
	{
		std::cout << "[INFO][Cooking][Exit]" << std::endl;
	}

	void Cooking::OnMessage(HouseWife* houseWife, const MessageInfo& messageInfo) const
	{
		std::cout << "[INFO][Cooking][OnMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
			case MESSAGE_TYPE::MSG_COOKING_COMPLETE:
			{
				MessageInfo messageInfo;
				messageInfo.senderID = EntityID::ID_HOUSE_WIFE;
				messageInfo.receiverID = EntityID::ID_MINER;
				messageInfo.delayTime = 0;
				messageInfo.messageType = MESSAGE_TYPE::MSG_COOKING_COMPLETE;

				MessageManager::GetInstance()->Dispatch(messageInfo);

				houseWife->GetStateManager()->ChangeState(LivingRoom::GetInstance());

				break;
			}
		}		
	}
	
	void BathRoom::Enter(HouseWife* houseWife)
	{
		std::cout << "[INFO][BathRoom][Enter]" << std::endl;

		if ( false == houseWife->IsCurrentLocation(LOCATION::HOME_BATH_ROOM) )
		{
			houseWife->GotoLocation(LOCATION::HOME_BATH_ROOM);
		}

		MessageInfo messageInfo;
		messageInfo.senderID = EntityID::ID_HOUSE_WIFE;
		messageInfo.receiverID = EntityID::ID_HOUSE_WIFE;
		messageInfo.delayTime = 5000;
		messageInfo.messageType = MESSAGE_TYPE::MSG_PASS_WATER_COMPLETE;

		MessageManager::GetInstance()->Dispatch(messageInfo);
	}

	void BathRoom::OnTick(HouseWife* houseWife)
	{
		std::cout << "[INFO][BathRoom][OnTick]" << std::endl;		
	}

	void BathRoom::Exit(HouseWife* houseWife)
	{
		std::cout << "[INFO][BathRoom][Exit]" << std::endl;
	}

	void BathRoom::OnMessage(HouseWife* houseWife, const MessageInfo& messageInfo) const
	{
		std::cout << "[INFO][BathRoom][OnMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
			case MESSAGE_TYPE::MSG_PASS_WATER_COMPLETE:
			{				
				houseWife->GetStateManager()->ChangeState(LivingRoom::GetInstance());

				break;
			}
		}
	}

	void LivingRoom::Enter(HouseWife* houseWife)
	{
		std::cout << "[INFO][LivingRoom][Enter]" << std::endl;

		if ( false == houseWife->IsCurrentLocation(LOCATION::HOME_LIVING_ROOM) )
		{
			houseWife->GotoLocation(LOCATION::HOME_LIVING_ROOM);
		}	
	}

	void LivingRoom::OnTick(HouseWife* houseWife)
	{
		std::cout << "[INFO][LivingRoom][OnTick]" << std::endl;
	}

	void LivingRoom::Exit(HouseWife* houseWife)
	{
		std::cout << "[INFO][LivingRoom][Exit]" << std::endl;
	}	

	void LivingRoom::OnMessage(HouseWife* houseWife, const MessageInfo& messageInfo) const
	{
		std::cout << "[INFO][LivingRoom][OnMessage]" << std::endl;

		switch ( messageInfo.messageType )
		{
			case MESSAGE_TYPE::MSG_IM_HUMGRY:
			{
				houseWife->GetStateManager()->ChangeState(Cooking::GetInstance());

				break;
			}
		}
	}
}