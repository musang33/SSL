#pragma once

#include "CommonData.h"
#include "BaseEntity.h"
#include "MessageManager.h"
#include "FSM.h"
#include "HFSM.h"
#include "BehaviorTreeManager.h"
#include "EventQueue.h"

namespace SSL
{		
	class NPC :public BaseEntity
	{
	private:		
		FSM<NPC>*					m_fsm;
		HFSM<NPC>*					m_hfsm;
		BehaviorTreeManager<NPC>*	m_behaviorTree;
				
		INT32						m_maxHP;
		INT32						m_currentHP;		
		
		std::string					m_npcAIIndex;
		std::string					m_npcInstanceIndex;	

	public:		
		static BaseEntityPtr CreateNPC( INT32 id, State<NPC>* state )
		{
			return std::make_shared<NPC>( id, state );
		}

	private:
		void initHFSMState();

	public:
		NPC(int id, State<NPC>* state);		

	public:
		// virtual function
		virtual void Update();	
		virtual void DealWithMessage(const ST_MESSAGE_INFO& messageInfo) const;
		virtual EN_STATE_ID GetCurrentStateID();
		virtual void SetCurrentStateIDInBehaviorTree( EN_STATE_ID stateId );
		
		// Get function		
		UINT32 GetCurrentHPRate();
		
		// inline function
		inline const char* GetAIIndex() { return m_npcAIIndex.c_str(); }
		inline const char* GetInstanceIndex() { return m_npcInstanceIndex.c_str(); }
		inline FSM<NPC>* GetStateManager() { return m_fsm; }
		inline HFSM<NPC>* GetHFSM() { return m_hfsm; }
		inline INT32 GetCurrentHP() { return m_currentHP; }

		// Is function
		bool IsDead();
		bool IsTargetInSkillDistance();
		bool HasFoundEnemy();
		bool HasEnemyInAggroList();
		bool IsEntityAt( int x, int y );

		// print function
		void PrintLog( const char* msg );

		// control function
		void AddHPByRate(INT32 addHPRate);
		void AddHP( INT32 addHP );
		void RandomMove();			

		// Script function
		void ScriptEnter( UINT32 stateID );
		void ScriptOnTick( UINT32 stateID );
		void ScriptExit( UINT32 stateID );

		// Behavior Tree function
		EN_BEHAVIOR_STATE FindEnemy();
		EN_BEHAVIOR_STATE AttackEnemy();
		EN_BEHAVIOR_STATE Patrol();
		EN_BEHAVIOR_STATE CheckHP();	
	};
} // namespace SSL

