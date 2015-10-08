#pragma once

#include <WinSock2.h>
#include <Windows.h>

#include <unordered_map>

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

		bool SetEvent(USHORT type, CallBack* e);
		
	private:
		typedef std::unordered_map<USHORT, CallBack*> EVENTMAP;

	private:
		HANDLE		m_iocp;
		HANDLE*		m_threadHandle;
		UINT32		m_threadCount;
		EVENTMAP	m_eventMap;
	};
}