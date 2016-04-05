#include "OperateEvent.h"



OperateEvent::OperateEvent( OperateEventType type )
{
}


OperateEvent::~OperateEvent( )
{
}

void				OperateEvent::Apply( )
{
	// DB�� �����ߴٰ� �˸���.
	apply( );
}

void				OperateEvent::Release( )
{
	// DB�� ���� �ߴٰ� �˸���.
	release( );
}

bool				OperateEvent::Can( ) const
{
	// �������ΰ� üũ�Ѵ�. ���� �ð��� �ȴ�. 
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