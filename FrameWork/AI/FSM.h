#pragma once

#include "State.h"
#include <iostream>

namespace SSL
{
	class Entity;
	class FSM
	{
	private:
		Entity*		m_owner;
		State*		m_currentState;
		State*		m_GlobalState;
		State*		m_previousState;

	public:
		FSM( Entity* owner )
			: m_owner(owner)
			, m_currentState(nullptr)
			, m_GlobalState(nullptr)
			, m_previousState(nullptr)
		{}
		

		EN_STATE_ID GetCurrentState() { return static_cast<EN_STATE_ID>(m_currentState->GetID()); }
		void SetCurrentState(State* state) {	m_currentState = state; }
		void SetGlobalState(State* state) { m_GlobalState = state; }
		void SetPreviousState(State* state) { m_previousState = state; }

		void RevertPreviousState() { ChangeState(m_previousState); }

		void Update()
		{
			if ( m_GlobalState )
			{
				m_GlobalState->OnTick(m_owner);
			}

			if ( m_currentState )
			{
				m_currentState->OnTick(m_owner);
			}
		}

		void DealWithMessage(const ST_MESSAGE_INFO& messageInfo) const
		{
			if ( m_GlobalState )
			{
				m_GlobalState->OnMessage(m_owner, messageInfo);
			}

			if ( m_currentState )
			{
				m_currentState->OnMessage(m_owner, messageInfo);
			}
		}

		void ChangeState(State* newState)
		{
			if ( m_currentState == newState )
			{
				std::cout << "[ERROR][NPC][ChangeState][$$:newState is same with currentState]" << std::endl;
				return;
			}

			if ( nullptr == newState )
			{
				std::cout << "[ERROR][NPC][ChangeState][$$:newState is nullptr]" << std::endl;
				return;
			}

			if ( nullptr == m_currentState )
			{
				m_currentState = newState;
				m_currentState->OnEnter(m_owner);
				return;
			}

			m_previousState = m_currentState;

			m_currentState->OnExit(m_owner);
			m_currentState = newState;
			m_currentState->OnEnter(m_owner);
		}
	};
}

