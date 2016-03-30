#pragma once

#include "StructuresOfCondition.h"

struct EventInfo
{
	INT32	type = 0;
	INT32	level = 0;
	INT32	exp = 0;
	INT32	count = 0;
	INT32	money = 0;
};

struct Event
{
public:
	explicit Event( UINT32 entityId, INT32 type )
	{
		m_entityId = entityId;
		m_info.type = type;
	}

	virtual ~Event( )
	{
	}
	INT32	GetEventType( ) const
	{
		return m_info.type;
	}

public:
	EventInfo	m_info;
	UINT32		m_entityId;
};


struct EventLevelUp : public Event
{
	EventLevelUp( UINT32 entityId, BYTE level )
		: Event( entityId, ConditionType::LEVEL_UP )		
	{ 
		m_info.level = level;
	}
};