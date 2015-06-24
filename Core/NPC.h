#pragma once

#include "CommonData.h"
#include "BaseEntity.h"
#include "FSM.h"
#include "MessageManager.h"
#include "HFSM.h"
#include "BehaviorTreeManager.h"

namespace SSL
{	
	class NPC :public BaseEntity
	{
	private:
		FSM<NPC>*					m_fsm;
		HFSM<NPC>*					m_hfsm;
		BehaviorTreeManager<NPC>*	m_behaviorTree;

		LOCATION				m_currentLocation;
		
		INT32					m_maxHP;
		INT32					m_currentHP;
		UINT32					m_strikingPower;
		
		std::string				m_npcAIIndex;
		std::string				m_npcInstanceIndex;

		static UINT32			static_instance_id;

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

		void RandomMove();
		bool HasFoundEnemy();
		bool IsTargetInSkillDistance();
		bool HasEnemyInAggroList();

		void PrintLog(const char* msg);

		void ScriptEnter( UINT32 stateID );
		void ScriptOnTick( UINT32 stateID );
		void ScriptExit( UINT32 stateID );

		BEHAVIOR_STATE FindEnemy() { return BH_FAILURE; }
		BEHAVIOR_STATE AttackEnemy() { return BH_SUCCESS; }
		BEHAVIOR_STATE Move();

		const char* GetAIIndex() { return m_npcAIIndex.c_str(); }
		const char* GetInstanceIndex() { return m_npcInstanceIndex.c_str(); }

		FSM<NPC>* GetStateManager() { return m_fsm; }
		HFSM<NPC>* GetHFSM() { return m_hfsm; }

		
	};
} // namespace SSL

