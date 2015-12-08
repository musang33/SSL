#pragma once

#include "BaseFactoryImpl.h"

namespace SSL
{

	class FactoryNpcImpl : public BaseFactoryImpl
	{
	public:
		FactoryNpcImpl();
		~FactoryNpcImpl();

	public:
		EN_ENTITY_TYPE GetFactoryType() override
		{
			return EN_ENTITY_TYPE::ENTITY_NPC_TYPE;
		}

		Entity* Create( UINT32 entityId ) override;
	};

}

