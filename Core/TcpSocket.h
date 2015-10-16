#pragma once
#include "Act.h"
#include "CommonData.h"
#include "PacketStream.h"
#include "MemoryPool.h"
#include "Event.h"
#include <atomic>

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
		enum EN_SOCKET_STATE
		{
			SOCKET_ACCEPTWAIT = 0,
			SOCKET_DEAD,
			SOCKET_ALIVE,
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
		void OnAccepted( );
		void RegisterAccept( );
		void RequestResend( SendAct* act );
		void SetReserve( bool reserve );		
		bool GetReserve();
		SOCKET GetSocket( )
		{
			return m_socket;
		}

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
		void SetState( INT32 state )
		{
			m_state = state;
		}

		void SetType( INT32 type )
		{
			m_type = type;
		}
		INT32 GetType( )
		{
			return m_type;
		}

		Act* GetAct( EN_ACT_TYPE type )
		{
			return m_act[ type ];
		}

	private:
		bool requestSend();
		void packetize( Proactor* proactor );
		bool purgeSend( SendAct* sendAct );

	private:		
		SOCKET			m_listenSocket;
		SOCKET			m_socket;
		sockaddr_in		m_socketAddr;
		Act*			m_act[ACT_TYPE_COUNT];
		WSABUF			m_recvWsaBuf;
		PacketStream	m_recvActBuf;		// WSARecv에 사용 되는 버퍼 ( complete이 되면 m_recvBuf에 카피한다. )
		PacketStream	m_recvBuf;			// 실제로 처리되는 데이터가 담긴 버퍼 ( receive, accept 등 )

		SendAct			m_sendBuf[2];
		SendAct*		m_curSendAct;
		bool			m_isSending;

		

		std::mutex		g_i_mutex;
		bool			m_reserve;

		SessionId		m_sessionId;

		std::atomic<INT32> m_state;
		INT32			m_type;

	protected:
		static MemoryPool<PacketStream> ms_sendBufferPool;
		static MemoryPool<PacketStream> ms_recvBufferPool;
	};

}