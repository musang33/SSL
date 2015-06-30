#include "EntityManager.h"

#include "BaseEntity.h"

namespace SSL
{
	bool EntityManager::RegisterEntity( BaseEntity* entity )
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

	const BaseEntity* EntityManager::GetEntity(INT32 id)
	{
		const auto it = m_entityMap.find(id);

		if ( m_entityMap.end() != it )
		{
			return it->second;
		}

		return nullptr;
	}

}// namespace SSL