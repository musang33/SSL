#pragma once

#include "CommonData.h"
#include "BaseEntity.h"
#include "StateManager.h"
#include "MessageManager.h"
#include "HFSM.h"

namespace SSL
{	
	class NPC :public BaseEntity
	{
	private: 		
		StateManager<NPC>*		m_stateManager;
		HFSM<NPC>*				m_hfsm;

		LOCATION				m_currentLocation;
		
		INT32					m_maxHP;
		INT32					m_currentHP;
		UINT32					m_strikingPower;

	private:
		void initState();

	public:
		NPC(int id, LOCATION location, State<NPC>* state);		

		virtual void Update();	
		virtual void DealWithMessage(const MessageInfo& messageInfo) const;
		
		void GotoLocation(LOCATION location);
		bool IsCurrentLocation(LOCATION location) const;

		void AddHPByRate(INT32 addHPRate);
		UINT32 GetCurrentHPRate();

		bool IsDead();

		bool HasFoundEnemy();
		bool IsTargetInSkillDistance();
		bool HasEnemyInAggroList();

		StateManager<NPC>* GetStateManager() { return m_stateManager; }
		HFSM<NPC>* GetHFSM() { return m_hfsm; }
	};
} // namespace SSL

