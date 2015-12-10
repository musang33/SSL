#pragma once
#include <../FrameWork/Core/Action.h>
#include <string>

namespace SSL
{
	class State;
	class FSM;
	class HFSM;
	class BTM;
	class Entity;

	class ActionAI : public Action
	{
	public:
		static const INT32 ID = EN_ACTION_TYPE::ACTION_AI;

	public:
		ActionAI( Entity* entity );		
		~ActionAI();

	private:
		void initHFSMState();

	private:
		FSM						*m_fsm;
		HFSM					*m_hfsm;
		BTM						*m_behaviorTree;

		std::string				m_npcAIIndex;
		std::string				m_npcInstanceIndex;
	
		EN_STATE_ID				m_currentState;

	public:
		void SetInitialAI( State* state );

		// virtual function
		virtual void Update();
		virtual void DealWithMessage( const ST_MESSAGE_INFO& messageInfo ) const;
		virtual EN_STATE_ID GetCurrentStateID();
		virtual void SetCurrentStateIDInBehaviorTree( EN_STATE_ID stateId );
		
		// inline function
		inline FSM* GetStateManager() { return m_fsm; }
		inline HFSM* GetHFSM() { return m_hfsm; }		
		inline const char* GetAIIndex() { return m_npcAIIndex.c_str(); }
		inline const char* GetInstanceIndex() { return m_npcInstanceIndex.c_str(); }

		EN_STATE_ID GetCurrentState() { return m_currentState; }
		void SetCurrentState( EN_STATE_ID stateId ) { m_currentState = stateId; }

		// Script function
		void ScriptEnter( UINT32 stateID );
		void ScriptOnTick( UINT32 stateID );
		void ScriptExit( UINT32 stateID );

	};

}