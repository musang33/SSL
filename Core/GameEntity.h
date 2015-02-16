#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "CommonData.h"
#include "BaseEntity.h"
#include "StateManager.h"

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

		void Update();	
		
		void GotoLocation(LOCATION location);
		void MineGold();

		bool IsCurrentLocation(LOCATION location);		
		bool IsFullNugget();
		void ChangeNuggetToGoldAndSave();		

		StateManager<Miner>* GetStateManager() { return m_stateManager; }
	};
} // namespace SSL

#endif