#pragma once

#include "CommonData.h"
#include "BaseEntity.h"
#include "StateManager.h"
#include "MessageManager.h"

namespace SSL
{
	class HouseWife : public BaseEntity
	{
	private:
		StateManager<HouseWife>*	m_stateManager;
		LOCATION					m_currentLocation;

	public:
		HouseWife(int id, LOCATION location, State<HouseWife>* state);

		virtual void Update();
		virtual void DealWithMessage(const MessageInfo& messageInfo) const;

		void GotoLocation(LOCATION location);

		bool IsCurrentLocation(LOCATION location) const;

		StateManager<HouseWife>* GetStateManager() { return m_stateManager; }
	};
} // namespace SSL

