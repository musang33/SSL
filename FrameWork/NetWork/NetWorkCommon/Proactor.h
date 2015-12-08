#pragma once

#include <unordered_map>

#include "Event.h"

namespace SSL
{
	class CallBack;
	class TcpSocket;
	class Proactor
	{
	public:
		Proactor();
		~Proactor();

	public:
		static UINT32 WINAPI ThreadWorker( void* param );

	public:
		bool StartIocp( UINT32 threadCount );
		void RegisterSocket( SOCKET socket );

		void ProcEvent();
		bool PostEvent( TcpSocket* tcpsocket, void* ptr, const INT32 ptrSize );
		bool PostEvent( EventPtr& ep );

		bool SetEvent(USHORT type, CallBack* e);
		void Stop( );
		
	private:
		typedef std::unordered_map<USHORT, CallBack*> EVENTMAP;

	private:
		HANDLE		m_iocp;
		HANDLE*		m_threadHandle;
		UINT32		m_threadCount;
		EVENTMAP	m_eventMap;
		bool		m_stop;
	};
}