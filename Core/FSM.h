#pragma once

#include "State.h"

namespace SSL
{
	template <typename T>
	class FSM
	{
	private:
		T*				m_owner;
		State<T>*		m_currentState;
		State<T>*		m_GlobalState;
		State<T>*		m_previousState;

	public:
		FSM(T* owner)
			: m_owner(owner)
			, m_currentState(nullptr)
			, m_GlobalState(nullptr)
			, m_previousState(nullptr)
		{}
		
		void SetCurrentState(State<T>* state) {	m_currentState = state; }
		void SetGlobalState(State<T>* state) { m_GlobalState = state; }
		void SetPreviousState(State<T>* state) { m_previousState = state; }

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

		void DealWithMessage(const MessageInfo& messageInfo) const
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

		void ChangeState(State<T>* newState)
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
				m_currentState->Enter(m_owner);
				return;
			}

			m_previousState = m_currentState;

			m_currentState->Exit(m_owner);
			m_currentState = newState;
			m_currentState->Enter(m_owner);
		}
	};
}

