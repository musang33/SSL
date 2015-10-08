#pragma once

#include "CommonData.h"
#include <future>
#include "PacketStream.h"
#include "MessageQueue.h"

namespace SSL
{
	enum EN_EVENT
	{
		ADD_HP = 100,
		ENTITY_UPDATE,
	};

	struct ST_EVENT
	{
		UINT32	eventIndex;
		UINT32	entityIndex;

		ST_EVENT( UINT32 _eventIndex, UINT32 _entityIndex )
			: eventIndex( _eventIndex )
			, entityIndex( _entityIndex )
		{}
	};

	typedef std::shared_ptr< ST_EVENT > EVENTPtr;

	struct ST_ADD_HP : public ST_EVENT
	{
		INT32	addHP;

		ST_ADD_HP( UINT32 _entityIndex, INT32 _addHp )
			: ST_EVENT( ADD_HP, _entityIndex )
			, addHP( _addHp )
		{
		}
	};

	struct ST_ENTITY_UPDATE : public ST_EVENT
	{
		ST_ENTITY_UPDATE( UINT32 _entityIndex )
			: ST_EVENT( ENTITY_UPDATE, _entityIndex )
		{
		}
	};

	// =============================================================

#define EVENT_DECL_BEGIN( event_class, super_class, protocol )\
	struct event_class : public super_class { \
	private: \
		typedef super_class super; \
	public:\
		static const USHORT kProtocol = protocol; \
		event_class() { \
			SetHead(kProtocol); \
			Reset(); \
		}\
		virtual ~event_class() {};
#define EVENT_DECL_END };

	struct EventStream
	{
		USHORT eventCode;
		UINT32 entityIndex;		

		EventStream()
			: eventCode(0)
			, entityIndex( 0 )
		{}
	};

	class EventHead
	{
	public:
		EventHead( const USHORT eventType = 0 )
			:m_eventType(eventType)
		{}

		USHORT GetEvent() const
		{
			return m_eventType;
		}

		void SetHead( USHORT eventType )
		{
			m_eventType = eventType;
		}

	private:
		USHORT m_eventType;
	};

	struct Event : public EventHead
	{
		enum
		{
			 eEventBegin = 0
			,eEventStart
			,eEventStop
			,eEventAccept
			,eEventServerLine

			,eEventConnected
			,eEventDisconnected
			
			,eEventMax = 100000
		};

		UINT32 entityIndex;

		Event()			
		{
			Reset();
		}
		virtual ~Event() {};

		void Reset()
		{
			entityIndex = 0;
		}

		virtual bool Pack( PacketStream& bs )
		{
			bs.Write( GetEvent() );
			bs.Write( entityIndex );

			return true;
		}

		virtual bool Unpack( PacketStream& bs )
		{
			USHORT cd;
			bs.Read( cd );
			SetHead( cd );
			bs.Read( entityIndex );

			return true;
		}

	};

	typedef std::shared_ptr<Event> EventPtr;
	typedef MessageConcurrentQueue<EventPtr> EventConcurrentQueue;

	EVENT_DECL_BEGIN( EventConnected, Event, Event::eEventConnected )

	EVENT_DECL_END
}