#pragma once

#include "CommonData.h"
#include "BaseEntity.h"
#include "MessageManager.h"
#include "FSM.h"
#include "HFSM.h"
#include "BehaviorTreeManager.h"

namespace SSL
{	
	class Player : public BaseEntity
	{
	private:
		FSM<Player>*					m_fsm;
		HFSM<Player>*					m_hfsm;
		BehaviorTreeManager<Player>*	m_behaviorTree;

		INT32							m_maxHP;
		INT32							m_currentHP;		

	private:
		void initHFSMState();

	public:
		Player(int id, State<Player>* state);

	public:
		// virtual function
		virtual void Update();
		virtual void DealWithMessage(const ST_MESSAGE_INFO& messageInfo) const;
		virtual EN_STATE_ID GetCurrentStateID();
		virtual void SetCurrentStateIDInBehaviorTree( EN_STATE_ID stateId );

		// Get function		
		UINT32 GetCurrentHPRate();

		// inline function
		inline FSM<Player>* GetStateManager() { return m_fsm; }
		inline HFSM<Player>* GetHFSM() { return m_hfsm; }
		inline INT32 GetCurrentHP() { return m_currentHP; }

		// Is function
		bool IsDead();
		bool IsTargetInSkillDistance();
		bool HasEnemyInAggroList();
					
		// control function
		void AddHPByRate( INT32 addHPRate );
		void AddHP( INT32 addHP );

		// Behavior Tree function
		EN_BEHAVIOR_STATE FindEnemy() { return BH_SUCCESS; }
		EN_BEHAVIOR_STATE AttackEnemy() { return BH_SUCCESS; }
		EN_BEHAVIOR_STATE Patrol() { return BH_SUCCESS; }
		EN_BEHAVIOR_STATE CheckHP();	
	};
} // namespace SSL

