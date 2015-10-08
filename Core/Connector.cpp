#include "Connector.h"

#include "TcpSocket.h"

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

		int result = tcpSocket->OnRecvCompleted( act, m_proactor, transferedBytes );
		if ( result < 0 )
		{
			tcpSocket->Disconnect();
		}
	}

	void Connector::Error( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;

		tcpSocket->Disconnect();
	}

}