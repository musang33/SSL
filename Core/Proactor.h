#pragma once

#include <Windows.h>

namespace SSL
{
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
		

	private:
		HANDLE		m_iocp;
		HANDLE*		m_threadHandle;
		UINT32		m_threadCount;
	};
}