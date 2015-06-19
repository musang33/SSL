#pragma once

#include "CommonData.h"
#include "BaseEntity.h"
#include "FSM.h"
#include "MessageManager.h"
#include "HFSM.h"
#include "BehaviorTreeManager.h"

namespace SSL
{	
	class Player : public BaseEntity
	{
	private:
		FSM<Player>*			m_fsm;
		HFSM<Player>*			m_hfsm;
		BehaviorTreeManager<Player>*	m_behaviorTree;

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

		FSM<Player>* GetStateManager() { return m_fsm; }
		HFSM<Player>* GetHFSM() { return m_hfsm; }
	};
} // namespace SSL

