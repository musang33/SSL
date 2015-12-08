#include "Connector.h"

#include "TcpSocket.h"
#include "Proactor.h"
#include "Act.h"

namespace SSL
{


	Connector::Connector( Proactor* proactor )
		: Actor( proactor )
	{
	}


	Connector::~Connector()
	{
	}


	void Connector::Complete( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;

		tcpSocket->SetState( TcpSocket::EN_SOCKET_STATE::SOCKET_ALIVE );
		tcpSocket->RequestRecv( );

		EventConnected* e = new EventConnected;
		e->socketIndex = tcpSocket->GetSessionId( );
		e->result = 0;

		m_proactor->PostEvent( EventPtr( e ) );
	}

	void Connector::Error( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;

		EventConnected* e = new EventConnected;
		e->socketIndex = tcpSocket->GetSessionId( );
		e->result = 1;

		m_proactor->PostEvent( EventPtr( e ) );
	}

}