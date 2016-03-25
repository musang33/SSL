#include "stdafx.h"

#include <Network/NetWorkClient/ClientIOCP.h>

#include <Network/NetworkClient/ConnectorClient.h>
#include <Network/NetworkClient/SenderClient.h>
#include <Network/NetworkClient/ReceiverClient.h>
#include <Network/NetworkCommon/Proactor.h>
#include <Network/NetworkCommon/TcpListenSocket.h>
#include <Network/NetworkCommon/TcpSocket.h>



namespace SSL
{

	ClientIOCP::ClientIOCP( )
	{
		m_proactor = new Proactor;		
		m_connector = new ConnectorClient( m_proactor );
		m_sender = new SenderClient( m_proactor );
		m_receiver = new ReceiverClient( m_proactor );		
	}


	ClientIOCP::~ClientIOCP( )
	{
		delete m_connector;
		delete m_sender;
		delete m_receiver;
		delete m_proactor;
	}

	bool ClientIOCP::Initialize( UINT32 poolSize, UINT32 threadCount )
	{
		for( UINT32 i = 0; i < poolSize; i++ )
		{
			TcpSocket* tcp = new TcpSocket;

			tcp->Create( );
			tcp->Setseq( i );
			tcp->SetSessionId( 0 );
			tcp->SetType( 0 );

			tcp->SetAct( TcpSocket::ACT_CONNECT, new Act( ( Actor* ) m_connector ) );
			tcp->SetAct( TcpSocket::ACT_RECV, new Act( ( Actor* ) m_receiver ) );
			tcp->SetAct( TcpSocket::ACT_SEND, new Act( ( Actor* ) m_sender ) );

			tcp->SetReserve( true );

			m_tcpSockets.push_back( tcp );
		}

		return m_proactor->StartIocp( threadCount );
	}

	void ClientIOCP::Destroy( )
	{		
		m_proactor->Stop( );
		Stop( );
	}

	bool ClientIOCP::Connect( USHORT idx, const char* ipAddress, const SHORT& port )
	{
		if( idx > m_tcpSockets.size( ) )
		{
			return false;
		}

		TcpSocket* tcp = m_tcpSockets[idx];

		if( tcp == nullptr )
		{
			return false;
		}

		tcp->Create( );
				
		
		DWORD		dwBytes = 0;
		LPFN_CONNECTEX	lpfnConnectEx = nullptr;
		GUID		guidConnectEx = WSAID_CONNECTEX;

		INT32 ret = ::WSAIoctl(
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
			return false;
		}

		struct sockaddr_in local_addr;
		memset( &local_addr, 0, sizeof( local_addr ) );
		local_addr.sin_family = AF_INET;

		if( SOCKET_ERROR == ::bind( tcp->GetSocket( ), ( struct sockaddr * )&local_addr, sizeof( local_addr ) ) )
		{
			tcp->SetReserve( true );
			return false;
		}

		struct sockaddr_in serv_addr;
		memset( &serv_addr, 0, sizeof( serv_addr ) );

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
									 ( LPOVERLAPPED ) tcp->GetAct( TcpSocket::ACT_CONNECT ) );

		if( FALSE == result && ( WSAGetLastError( ) != WSA_IO_PENDING ) )
		{
			DWORD lastError = WSAGetLastError( );
			tcp->SetReserve( true );
			return false;
		}

		return true;
	}

	bool ClientIOCP::Disconnect( UINT32 idx )
	{
		if( idx > m_tcpSockets.size( ) )
		{
			return false;
		}

		m_tcpSockets[ idx ]->Disconnect( );

		return true;
	}

	TcpSocket* ClientIOCP::GetsockFromConnectPool( UINT32 idx )
	{
		if( idx > m_tcpSockets.size( ) )
		{
			return nullptr;
		}

		return m_tcpSockets[idx];
	}

	INT32 ClientIOCP::GetReservedTcpSocket( ) const
	{
		INT32 idx = 0;

		for( auto it : m_tcpSockets )
		{
			if( it->IsReserved( ) )
			{
				return idx;
			}
			idx++;
		}

		return -1;
	}

	bool ClientIOCP::Send( UINT32 index, EventPtr& e )
	{
		TcpSocket::SessionId id( index );

		if( id.sessionId > m_tcpSockets.size() )
		{
			return false;
		}

		return m_tcpSockets[ id.sessionId ]->Send( e );
	}

	void ClientIOCP::SetRecvCallback( UINT16 type, CallBack* e )
	{
		m_proactor->SetEvent( type, e );
	}
}