#pragma once

#include "StructuresOfCondition.h"
#include "StructuresOfEvent.h"
#include <array>
#include <map>

class ConditionGroup
{
public:
	using ConditionCheckFunction = bool( ConditionGroup::* )( const Event*, const Condition* );
	using ConditionCheckFunctionList = std::array< ConditionCheckFunction, ConditionType::TYPE_END >;

public:
	ConditionGroup( );
	~ConditionGroup( );

public:
	bool			Check( const Event* event ) const;
	bool			Process( const Event* event );

private:
	bool			checkLevelUp( const Event* event, const Condition* condition );
	bool			checkGetMoney( const Event* event, const Condition* condition );

private:
	const ConditionUnit*	m_conditionUnit = nullptr;

private:
	static ConditionCheckFunctionList	m_handler;
};

using ConditionGroupMap = std::map< UINT32, ConditionGroup>;

