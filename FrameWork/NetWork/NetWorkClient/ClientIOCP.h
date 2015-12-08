#pragma once

#include <mutex>
#include <vector>
#include <Thread.h>


namespace SSL
{	
	class SenderClient;
	class ReceiverClient;
	class ConnectorClient;
	class DisConnector;
	class TcpListenSocket;
	class TcpSocket;
	class Proactor;


	class ClientIOCP : public Thread
	{
	public:
		ClientIOCP( );
		~ClientIOCP( );

	public:
		bool Initialize( UINT32 poolSize, UINT32 threadCount );
		void Destroy( );		
		bool Connect( USHORT idx, const char* ipAddress, const SHORT& port );
		bool Disconnect( UINT32 idx );
		TcpSocket* GetsockFromConnectPool( UINT32 idx );
		INT32 GetReservedTcpSocket( ) const;
			
	private:
		Proactor*		m_proactor;		
		SenderClient*		m_sender;
		ReceiverClient*		m_receiver;
		ConnectorClient*	m_connector;
		
		std::vector<TcpSocket*>			m_tcpSockets;				
	};

}