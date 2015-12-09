#pragma once

#include <map>

#include <Common/CommonData.h>
#include <Core/Singleton.h>
#include <Entity/Entity.h>

namespace SSL
{
	class EntityManager : public Singleton<EntityManager>
	{
	public:
		//typedef std::map<INT32, const Entity*> ENTITY_MAP;
		typedef std::map<INT32, Entity*> ENTITY_MAP;
	private:
		ENTITY_MAP m_entityMap;

	public:
		bool RegisterEntity( Entity* entity );
		bool RemoveEntity(INT32 id);
		Entity* GetEntity( INT32 id );
		const ENTITY_MAP& GetEntityMap() { return m_entityMap; };
		void UpdateEntities();
	};
}

