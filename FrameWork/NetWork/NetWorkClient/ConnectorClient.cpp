#include <NetworkClient/ConnectorClient.h>

#include <TcpSocket.h>
#include <Proactor.h>
#include <Act.h>

namespace SSL
{


	ConnectorClient::ConnectorClient( Proactor* proactor )
		: Actor( proactor )
	{
	}


	ConnectorClient::~ConnectorClient()
	{
	}


	void ConnectorClient::Complete( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;

		tcpSocket->SetState( TcpSocket::EN_SOCKET_STATE::SOCKET_ALIVE );
		tcpSocket->RequestRecv( );

		EventRawConnected* e = new EventRawConnected;
		e->socketIndex = tcpSocket->GetSessionId( );
		e->result = 0;

		m_proactor->PostEvent( EventPtr( e ) );
	}

	void ConnectorClient::Error( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;

		EventConnected* e = new EventConnected;
		e->socketIndex = tcpSocket->GetSessionId( );
		e->result = 1;

		m_proactor->PostEvent( EventPtr( e ) );
	}

}