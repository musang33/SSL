#pragma once

#include <vector>
#include <string>

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
	// 입장할 때 해야 할 일
	virtual void		OnEnter( unsigned __int64 entityId ) const abstract;

protected:
	// 적용
	virtual void		apply( ) abstract;
	// 해제
	virtual void		release( ) abstract;

	// 할수 있나?
	virtual bool		can( ) const abstract;
	// OperateCommand를 Event의 데이터에 설정하는 함수
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

