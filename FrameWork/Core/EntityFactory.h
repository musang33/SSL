#pragma once
#include <map>

#include "CommonData.h"
#include "Singleton.h"
#include <atomic>

namespace SSL
{
	class Entity;
	class BaseFactoryImpl;
	class EntityFactory : public Singleton<EntityFactory>
	{
	public:
		EntityFactory();
		~EntityFactory();

	public:
		void AddFactoryImpl( BaseFactoryImpl* impl );
		Entity* CreateEntity( UINT32 EntityType );

	private:
		std::map<INT32, BaseFactoryImpl*>	m_factoryMap; // <factory type, factory*>
		std::atomic<UINT32>					m_seed;
	};

}