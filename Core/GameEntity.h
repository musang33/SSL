#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "CommonData.h"
#include "BaseEntity.h"
#include "State.h"

namespace SSL
{	
	class Miner :public BaseEntity
	{
	private: 
		State*		m_currentState;
		LOCATION	m_currentLocation;
		UINT32		m_nuggetSize;
		UINT32		m_gold;


	public:
		Miner(int id, LOCATION location, State* state);

		void Update();
		void ChangeState(State* newState);

		void GotoLocation(LOCATION location);
		void MineGold();

		bool IsCurrentLocation(LOCATION location);		
		bool IsFullNugget();
		void ChangeNuggetToGoldAndSave();		
	};
} // namespace SSL

#endif