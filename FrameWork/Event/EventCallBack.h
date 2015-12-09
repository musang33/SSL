#pragma once

#include <Event/Event.h>
#include <Core/MessageQueue.h>

namespace SSL
{

	class TcpSocket;
	class EventListener
	{
	public:
		EventListener( )
		{
		}
		virtual ~EventListener( )
		{
		}

		void Notify( const EventPtr& ep );

	protected:
		EventConcurrentQueue	m_queue;
	};

	inline void EventListener::Notify( const EventPtr& ep )
	{
		m_queue.Push( ep );
	}

	class CallBack
	{
	public:
		virtual ~CallBack( )
		{
		};
		virtual bool Proc( UINT32 idx, const void* ptr, const INT32 ptrSize ) const = 0;

		virtual void Notify( const EventPtr& ep ) const
		{
			ep;
		}
	};

	template <typename EVENT>
	class EventCallBack : public CallBack
	{
	public:
		EventCallBack( EventListener* listener );
		~EventCallBack( )
		{
		};

		bool Proc( UINT32 idx, const void* ptr, const INT32 ptrSize ) const;

		void Notify( const EventPtr& ep ) const;

	private:
		EventListener*	m_listener;
	};

	template <typename EVENT>
	inline EventCallBack<EVENT>::EventCallBack( EventListener* listener )
		:m_listener( listener )
	{
	}

	template <typename EVENT>
	bool EventCallBack<EVENT>::Proc( UINT32 idx, const void* ptr, const INT32 ptrSize ) const
	{
		const EventHead* head = static_cast< const EventHead* >( ptr );
		if( head->GetEvent( ) < Event::eEventBegin )
		{
			return false;
		}

		PacketStream bs( ( BYTE* ) ptr, ptrSize );
		EVENT* ntr = new EVENT;
		if( !ntr->Unpack( bs ) )
		{
			return false;
		}

		ntr->idx = idx;
		Notify( EventPtr(ntr) );

		return true;	
	}

	template <typename EVENT>
	void EventCallBack<EVENT>::Notify(const EventPtr& ep) const
	{
		if (m_listener)
		{
			m_listener->Notify(ep);
		}		
	}
}