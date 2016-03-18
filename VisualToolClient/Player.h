#pragma once

#include <..\FrameWork\Entity\Entity.h>

namespace SSL
{
	class Player : public Entity
	{
	public:
		Player( unsigned int id );
		~Player( );

	private:
		SSL::ST_COORDINATE			m_location;
		SSL::EN_ENTITY_DIRECTION	m_direction;

	public:
		SSL::ST_COORDINATE GetLocation( )
		{
			return m_location;
		}

		void SetLocation( int x, int y )
		{
			m_location.x = x;
			m_location.y = y;
		}

		SSL::EN_ENTITY_DIRECTION GetDirection( )
		{
			return m_direction;
		}

		void SetDirection( SSL::EN_ENTITY_DIRECTION direction )
		{
			m_direction = direction;			
		}
	};

}