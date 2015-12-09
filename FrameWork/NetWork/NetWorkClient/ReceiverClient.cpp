#include "stdafx.h"

#include <Network/NetWorkClient/ReceiverClient.h>

#include <Network/NetworkCommon/TcpSocket.h>
#include <Network/NetworkCommon/Proactor.h>
#include <Network/NetworkCommon/Act.h>

namespace SSL
{

	ReceiverClient::ReceiverClient( Proactor* proactor )
		: Actor( proactor )
	{
	}


	ReceiverClient::~ReceiverClient()
	{
	}

	void ReceiverClient::Complete( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;
		
		int result = tcpSocket->OnRecvCompleted( act, m_proactor, transferedBytes );
		if ( result < 0 )
		{
			tcpSocket->Disconnect();
		}		
	}

	void ReceiverClient::Error( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;
		
		tcpSocket->Disconnect();

		EventDisconnected* e = new EventDisconnected;
		e->socketIndex = tcpSocket->GetSessionId( );

		m_proactor->PostEvent( EventPtr( e ) );
	}

}
