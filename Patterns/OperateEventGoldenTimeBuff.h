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
	// ������ �� �ؾ� �� ��
	virtual void		OnEnter( unsigned __int64 entityId ) const override;

public:
	// Ŭ���̾�Ʈ ������ ���� OperateEventGoldenTimeBuff�� ������ �ִ� �����͸�
	// ������� ��Ŷ�� ������ �ش�.
	// void				FillUp( ��Ŷ )

protected:
	// ����
	virtual void		apply( ) override;
	// ����
	virtual void		release( ) override;

	// �Ҽ� �ֳ�?
	virtual bool		can( ) const override;
	// OperateCommand�� Event�� �����Ϳ� �����ϴ� �Լ�
	virtual bool		parse( const OperateCommand& command ) override;

private:
	OperateEventGoldenTimeBuffImplPtr	m_impl;
};

