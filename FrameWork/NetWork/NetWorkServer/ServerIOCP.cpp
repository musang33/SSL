#include "stdafx.h"

#include <Network/NetworkServer/ServerIOCP.h>

#include <Network/NetworkCommon/Proactor.h>
#include <Network/NetworkServer/Acceptor.h>
#include <Network/NetworkServer/Connector.h>
#include <Network/NetworkServer/Sender.h>
#include <Network/NetworkServer/Receiver.h>
#include <Network/NetworkCommon/Proactor.h>
#include <Network/NetworkCommon/TcpListenSocket.h>
#include <Network/NetworkCommon/TcpSocket.h>



namespace SSL
{

	ServerIOCP::ServerIOCP( )
	{
		m_proactor = new Proactor;
		m_acceptor = new Acceptor( m_proactor );
		m_connector = new Connector( m_proactor );
		m_sender = new Sender( m_proactor );
		m_receiver = new Receiver( m_proactor );
		m_stop = false;
		m_poolIndex = 0;
	}


	ServerIOCP::~ServerIOCP( )
	{
		for( auto &it : m_listenSockets )
		{
			delete it;
			it = nullptr;
		}
		m_listenSockets.clear( );

		for( auto &it : m_clientPool )
		{
			delete it;
			it = nullptr;
		}
		m_clientPool.clear( );

		for( auto &it : m_connectPool )
		{
			delete it;
			it = nullptr;
		}
		m_connectPool.clear( );

		delete m_acceptor;
		delete m_connector;
		delete m_sender;
		delete m_receiver;
		delete m_proactor;
	}

	bool ServerIOCP::Create( UINT32 numIoThread )
	{
		Start( );

		return m_proactor->StartIocp( numIoThread );
	}

	void ServerIOCP::Destroy( )
	{
		m_stop = true;
		m_proactor->Stop( );
		Stop( );
	}

	bool ServerIOCP::CreateListen( USHORT port, UINT32 backlog, UINT32 maxConnect, const INT32& type )
	{
		TcpListenSocket* listen = new TcpListenSocket;

		if( false == listen->Init( port, backlog ) )
		{
			delete listen;
			return false;
		}

		if( false == listen->Listen( m_proactor ) )
		{
			delete listen;
			return false;
		}

		std::lock_guard<std::mutex> lock( g_i_mutex );
		m_clientPool.reserve( maxConnect );

		for( UINT32 i = 0; i < maxConnect; i++ )
		{
			TcpSocket* tcp = new TcpSocket(listen->GetSocket());

			tcp->Create( );
			tcp->SetSessionId( m_poolIndex++ );
			tcp->SetType( type );
			tcp->SetServerMode( true );

			tcp->SetAct( TcpSocket::EN_ACT_TYPE::ACT_ACCEPT, new Act( ( Actor* ) m_acceptor ));
			tcp->SetAct( TcpSocket::EN_ACT_TYPE::ACT_SEND, new Act( ( Actor* ) m_sender ));
			tcp->SetAct( TcpSocket::EN_ACT_TYPE::ACT_RECV, new Act( ( Actor* ) m_receiver ));

			tcp->RegisterAccept( );

			tcp->SetReserve( true );
			tcp->SetReserve( false );

			m_clientPool.push_back( tcp );		
		}

		m_listenSockets.push_back( listen );

		return true;
	}

	TcpSocket* ServerIOCP::Connect( USHORT idx, const char* ipAddress, const SHORT& port )
	{
		struct sockaddr_in serv_addr;
		struct sockaddr_in local_addr;
		memset( &serv_addr, 0, sizeof( serv_addr ) );
		memset( &local_addr, 0, sizeof( local_addr ) );

		TcpSocket* tcp = getsockFromConnectPool( );

		if( tcp == nullptr )
		{
			return nullptr;
		}

		tcp->Create( );
		tcp->Setseq( idx );
		tcp->SetRemoteAddr( ipAddress, port );

		INT32		ret;
		DWORD		dwBytes = 0;
		LPFN_CONNECTEX	lpfnConnectEx = nullptr;
		GUID		guidConnectEx = WSAID_CONNECTEX;

		ret = ::WSAIoctl(
			tcp->GetSocket( ),
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidConnectEx,
			sizeof( guidConnectEx ),
			&lpfnConnectEx,
			sizeof( lpfnConnectEx ),
			&dwBytes,
			nullptr,
			nullptr
			);

		if( ret == SOCKET_ERROR )
		{
			tcp->SetReserve( true );
			return nullptr;
		}

		local_addr.sin_family = AF_INET;

		if( SOCKET_ERROR == ::bind( tcp->GetSocket( ), ( struct sockaddr * )&local_addr, sizeof( local_addr ) ) )
		{
			tcp->SetReserve( true );
			return nullptr;
		}

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr( ipAddress );
		serv_addr.sin_port = htons( port );

		m_proactor->RegisterSocket( tcp->GetSocket( ) );

		BOOL result = lpfnConnectEx( tcp->GetSocket( ),
									 ( struct sockaddr * )&serv_addr,
									 sizeof( struct sockaddr ),
									 nullptr,
									 0,
									 nullptr,
									 ( LPOVERLAPPED ) tcp->GetAct( TcpSocket::ACT_ACCEPT ) );

		if( FALSE == result && ( WSAGetLastError( ) != WSA_IO_PENDING ) )
		{
			tcp->SetReserve( true );
			return nullptr;
		}

		return tcp;
	}

	void ServerIOCP::SetRecvCallback( USHORT type, CallBack* e )
	{
		m_proactor->SetEvent( type, e );
	}

	UINT32 ServerIOCP::run( )
	{
		while( isActive( ) )
		{
			{
				std::lock_guard<std::mutex> lock( g_i_mutex );				

				for( auto iter : m_clientPool )
				{
					TcpSocket* socket = iter;

					if( socket->GetState( ) >= TcpSocket::EN_SOCKET_STATE::SOCKET_ALIVE )
					{
						socket->KickSend( );
					}					
				}
			}
		}

		return 0;
	}

	TcpSocket* ServerIOCP::getsockFromConnectPool( )
	{
		std::lock_guard<std::mutex> lock( g_i_mutex );

		for( auto &iter : m_connectPool )
		{
			TcpSocket* tcp = iter;
			
			if( tcp->IsReserved( ) )
			{
				tcp->SetReserved( false );

				return tcp;
			}
		}

		return nullptr;
	}

	bool ServerIOCP::Send( UINT32 index, EventPtr& e )
	{
		TcpSocket::SessionId id( index );		

		if( id.sessionId > m_poolIndex )
		{
			return false;
		}

		return m_clientPool[ id.sessionId ]->Send( e );
	}
}