#pragma once

#include "CommonData.h"
#include "BaseEntity.h"
#include "Singleton.h"
#include "EntityManager.h"

namespace SSL
{
	class WorldManager : public Singleton<WorldManager>
	{
	public:
		WorldManager()
			: m_xLength( 0 )
			, m_yLength( 0 )
		{};
		~WorldManager(){};
		
	public:		
		BOOL IsNear( int myX, int myY, int oppositeX, int oppositeY )
		{
			if ( oppositeX - myX == -1 || oppositeX - myX == 0 || oppositeX - myX == 1 )
			{
				if ( oppositeY - myY == -1 || oppositeY - myY == 0 || oppositeY - myY == 1 )
				{
					return true;
				}
			}

			return false;
		}

		INT32 IsEntityAt( int x, int y ) const
		{			
			const EntityManager::ENTITY_MAP& entityMap = EntityManager::GetInstance()->GetEntityMap();
			for ( const auto &it : entityMap )
			{								
				const ST_COORDINATE location = it.second->GetCurLocation();
				if ( location.x == x && location.y == y )
				{
					return it.first;
				}			
			}

			return 0;
		}		

	private:		
		UINT32			m_xLength;
		UINT32			m_yLength;		
	};

}


