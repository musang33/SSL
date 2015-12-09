#include "stdafx.h"

#include <Entity/EntityFactory.h>
#include <Core/BaseFactoryImpl.h>
#include <Entity/Entity.h>

namespace SSL
{


	EntityFactory::EntityFactory()
	{
		m_seed = 1;
	}

	EntityFactory::~EntityFactory()
	{
	}

	void EntityFactory::AddFactoryImpl( BaseFactoryImpl* impl )
	{
		m_factoryMap.insert( std::map<UINT32, BaseFactoryImpl*>::value_type( impl->GetFactoryType(), impl ) );
	}

	Entity* EntityFactory::CreateEntity( UINT32 factoryType )
	{
		auto it = m_factoryMap.find( factoryType );
		if ( it != m_factoryMap.end() )
		{
			return it->second->Create( m_seed++ );
		}

		return nullptr;
	}
}