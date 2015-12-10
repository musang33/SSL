#pragma once

#include <../FrameWork/Core/BaseFactoryImpl.h>

namespace SSL
{

	class FactoryPlayerImpl : public BaseFactoryImpl
	{
	public:
		FactoryPlayerImpl();
		~FactoryPlayerImpl();

	public:
		EN_ENTITY_TYPE GetFactoryType() override
		{
			return EN_ENTITY_TYPE::ENTITY_PLAYER_TYPE;
		}

		Entity* Create( UINT32 entityId ) override;
	};

}

