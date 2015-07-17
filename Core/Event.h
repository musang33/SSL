#pragma once

#include "CommonData.h"
#include <future>

namespace SSL
{
	enum EN_EVENT
	{
		ADD_HP = 100,
		ENTITY_UPDATE,
	};
	
	struct ST_EVENT
	{
		UINT32	eventIndex;
		UINT32	entityIndex;		

		ST_EVENT( UINT32 _eventIndex, UINT32 _entityIndex )
			: eventIndex( _eventIndex )
			, entityIndex( _entityIndex )
		{}
	};	

	typedef std::shared_ptr< ST_EVENT > EVENTPtr;

	struct ST_ADD_HP : public ST_EVENT
	{
		INT32	addHP;

		ST_ADD_HP( UINT32 _entityIndex, INT32 _addHp )
			: ST_EVENT( ADD_HP, _entityIndex )
			, addHP( _addHp )
		{
		}		
	};

	struct ST_ENTITY_UPDATE : public ST_EVENT
	{
		ST_ENTITY_UPDATE( UINT32 _entityIndex )
			: ST_EVENT( ENTITY_UPDATE, _entityIndex )
		{
		}
	};
}