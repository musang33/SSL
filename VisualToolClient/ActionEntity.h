#pragma once

namespace SSL
{
	class ActionEntity 
	{
	public:
		ActionEntity( );
		~ActionEntity( );

	public:
		void SetSocketIndex( UINT32 index )
		{
			m_socketIndex = index;
		}
		UINT32 GetSocketIndex( )
		{
			return m_socketIndex;
		}

		void SetEntityIndex( INT32 index )
		{
			m_entityIndex = index;
		}
		UINT32 GetEntityIndex( )
		{
			return m_entityIndex;
		}

		void SetLocation( INT32 x, INT32 y )
		{
			m_coordinate.x = x;
			m_coordinate.y = y;
		}
		ST_COORDINATE GetLocation( )
		{
			return m_coordinate;
		}

	private:
		UINT32				m_socketIndex;
		INT32				m_entityIndex;
		ST_COORDINATE		m_coordinate;
		
	};
}