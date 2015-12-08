#include "ActionMove.h"
#include "EntityManager.h"

namespace SSL
{

	ActionMove::ActionMove( Entity *entity )
		:Action( entity, EN_ACTION_TYPE::ACTION_MOVE )
	{
		m_currentLocation.x = 0;
		m_currentLocation.y = 0;
	}


	ActionMove::~ActionMove()
	{
	}

	INT32 ActionMove::IsEntityAt( int x, int y )
	{
		const EntityManager::ENTITY_MAP& entityMap = EntityManager::GetInstance()->GetEntityMap();
		for ( auto &it : entityMap )
		{
			if ( it.first != GetOwner()->ID() )
			{
				ActionMove *am = GetEntityAction(it.second);
				SSL::ST_COORDINATE location = am->GetCurLocation();
				if ( location.x == x && location.y == y )
				{
					return it.first;
				}
			}
		}

		return 0;
	}

	bool ActionMove::IsNear( int oppositeX, int oppositeY )
	{
		if ( oppositeX - m_currentLocation.x == -1 || oppositeX - m_currentLocation.x == 0 || oppositeX - m_currentLocation.x == 1 )
		{
			if ( oppositeY - m_currentLocation.y == -1 || oppositeY - m_currentLocation.y == 0 || oppositeY - m_currentLocation.y == 1 )
			{
				return true;
			}
		}

		return false;
	}

	void ActionMove::RandomMove()
	{
		SSL::ST_COORDINATE curLocation = GetCurLocation();
		int loopCount = 0;
		INT32 result = 1;
		SSL::ST_COORDINATE tempLocation;
		while ( result )
		{
			tempLocation = curLocation;
			tempLocation.x += rand() % 3 - 1;
			tempLocation.y += rand() % 3 - 1;
			result = IsEntityAt( tempLocation.x, tempLocation.y );
			if ( loopCount++ > 10 )
			{
				return;
			}
		}

		curLocation = tempLocation;

		if ( curLocation.x < 0 )
		{
			curLocation.x = 0;
		}
		else if ( curLocation.x > 14 )
		{
			curLocation.x = 14;
		}

		if ( curLocation.y < 0 )
		{
			curLocation.y = 0;
		}
		else if ( curLocation.y > 14 )
		{
			curLocation.y = 14;
		}

		SetCurLocation( curLocation.x, curLocation.y );
	}

}