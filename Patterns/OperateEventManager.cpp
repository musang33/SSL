#include "OperateEventManager.h"
#include "OperateEventGoldenTimeBuff.h"
#include "OperateEventNotifier.h"

#include <assert.h>

struct OperateEventManagerImpl
{
	OperateEventManager::OperateEventMap	eventMap;
	OperateEventManager::EventFactory		eventFactory;
	OperateEventNotifier					eventNotifier;
};

OperateEventManager::OperateEventManager( )
{
	m_impl->eventFactory.fill( nullptr );
	m_impl->eventFactory[ OperateEventType::TIME_BUFF ] = &OperateEventManager::createGoldenTimeBuffEvent();
}

OperateEventManager::~OperateEventManager( )
{
}

void	OperateEventManager::UpdateEvent( const OperateCommands& commandList )
{
	IDList	processedList;
	IDList	preProcessedList;
	IDList	removeList;

	std::transform( m_impl->eventMap.begin( ), m_impl->eventMap.end( ), std::inserter( preProcessedList, preProcessedList.begin( ) ), std::bind( &OperateEventMap::value_type::first, std::placeholders::_1 ) );
	for( auto &it : commandList )
	{
		if( false == process( it ) )
		{
			continue;
		}

		processedList.emplace( it.id );
	}

	std::set_difference( preProcessedList.begin( ), preProcessedList.end( ), processedList.begin( ), preProcessedList.end( ), std::inserter( removeList, removeList.begin( ) );

	for( auto &it : removeList )
	{
		remove( it );
	}

	if( processedList != preProcessedList )
	{
		update( );
	}	
}

void	OperateEventManager::OnEnter( unsigned __int64 entityId )
{
	m_impl->eventNotifier( entityId );
}

bool	OperateEventManager::process( const OperateCommand& command )
{
	OperateEventPtr eventPtr = parse( command );
	
	if( eventPtr->Can( ) )
	{
		eventPtr->Apply( );
		add( eventPtr );
	}	
}

void	OperateEventManager::add( OperateEventPtr& eventPtr )
{
	assert( m_impl->eventMap.emplace( eventPtr->GetID( ), eventPtr ).second );
}

void	OperateEventManager::remove( unsigned __int64 eventId )
{
	auto it = m_impl->eventMap.find( eventId );
	if( it == m_impl->eventMap.end( ) )
	{
		return;
	}

	m_impl->eventNotifier.Release( it->second );
	it->second->release( );
	m_impl->eventMap.erase( it );
}

void	OperateEventManager::update( )
{
	m_impl->eventNotifier.UpdateNotifyPacket( );
	// 존 서버한테 노티
}

OperateEventPtr	OperateEventManager::parse( const OperateCommand& command )
{
	auto factory = m_impl->eventFactory[ command.actionCode ];
	
	auto eventPtr = ( this->*factory )( );
	if( eventPtr == nullptr )
	{
		return nullptr;
	}

	if( false == eventPtr->Parse( command ) )
	{
		return nullptr;
	}

	return eventPtr;
}


OperateEventPtr	OperateEventManager::createGoldenTimeBuffEvent( )
{	
	return std::make_shared<OperateEventGoldenTimeBuff>( );
}