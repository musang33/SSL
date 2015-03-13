#pragma once

#include "CommonData.h"
#include "BaseEntity.h"
#include "StateManager.h"
#include "MessageManager.h"
#include "HFSM.h"

namespace SSL
{	
	class Player : public BaseEntity
	{
	private:
		StateManager<Player>*	m_stateManager;
		HFSM<Player>*			m_hfsm;

		LOCATION				m_currentLocation;

		INT32					m_maxHP;
		INT32					m_currentHP;
		UINT32					m_strikingPower;

	private:
		void initState();

	public:
		Player(int id, LOCATION location, State<Player>* state);

		virtual void Update();
		virtual void DealWithMessage(const MessageInfo& messageInfo) const;

		void GotoLocation(LOCATION location);
		bool IsCurrentLocation(LOCATION location) const;

		void AddHPByRate(INT32 addHPRate);
		UINT32 GetCurrentHPRate();

		bool IsDead();

		bool IsTargetInSkillDistance();
		bool HasEnemyInAggroList();

		StateManager<Player>* GetStateManager() { return m_stateManager; }
		HFSM<Player>* GetHFSM() { return m_hfsm; }
	};
} // namespace SSL

