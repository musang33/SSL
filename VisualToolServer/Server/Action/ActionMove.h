#pragma once

#include <../FrameWork/Core/Action.h>

namespace SSL
{

	class ActionMove : public Action
	{
	public:
		static const INT32 ID = EN_ACTION_TYPE::ACTION_MOVE;

	public:
		ActionMove(Entity *entity);
		~ActionMove();

	private:
		ST_COORDINATE											m_currentLocation;
		EN_ENTITY_DIRECTION										m_direction;

	public:
		ST_COORDINATE GetCurLocation() const { return m_currentLocation; };
		void SetCurLocation( INT32 _x, INT32 _y )
		{
			m_currentLocation.x = _x;
			m_currentLocation.y = _y;
		}

		EN_ENTITY_DIRECTION GetDirection() { return m_direction; }
		void SetDirection( EN_ENTITY_DIRECTION direction )
		{
			m_direction = direction;
		}

	public:	
		INT32 IsEntityAt( int x, int y );
		bool IsNear( int x, int y );
		void RandomMove();
		bool Move( int x, int y );
	};

}