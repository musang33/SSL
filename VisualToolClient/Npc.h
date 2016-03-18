#pragma once

#include <..\FrameWork\Entity\Entity.h>

namespace SSL
{

	class Npc : public Entity
	{
	public:
		Npc( unsigned int id );
		~Npc( );

	private:
		bool				m_isDead;
		int					m_state;
		SSL::ST_COORDINATE	m_location;

	public:
		bool IsDead( )
		{
			return m_isDead;
		}

		int GetCurState( )
		{
			return m_state;
		}

		SSL::ST_COORDINATE GetLocation( )
		{
			return m_location;
		}

		void SetLocation( int x, int y )
		{
			m_location.x = x;
			m_location.y = y;
		}

	};

}