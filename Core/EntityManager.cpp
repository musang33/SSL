#include "EntityManager.h"

#include "BaseEntity.h"

namespace SSL
{
	bool EntityManager::RegisterEntity( BaseEntity::BaseEntityPtr entity )
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

	const BaseEntity::BaseEntityPtr EntityManager::GetEntity( INT32 id )
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
			if ( it.second->GetEntityState() == SSL::EN_ENTITY_STATE::STATE_ALIVE )
			{
				// AI 업데이트
				it.second->Update();
			}
		}
	}

}// namespace SSL