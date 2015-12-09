#pragma once

#include <Network/NetworkCommon/Proactor.h>
#include <Network/NetworkCommon/Act.h>

namespace SSL
{
	class TcpListenSocket
	{
	public:
		TcpListenSocket( )
			: m_backlog(0), m_socket(INVALID_SOCKET )
		{ }

		virtual ~TcpListenSocket( )
		{
			if( m_socket != INVALID_SOCKET )
			{
				closesocket( m_socket );
			}
		}

		bool Init( USHORT port, UINT32 backlog = 0 )
		{
			m_socket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
			if( m_socket == INVALID_SOCKET )
			{
				return false;
			}

			m_addr.sin_family = AF_INET;
			m_addr.sin_addr.s_addr = htonl( INADDR_ANY );
			m_addr.sin_port = htons( port );

			m_backlog = backlog;

			int optval = 0;
			::setsockopt( m_socket, SOL_SOCKET, SO_SNDBUF, ( char* ) &optval, sizeof( optval ) );

			return true;
		}

		bool Listen( Proactor* proactor )
		{
			proactor->RegisterSocket( m_socket );
			if( bind( m_socket, ( SOCKADDR* ) &m_addr, sizeof( m_addr ) ) == SOCKET_ERROR )
			{
				return false;
			}

			if( listen( m_socket, m_backlog ) == SOCKET_ERROR )
			{
				return false;
			}

			return true;
		}

		SOCKET GetSocket( ) const
		{
			return m_socket;
		}

	private:
		SOCKET		m_socket;
		SOCKADDR_IN m_addr;
		UINT32		m_backlog;
		
			
	};
} // SSL