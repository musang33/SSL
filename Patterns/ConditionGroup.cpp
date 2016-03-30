#include "ConditionGroup.h"


ConditionGroup::ConditionCheckFunctionList ConditionGroup::m_handler =
{
	&ConditionGroup::checkLevelUp,
	&ConditionGroup::checkGetMoney
};

ConditionGroup::ConditionGroup( )
{	
	assert( m_handler.end( ) == std::find( m_handler.begin( ), m_handler.end( ), nullptr ) );
}

ConditionGroup::~ConditionGroup( )
{
}

bool			ConditionGroup::Check( const Event* event ) const
{
	return ( event->GetEventType() == 
}

bool			ConditionGroup::Process( const Event* event )
{

}

bool			ConditionGroup::checkLevelUp( const Event* event, const Condition* condition )
{
	auto purposeValue = static_cast< const ConditionLevelUp* >( condition );
	return ( event->m_info.level == purposeValue->level );
}

bool			ConditionGroup::checkGetMoney( const Event* event, const Condition* condition )
{
	auto purposeValue = static_cast< const ConditionGetMoney* >( condition );
	return ( event->m_info.money >= purposeValue->money );
}
