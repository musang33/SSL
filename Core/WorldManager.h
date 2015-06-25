#pragma once

#include "CommonData.h"
#include "BaseEntity.h"
#include "Singleton.h"

#include <map>

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
		typedef std::map<INT32, BaseEntity*> ENTITY_MAP;
		
	public:
		void AddEntity( BaseEntity* entity ) 
		{ 
			m_entityMap.insert( ENTITY_MAP::value_type( entity->ID(), entity ) );
		};

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

		const ENTITY_MAP& GetWorldEntityMap() { return m_entityMap; };

	private:
		ENTITY_MAP		m_entityMap;
		UINT32			m_xLength;
		UINT32			m_yLength;
	};

}


