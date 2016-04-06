#include "OperateEventGoldenTimeBuff.h"

#include <Windows.h>

struct OperateEventGoldenTimeBuffImpl
{
	unsigned __int32	exp			= 0;	// ����ġ ������
	unsigned __int32	dropRate	= 0;	// ������ ��� Ȯ�� ������
};

OperateEventGoldenTimeBuff::OperateEventGoldenTimeBuff( OperateEventType tyep )
{
}


OperateEventGoldenTimeBuff::~OperateEventGoldenTimeBuff( )
{
}

void		OperateEventGoldenTimeBuff::OnEnter( unsigned __int64 entityId ) const
{
	UNREFERENCED_PARAMETER( entityId );
}

void		OperateEventGoldenTimeBuff::apply( )
{
	// ������ ���� ��û
}

// ����
void		OperateEventGoldenTimeBuff::release( )
{
	// ������ ���� ��û
}

// �Ҽ� �ֳ�?
bool		OperateEventGoldenTimeBuff::can( ) const
{
	return true;
}

// OperateCommand�� Event�� �����Ϳ� �����ϴ� �Լ�
bool		OperateEventGoldenTimeBuff::parse( const OperateCommand& command )
{
	m_impl->exp			= ::_wtoi( command.values[ 0 ].c_str( ) );
	m_impl->dropRate	= ::_wtoi( command.values[ 0 ].c_str( ) );

	return true;
}
