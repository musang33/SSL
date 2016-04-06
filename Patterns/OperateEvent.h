#pragma once

#include <vector>
#include <string>
#include <memory>

enum OperateEventType
{
	TIME_BUFF,
	TIME_GIFT,

	OPERATE_EVENT_MAX,
};

enum class OperateEventStatus : unsigned __int8
{
	PROCES,
	COMPLETE,

	MAX,
};

struct OperateCommand
{
public:
	using Values = std::vector<std::wstring>;

public:
	unsigned __int64	id			= 0;
	unsigned __int16	actionCode	= 0;
	time_t				startDate;
	time_t				endDate;
	Values				values;

public:
	OperateCommand( )
	{
		startDate	= { 0, };
		endDate		= { 0, };
	}

	// bool Pack( PacketStream& bs )
	// bool Unpack( PacketStream& bs )
};


class OperateEvent
{
public:
	OperateEvent( OperateEventType tyep );

public:
	virtual ~OperateEvent( ) = default;

public:
	// ������ �� �ؾ� �� ��
	virtual void		OnEnter( unsigned __int64 entityId ) const abstract;

protected:
	// ����
	virtual void		apply( ) abstract;
	// ����
	virtual void		release( ) abstract;

	// �Ҽ� �ֳ�?
	virtual bool		can( ) const abstract;
	// OperateCommand�� Event�� �����Ϳ� �����ϴ� �Լ�
	virtual bool		parse( const OperateCommand& command ) abstract;

public:
	void				Apply( );
	void				Release( );
	bool				Can( ) const;
	bool				Parse( const OperateCommand& command );
	OperateEventType	GetType( ) const;
	unsigned __int64	GetID( ) const;

private:
	OperateEventType	m_event = OperateEventType::OPERATE_EVENT_MAX;
	unsigned __int64	m_id	= 0;	
};

using OperateEventPtr = std::shared_ptr<OperateEvent>;
