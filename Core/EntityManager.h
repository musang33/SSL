#pragma once

#include <map>

#include "CommonData.h"
#include "Singleton.h"


namespace SSL
{
	class BaseEntity;

	class EntityManager : public Singleton<EntityManager>
	{
	public:
		//typedef std::map<INT32, const BaseEntity*> ENTITY_MAP;
		typedef std::map<INT32, BaseEntity*> ENTITY_MAP;
	private:
		ENTITY_MAP m_entityMap;

	public:
		bool RegisterEntity( BaseEntity* entity);		
		bool RemoveEntity(INT32 id);
		const BaseEntity* GetEntity(INT32 id);
		const ENTITY_MAP& GetEntityMap() { return m_entityMap; };
	};
}

