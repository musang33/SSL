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

		bool HasFoundEnemy();
		bool IsTargetInSkillDistance();
		bool HasEnemyInAggroList();

		void PrintLog(const char* msg);

		void ScriptEnter( UINT32 stateID );
		void ScriptOnTick( UINT32 stateID );
		void ScriptExit( UINT32 stateID );

		const char* GetAIIndex() { return m_npcAIIndex.c_str(); }
		const char* GetInstanceIndex() { return m_npcInstanceIndex.c_str(); }

		StateManager<NPC>* GetStateManager() { return m_stateManager; }
		HFSM<NPC>* GetHFSM() { return m_hfsm; }
	};
} // namespace SSL

