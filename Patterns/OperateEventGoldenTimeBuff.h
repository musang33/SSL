#pragma once
#include "OperateEvent.h"
#include <memory>

struct OperateEventGoldenTimeBuffImpl;
using OperateEventGoldenTimeBuffImplPtr = std::unique_ptr<OperateEventGoldenTimeBuffImpl>;

class OperateEventGoldenTimeBuff : 	public OperateEvent
{
public:
	OperateEventGoldenTimeBuff( OperateEventType tyep );
	~OperateEventGoldenTimeBuff( ) = default;

public:
	// 입장할 때 해야 할 일
	virtual void		OnEnter( unsigned __int64 entityId ) const override;

public:
	// 클라이언트 전달을 위해 OperateEventGoldenTimeBuff가 가지고 있는 데이터를
	// 기반으로 패킷을 셋팅해 준다.
	// void				FillUp( 패킷 )

protected:
	// 적용
	virtual void		apply( ) override;
	// 해제
	virtual void		release( ) override;

	// 할수 있나?
	virtual bool		can( ) const override;
	// OperateCommand를 Event의 데이터에 설정하는 함수
	virtual bool		parse( const OperateCommand& command ) override;

private:
	OperateEventGoldenTimeBuffImplPtr	m_impl;
};

