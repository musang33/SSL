#pragma once

#include <../FrameWork/Core/Action.h>

namespace SSL
{
	class ActionState : public Action
	{
	public:
		static const INT32 ID = EN_ACTION_TYPE::ACTION_STATE;

	public:
		ActionState( Entity* entity );
		~ActionState();

	private:
		INT32						m_maxHP;
		INT32						m_currentHP;


	public:
		// inline function
		inline INT32 GetCurrentHP() { return m_currentHP; }

		// Is function
		bool IsDead();

		// Get function		
		UINT32 GetCurrentHPRate();

		// control function
		void AddHPByRate( INT32 addHPRate );
		void AddHP( INT32 addHP );
	};
}