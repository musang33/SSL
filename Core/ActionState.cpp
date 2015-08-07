#include "ActionState.h"

namespace SSL
{
	ActionState::ActionState( Entity* entity )
		:Action( entity, EN_ACTION_TYPE::ACTION_STATE )
	{
		m_maxHP = 10;
		m_currentHP = 10;
	}


	ActionState::~ActionState()
	{
	}

	UINT32 ActionState::GetCurrentHPRate()
	{
		if ( m_maxHP == 0 )
		{
			return 0;
		}

		return m_currentHP * 10000 / m_maxHP;
	}

	// ===================================================================================

	bool ActionState::IsDead()
	{
		if ( 0 == m_currentHP )
		{
			return true;
		}

		return false;
	}

	// ===================================================================================

	void ActionState::AddHPByRate( INT32 addHPRate )
	{
		std::cout << "[INFO][NPC][AddHP]" << std::endl;

		INT32 addHP = m_maxHP * addHPRate / 10000;
		m_currentHP += addHP;

		if ( m_currentHP >= m_maxHP )
		{
			m_currentHP = m_maxHP;
		}
	}

	void ActionState::AddHP( INT32 addHP )
	{
		m_currentHP += addHP;
		if ( m_currentHP > m_maxHP )
		{
			m_currentHP = m_maxHP;
		}
		else if ( m_currentHP < 0 )
		{
			m_currentHP = 0;
		}
	}
}