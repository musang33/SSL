#pragma once

#include <Common/CommonData.h>

namespace SSL
{
	class Entity;
	class BaseFactoryImpl
	{
	public:
		BaseFactoryImpl() {};
		virtual ~BaseFactoryImpl() {};

	public:
		virtual EN_ENTITY_TYPE GetFactoryType() = 0;
		virtual Entity* Create( UINT32 entityId ) = 0;
	};

}