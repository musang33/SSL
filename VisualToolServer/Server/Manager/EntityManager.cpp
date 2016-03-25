#include "stdafx.h"

#include "EntityManager.h"

#include <../FrameWork/Entity/Entity.h>
#include "../Action/ActionAI.h"

namespace SSL
{
	bool EntityManager::RegisterEntity( Entity* entity )
	{
		if ( nullptr == entity )
		{
			return false;
		}

		if ( m_entityMap.end() != m_entityMap.find(entity->ID()) )
		{
			return false;
		}

		m_entityMap.insert( ENTITY_MAP::value_type( entity->ID(), entity ) );

		return true;
	}

	bool EntityManager::RemoveEntity(INT32 id)
	{
		const auto it = m_entityMap.find(id);

		if ( m_entityMap.end() != it )
		{
			m_entityMap.erase(it);
			return true;
		}

		return false;
	}

	Entity* EntityManager::GetEntity( INT32 id )
	{
		const auto it = m_entityMap.find(id);

		if ( m_entityMap.end() != it )
		{
			return it->second;
		}

		return nullptr;
	}

	void EntityManager::UpdateEntities()
	{
		const EntityManager::ENTITY_MAP& entityMap = EntityManager::GetInstance()->GetEntityMap();
		for ( const auto &it : entityMap )
		{
			ActionAI* aa = GetEntityAction( it.second );			
			if( aa != nullptr )
			{
				// AI 업데이트				
				aa->Update( );
			}			
		}
	}

}// namespace SSL