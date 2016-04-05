#include "OperateEvent.h"



OperateEvent::OperateEvent( OperateEventType type )
{
}


OperateEvent::~OperateEvent( )
{
}

void				OperateEvent::Apply( )
{
	// DB에 적용했다고 알린다.
	apply( );
}

void				OperateEvent::Release( )
{
	// DB에 해제 했다고 알린다.
	release( );
}

bool				OperateEvent::Can( ) const
{
	// 공통적인걸 체크한다. 보통 시간이 된다. 
	can( );
}

bool				OperateEvent::Parse( const OperateCommand& command )
{
	m_id = command.id;

	parse( command );
}

OperateEventType	OperateEvent::GetType( ) const
{
	return m_event;
}

unsigned __int64	OperateEvent::GetID( ) const
{
	return m_id;
}