#pragma once

#include <map>

#include "CommonData.h"
#include "Singleton.h"


namespace SSL
{
	class BaseEntity;

	class EntityManager : public Singleton<EntityManager>
	{
	private:
		std::map<INT32, const BaseEntity*> m_entityMap;

	public:
		bool RegisterEntity(const BaseEntity* entity);		
		bool RemoveEntity(INT32 id);
		const BaseEntity* GetEntity(INT32 id);
	};
}

