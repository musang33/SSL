#pragma once

#include <memory>
#include <set>
#include <array>
#include "OperateEvent.h"

struct OperateEventNotifierImpl;
using OperateEventNotifierImplPtr = std::unique_ptr<OperateEventNotifierImpl>;

class OperateEventNotifier
{
public:
	using OperateEventSet		= std::set<OperateEventPtr>;
	using Notifier				= void( OperateEventNotifier::* )( OperateEventPtr& ) const;
	using Notifiers				= std::array<Notifier, OperateEventType::OPERATE_EVENT_MAX>;
	using PacketMaker			= void( OperateEventNotifier::* )( EventPtr&, OperateEventSet& ) const;
	using PacketMakers			= std::array<PacketMaker, OperateEventType::OPERATE_EVENT_MAX>;
	using NotifyPacketList		= std::array<EventPtr, OperateEventType::OPERATE_EVENT_MAX>;
	using OperateEventTypeList	= std::array<OperateEventSet, OperateEventType::OPERATE_EVENT_MAX>;
	
public:
	OperateEventNotifier( );
	~OperateEventNotifier( );

public:
	void		Apply( OperateEventPtr& eventPtr );
	void		Release( OperateEventPtr& eventPtr );
	void		UpdateNotifyPacket( );
	
private:
	void		apply( OperateEventPtr& eventPtr );
	void		release( OperateEventPtr& eventPtr );

	void		applyDummy( OperateEventPtr& eventPtr ) const;
	void		releaseDummy( OperateEventPtr& eventPtr ) const;

	void		applyGoldenTimeBuff( OperateEventPtr& eventPtr ) const;
	void		releaseGoldenTimeBuff( OperateEventPtr& eventPtr ) const;

	void		packetMakerDummy( EventPtr& packetPtr, OperateEventPtr& eventPtr ) const;
	void		packetMakerGoldenTimeBuff( EventPtr& packetPtr, OperateEventPtr& eventPtr ) const;


	void		initNotifier( );
	void		initPacketMaker( );

	void		add( OperateEventPtr& eventPtr );
	void		remove( OperateEventPtr& eventPtr );

private:
	OperateEventNotifierImplPtr		m_impl;
};


