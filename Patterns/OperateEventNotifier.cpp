#include <assert.h>

#include "OperateEventNotifier.h"

#include "OperateEventGoldenTimeBuff.h"

struct OperateEventNotifierImpl
{
	OperateEventNotifier::Notifiers				m_applyNotifiers;
	OperateEventNotifier::Notifiers				m_releaseNotifiers;
	OperateEventNotifier::PacketMakers			m_packetMakers;
	OperateEventNotifier::OperateEventTypeList	m_eventTypeList;
	OperateEventNotifier::NotifyPacketList		m_packetList;
};

OperateEventNotifier::OperateEventNotifier( )
{
	initNotifier( );
	initPacketMaker( );
}


OperateEventNotifier::~OperateEventNotifier( )
{
}


void		OperateEventNotifier::Apply( OperateEventPtr& eventPtr )
{
	apply( eventPtr );
}

void		OperateEventNotifier::Release( OperateEventPtr& eventPtr )
{
	release( eventPtr );
}

void		OperateEventNotifier::UpdateNotifyPacket( )
{
	for( unsigned __int16 i = 0; i < m_impl->m_packetMakers.max_size( ); i++ )
	{
		auto& packetPtr = m_impl->m_packetList[ i ];
		auto& list = m_impl->m_eventTypeList[ i ];

		( this->*m_impl->m_packetMakers[ i ] )( packetPtr, list );
	}
}

void		OperateEventNotifier::apply( OperateEventPtr& eventPtr )
{
	( this->*m_impl->m_applyNotifiers[ eventPtr->GetType( ) ] )( eventPtr );
	add( eventPtr );
}

void		OperateEventNotifier::release( OperateEventPtr& eventPtr )
{
	( this->*m_impl->m_releaseNotifiers[ eventPtr->GetType( ) ] )( eventPtr );
	release( eventPtr );
}

void		OperateEventNotifier::applyDummy( OperateEventPtr& /*eventPtr*/ ) const
{
	
}
void		OperateEventNotifier::releaseDummy( OperateEventPtr& /*eventPtr*/ ) const
{

}

void		OperateEventNotifier::applyGoldenTimeBuff( OperateEventPtr& eventPtr ) const
{
	// 클라이언트한테 패킷 발송
	// eventPtr->GetID( );
}

void		OperateEventNotifier::releaseGoldenTimeBuff( OperateEventPtr& eventPtr ) const
{
	// 클라이언트한테 패킷 발송
	// eventPtr->GetID( );
}

void		OperateEventNotifier::packetMakerDummy( EventPtr& /*packetPtr*/, OperateEventSet& /*eventPtr*/ ) const
{
	
}

void		OperateEventNotifier::packetMakerGoldenTimeBuff( EventPtr& packetPtr, OperateEventSet& eventPtr ) const
{
	// GoldenTimeBuff 패킷 생성
	ENtfGoldenTimerBuff *ntf = new ENtfGoldenTimerBuff();

	for( auto& it : eventPtr )
	{
		OperateEventGoldenTimeBuff * event = static_cast< OperateEventGoldenTimeBuff >( it );

		GoldenTimeBuff buff;
		event->FillUp( buff );
		ntf->buffInfo.push_back( buff );
	}

	packetPtr = ntf;
}

void		OperateEventNotifier::initNotifier( )
{
	m_impl->m_applyNotifiers.fill( &OperateEventNotifier::applyDummy );
	m_impl->m_applyNotifiers[ OperateEventType::TIME_BUFF ] = &OperateEventNotifier::applyGoldenTimeBuff;

	m_impl->m_releaseNotifiers.fill( &OperateEventNotifier::releaseDummy );
	m_impl->m_releaseNotifiers[ OperateEventType::TIME_BUFF ] = &OperateEventNotifier::releaseGoldenTimeBuff;
}

void		OperateEventNotifier::initPacketMaker( )
{
	m_impl->m_packetMakers.fill( &OperateEventNotifier::packetMakerDummy );
	m_impl->m_packetMakers[ OperateEventType::TIME_BUFF ] = &OperateEventNotifier::packetMakerGoldenTimeBuff;
}

void		OperateEventNotifier::add( OperateEventPtr& eventPtr )
{
	const auto& list = m_impl->m_eventTypeList[ eventPtr->GetType( ) ];
	if( list.end( ) == list.find( eventPtr ) )
	{
		assert( m_impl->m_eventTypeList[ eventPtr->GetType( ) ].insert( eventPtr ).second );
	}
}

void		OperateEventNotifier::remove( OperateEventPtr& eventPtr )
{
	auto& list = m_impl->m_eventTypeList[ eventPtr->GetType( ) ];
	if( list.end( ) != list.find( eventPtr ) )
	{
		list.erase( eventPtr );
	}
}