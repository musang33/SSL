
#include "stdafx.h"

#include <Network/NetworkCommon/Proactor.h>
#include <Network/NetworkCommon/Act.h>
#include <Network/NetworkCommon/TcpSocket.h>

namespace SSL
{
	MemoryPool<PacketStream> TcpSocket::ms_sendBufferPool( 128 );
	MemoryPool<PacketStream> TcpSocket::ms_recvBufferPool( 128 );
	const static UINT32 READ_SIZE_ONCE = 1024 * 32;

	TcpSocket::TcpSocket( SOCKET listenSocket )
		: m_listenSocket( listenSocket)
		, m_socket( INVALID_SOCKET )
		, m_isSending( false )
		, m_type( 0 )		
		, m_isServerMode(false )
		, m_reserve( false )
	{
	}


	TcpSocket::~TcpSocket()
	{
	}

	void TcpSocket::Create()
	{
		if ( INVALID_SOCKET != m_socket )
		{
			closesocket( m_socket );
			shutdown( m_socket, SD_BOTH );
			m_socket = INVALID_SOCKET;
		}

		m_socket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );		
		m_sendBuf[ 0 ] = new SendAct;
		m_sendBuf[ 1 ] = new SendAct;

		reset( );
	}

	void TcpSocket::Disconnect()
	{
		if ( m_socket != INVALID_SOCKET )
		{
			closesocket( m_socket );
			shutdown( m_socket, SD_BOTH );
		}

		reset( );
	}

	bool TcpSocket::RequestRecv()
	{
		m_recvActBuf.ResetBufferSize( READ_SIZE_ONCE );

		m_recvWsaBuf.len = READ_SIZE_ONCE;
		m_recvWsaBuf.buf = m_recvActBuf.GetDataPtr();

		DWORD flags = 0;
		DWORD size = m_recvWsaBuf.len;
		int result = WSARecv(
			m_socket,
			&m_recvWsaBuf,
			1,
			&size,
			&flags,
			m_act[ACT_RECV],
			0 );

		if ( result == SOCKET_ERROR )
		{
			INT error = WSAGetLastError();
			if ( error != ERROR_IO_PENDING )
			{
				Disconnect();
				return false;
			}
		}

		return true;
	}

	bool TcpSocket::Send( const EventPtr& e )
	{
		PacketStream* psp = ms_sendBufferPool.Acquire();
		if ( psp == nullptr )
		{
			return false;
		}

		psp->Reset();

		MemoryPoolPtr<PacketStream> psPtr( &ms_sendBufferPool, psp );

		PacketStream& ps = *psPtr;

		e->Pack( ps );

		{
			std::lock_guard<std::mutex> lock( g_i_mutex );

			m_curSendAct->buffer.Write( ps.Size() );
			m_curSendAct->buffer.Write( ps.GetDataPtr(), ps.Size() );
		}

		if( m_isServerMode )
		{
			return true;
		}

		return requestSend();

	}

	INT32 TcpSocket::OnRecvCompleted( Act* act, Proactor* proactor, UINT32 len )
	{
		if ( len == 0 )
		{
			Disconnect();
			return -1;
		}

		m_recvBuf.Write( m_recvActBuf.GetPtr(), len );

		packetize( proactor );
		bool result = RequestRecv();
		if ( false == RequestRecv() )
		{
			return -1;
		}

		return len;
	}

	void TcpSocket::OnSendCompleted( SendAct* act )
	{
		act->buffer.Reset();

		{
			std::lock_guard<std::mutex> lock( g_i_mutex );

			m_isSending = false;
		}

		requestSend();
	}

	void TcpSocket::OnAccepted( )
	{
		int localAddrLen, remoteAddrLen;
		struct sockaddr_in* localAddr;
		struct sockaddr_in* remoteAddr;

		GetAcceptExSockaddrs( m_recvBuf.GetPtr( ), 0, sizeof( SOCKADDR_IN ) + 16, sizeof( SOCKADDR_IN ) + 16,
							  ( sockaddr** ) &localAddr,
							  &localAddrLen,
							  ( sockaddr** ) &remoteAddr,
							  &remoteAddrLen );			

	}

	void TcpSocket::RegisterAccept( )
	{
		DWORD byteTransferred = 0;
		BOOL result = AcceptEx(
			m_listenSocket,
			m_socket,
			m_recvBuf.GetPtr( ),
			0,
			sizeof( SOCKADDR_IN ) + 16,
			sizeof( SOCKADDR_IN ) + 16,
			&byteTransferred,
			reinterpret_cast< LPOVERLAPPED >( GetAct( TcpSocket::ACT_ACCEPT ) )
			);

		int error = WSAGetLastError( );
		if( result == FALSE && error != ERROR_IO_PENDING )
		{
			return;
		}

		SetState( SOCKET_ACCEPTWAIT );
	}

	void TcpSocket::RequestResend( SendAct* sendAct )
	{
		UINT32 length = sendAct->wsabuf.len - sendAct->bytesRequested;

		sendAct->m_actor = m_act[TcpSocket::ACT_SEND]->m_actor;
		sendAct->m_tcpSocket = this;

		sendAct->wsabuf.len = length;
		sendAct->wsabuf.buf = reinterpret_cast< char* >( sendAct->buffer.GetPtr() );

		sendAct->bytesRequested = length;
		sendAct->sentTransferred = 0;

		INT32 result = WSASend(
			m_socket,
			&( sendAct->wsabuf ),
			1,
			( LPDWORD ) &length,
			0,
			sendAct,
			0
			);

		if ( result == SOCKET_ERROR )
		{
			DWORD err = GetLastError();
			if ( err != WSA_IO_PENDING )
			{
				if ( err != ERROR_NETNAME_DELETED )
				{
					Disconnect();
				}

				return;
			}
		}
	}

	void TcpSocket::SetReserve( bool reserve )
	{
		m_reserve = reserve;
	}

	bool TcpSocket::GetReserve()
	{
		return m_reserve;
	}

	void TcpSocket::packetize( Proactor* proactor )
	{
		static const UINT32 MAX_PACKET_SIZE = 255 * 1024;

		BYTE* data = m_recvBuf.GetPtr();
		UINT32 dataSize = m_recvBuf.Size();
		UINT32 processedLength = 0;
		UINT32 packetCount = 0;


		while ( 4 <= dataSize )
		{
			INT32* packetSizePtr = reinterpret_cast< INT32* >( data );
			UINT32 packetLength = *packetSizePtr;

			if ( packetLength > MAX_PACKET_SIZE )
			{
				Disconnect();
				if ( processedLength > 0 )
				{
					m_recvBuf.Pop( processedLength );
				}
				return;
			}

			if ( packetLength + 4 > dataSize )
			{
				if ( processedLength > 0 )
				{
					m_recvBuf.Pop( processedLength );
				}
				return;
			}

			UINT32 length = packetLength;
			void* packet = ( void* ) ( data + 4 );

			if ( false == proactor->PostEvent( this, packet, static_cast< INT32 >( length ) ) )
			{
				Disconnect();
				if ( processedLength > 0 )
				{
					m_recvBuf.Pop( processedLength );
				}
				return;
			}

			++packetCount;

			processedLength += ( packetLength + 4 );

			data = data + ( packetLength + 4 );
			dataSize = dataSize - ( packetLength + 4 );
		}
	}

	bool TcpSocket::requestSend( )
	{
		SendAct* sendAct = nullptr;

		{
			std::lock_guard<std::mutex> lock( g_i_mutex );

			if ( m_isSending || m_curSendAct->buffer.Size() == 0 )
			{
				return true;
			}

			sendAct = m_curSendAct;

			if ( sendAct == m_sendBuf[0] )
			{
				m_curSendAct = m_sendBuf[1];
			}
			else
			{
				m_curSendAct = m_sendBuf[0];
			}

			m_isSending = true;
			m_curSendAct->buffer.Reset();
		}

		return purgeSend( sendAct );
	}

	bool TcpSocket::purgeSend( SendAct* sendAct )
	{	
		UINT32 length = static_cast< UINT32 >( sendAct->buffer.Size() );

		sendAct->m_actor = m_act[TcpSocket::ACT_SEND]->m_actor;
		sendAct->m_tcpSocket = this;

		sendAct->wsabuf.len = length;
		sendAct->wsabuf.buf = reinterpret_cast<char*>(sendAct->buffer.GetPtr());

		sendAct->bytesRequested = length;
		sendAct->sentTransferred = 0;

		INT32 result = WSASend(
			m_socket,
			&( sendAct->wsabuf ),
			1,
			( LPDWORD ) &length,
			0,
			sendAct,
			0
			);

		if ( result == SOCKET_ERROR )
		{
			DWORD err = GetLastError();
			if ( err != WSA_IO_PENDING )
			{
				if ( err != ERROR_NETNAME_DELETED )
				{
					Disconnect();
				}

				return false;
			}
		}

		return true;
	}

	void TcpSocket::reset( )
	{
		std::lock_guard<std::mutex> lock( g_i_mutex );

		m_recvActBuf.Reset( );
		m_recvBuf.Reset( );

		m_sendBuf[ 0 ]->buffer.ResetBufferSize( 0 );
		m_sendBuf[ 0 ]->buffer.ResetIncreaseSize( 0 );

		m_sendBuf[ 1 ]->buffer.ResetBufferSize( 0 );
		m_sendBuf[ 1 ]->buffer.ResetIncreaseSize( 0 );

		m_curSendAct = m_sendBuf[ 0 ];

		m_isSending = false;
		m_reserve = false;
	}
	
}