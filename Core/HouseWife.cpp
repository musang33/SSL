#include "HouseWife.h"

namespace SSL {

	HouseWife::HouseWife(int id, LOCATION location, State<HouseWife>* state)
		:BaseEntity(id)
		, m_currentLocation(location)		
	{
		m_stateManager = new StateManager < HouseWife >(this);
		m_stateManager->SetCurrentState(state);

		std::cout << "[INFO][HouseWife][$$:create new HouseWife]" << std::endl;
	}

	bool HouseWife::IsCurrentLocation(LOCATION location) const
	{
		if ( m_currentLocation == location )
		{
			return true;
		}

		return false;
	}

	void HouseWife::Update()
	{
		m_stateManager->Update();
	}

	void HouseWife::DealWithMessage(const MessageInfo& messageInfo) const
	{
		m_stateManager->DealWithMessage(messageInfo);
	}

	void HouseWife::GotoLocation(LOCATION location)
	{
		m_currentLocation = location;
		std::cout << "[INFO][HouseWife][$$:Goto : " << location << "]" << std::endl;
	}
	

}