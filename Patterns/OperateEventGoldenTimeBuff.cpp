#include "OperateEventGoldenTimeBuff.h"

#include <Windows.h>

struct OperateEventGoldenTimeBuffImpl
{
	unsigned __int32	exp			= 0;	// 경험치 증가량
	unsigned __int32	dropRate	= 0;	// 아이템 드랍 확률 증가량
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
	// 서버에 적용 요청
}

// 해제
void		OperateEventGoldenTimeBuff::release( )
{
	// 서버에 해제 요청
}

// 할수 있나?
bool		OperateEventGoldenTimeBuff::can( ) const
{
	return true;
}

// OperateCommand를 Event의 데이터에 설정하는 함수
bool		OperateEventGoldenTimeBuff::parse( const OperateCommand& command )
{
	m_impl->exp			= ::_wtoi( command.values[ 0 ].c_str( ) );
	m_impl->dropRate	= ::_wtoi( command.values[ 0 ].c_str( ) );

	return true;
}
