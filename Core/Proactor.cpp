#include "Act.h"
#include "Proactor.h"
#include <process.h>

#include "CommonData.h"
#include "Event.h"
#include "EventCallBack.h"
#include "Actor.h"
#include "TcpSocket.h"

namespace SSL
{

	Proactor::Proactor()
	{
	}


	Proactor::~Proactor()
	{
	}

	UINT32 WINAPI Proactor::ThreadWorker( void* param )
	{
		Proactor* proactor = reinterpret_cast< Proactor* >( param );
		if ( nullptr != proactor )
		{
			proactor->ProcEvent();
		}

		return 0;
	}

	bool Proactor::StartIocp( UINT32 threadCount )
	{
		if ( threadCount < 1 )
		{
			return false;
		}

		SYSTEM_INFO sysInfo;
		GetSystemInfo( &sysInfo );

		m_threadCount = threadCount;

		m_iocp = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, NULL, m_threadCount );
		if ( m_iocp == NULL )
		{
			return false;
		}

		m_threadHandle = new HANDLE[m_threadCount];
		for ( UINT32 i = 0; i < m_threadCount; i++ )
		{
			UINT32 threadId;
			m_threadHandle[i] = reinterpret_cast<HANDLE>( _beginthreadex( nullptr, 0, ThreadWorker, this, 0, &threadId ) );
			if ( m_threadHandle[i] == NULL )
			{
				return false;
			}
		}

		return true;
	}

	void Proactor::RegisterSocket( SOCKET socket )
	{
		assert( m_iocp != NULL );
		::CreateIoCompletionPort( reinterpret_cast<HANDLE>(socket), m_iocp, 0, 0 );
	}

	void Proactor::ProcEvent()
	{
		PULONG_PTR completionKey = nullptr;

		while ( true )
		{
			DWORD trasferedBytes = 0;
			Act* act = nullptr;

			BOOL result = ::GetQueuedCompletionStatus(
				m_iocp, &trasferedBytes
				, reinterpret_cast<PULONG_PTR>(completionKey)
				, (LPOVERLAPPED*)&act
				, INFINITE );
								

			if ( nullptr == act )
			{
				assert( act != nullptr );
				break;
			}

			if ( result == TRUE )
			{
				act->m_actor->Complete( act, trasferedBytes );
			}
			else
			{
				DWORD error = GetLastError();
				act->m_actor->Error( act, error );
			}
		}
	}

	bool Proactor::PostEvent( TcpSocket* tcpsocket, void* ptr, const INT32 ptrSize )
	{
		if (nullptr == tcpsocket || nullptr == ptr)
		{
			return false;
		}

		EventStream* stream = static_cast<EventStream*>(ptr);

		EVENTMAP::const_iterator it;

		it = m_eventMap.find(EVENTMAP::key_type(stream->eventCode));
		if (it != m_eventMap.end())
		{
			if( false == it->second->Proc( tcpsocket->GetUniqueId(), ptr, ptrSize ) )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			// Unregistered event
			return false;
		}

		return true;
	}

	bool Proactor::SetEvent(USHORT type, CallBack* e)
	{
		auto result = m_eventMap.insert(Proactor::EVENTMAP::value_type(type, e));
		if (result.second == false)
		{
			return false;		
		}

		return true;
	}
}