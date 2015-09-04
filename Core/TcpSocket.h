#pragma once
#include <WinSock2.h>
#include "CommonData.h"
#include "PacketStream.h"
#include "Act.h"
#include "MemoryPool.h"
#include "Event.h"

namespace SSL
{
	class TcpSocket
	{
		enum EN_ACT_TYPE
		{
			ACT_ACCEPT = 0,
			ACT_SEND,
			ACT_RECV,
			ACT_CONNECT,
			ACT_TYPE_COUNT
		};
	public:
		TcpSocket( SOCKET listenSocket = INVALID_SOCKET );
		~TcpSocket();

	public:
		void Create();
		void Disconnect();
		bool RequestRecv();
		bool Send(const EventPtr& e );
		INT32 OnRecvCompleted( Act* act, Proactor* proactor, UINT32 len );
		void OnSendCompleted( SendAct* act );
		void SetReserve( bool reserve );		
		bool GetReserve();

	private:
		bool requestSend();
		void packetize( Proactor* proactor );
		bool purgeSend( SendAct* sendAct );

	private:
		SOCKET			m_socket;
		sockaddr_in		m_socketAddr;
		Act*			m_act[ACT_TYPE_COUNT];
		WSABUF			m_recvWsaBuf;
		PacketStream	m_recvActBuf;

		SendAct			m_sendBuf[2];
		SendAct*		m_curSendAct;
		bool			m_isSending;

		PacketStream	m_recvBuf;

		std::mutex		g_i_mutex;
		bool			m_reserve;

	protected:
		static MemoryPool<PacketStream> ms_sendBufferPool;
		static MemoryPool<PacketStream> ms_recvBufferPool;
	};

}