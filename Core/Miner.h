#pragma once

#include "CommonData.h"
#include "BaseEntity.h"
#include "StateManager.h"
#include "MessageManager.h"

namespace SSL
{	
	class Miner :public BaseEntity
	{
	private: 		
		StateManager<Miner>*	m_stateManager;
		LOCATION				m_currentLocation;
		UINT32					m_nuggetSize;
		UINT32					m_gold;


	public:
		Miner(int id, LOCATION location, State<Miner>* state);

		virtual void Update();	
		virtual void DealWithMessage(const MessageInfo& messageInfo) const;
		
		void GotoLocation(LOCATION location);
		void MineGold();

		bool IsCurrentLocation(LOCATION location) const;		
		bool IsFullNugget() const;
		void ChangeNuggetToGoldAndSave();		

		StateManager<Miner>* GetStateManager() { return m_stateManager; }
	};
} // namespace SSL

