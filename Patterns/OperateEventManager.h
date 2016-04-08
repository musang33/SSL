#pragma once

#include "OperateEvent.h"

#include <map>
#include <memory>
#include <set>

struct OperateEventManagerImpl;
using OperateEventManagerImplPtr = std::unique_ptr<OperateEventManagerImpl>;

class OperateEventManager
{
public:
	using OperateEventMap		= std::map<unsigned __int64, OperateEventPtr>;
	using EventFactoryFunction	= OperateEventPtr ( OperateEventManager::* )( ) const;
	using EventFactory			= std::array< EventFactoryFunction, OperateEventType::OPERATE_EVENT_MAX>;
	using IDList				= std::set< unsigned __int64 >;

public:
	OperateEventManager( );
	~OperateEventManager( );

public:
	void						UpdateEvent( const OperateCommands& commandList );
	void						OnEnter( unsigned __int64 );

private:
	bool						process( const OperateCommand& command );
	void						add( OperateEventPtr& eventPtr );
	void						remove( unsigned __int64 eventId );
	void						update( );

	OperateEventPtr				parse( const OperateCommand& command );


	OperateEventPtr				createGoldenTimeBuffEvent( );


private:
	OperateEventManagerImplPtr	m_impl;
};

