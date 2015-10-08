#pragma once
#include "Act.h"
#include "CommonData.h"
#include "PacketStream.h"
#include "MemoryPool.h"
#include "Event.h"

namespace SSL
{
	class Proactor;
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

		struct SessionId
		{
			union
			{
				struct
				{
					USHORT	seq;
					USHORT	sessionId : 15;
					USHORT	external : 1;
				};
				UINT32	unique;
			};

			SessionId( )
				:unique(0)
			{ }
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
		void RequestResend( SendAct* act );
		void SetReserve( bool reserve );		
		bool GetReserve();
		
		UINT32 GetUniqueId( )
		{
			return m_sessionId.unique;
		}
		UINT32 GetSessionId( )
		{
			return m_sessionId.sessionId;
		}
		void Setseq( SHORT seq )
		{
			m_sessionId.seq = seq;
		}
		void SetSessionId( SHORT sessionId )
		{
			m_sessionId.sessionId = sessionId;
		}

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

		SessionId		m_sessionId;

	protected:
		static MemoryPool<PacketStream> ms_sendBufferPool;
		static MemoryPool<PacketStream> ms_recvBufferPool;
	};

}