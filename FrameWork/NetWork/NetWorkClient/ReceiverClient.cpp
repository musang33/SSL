#include <NetWorkClient/ReceiverClient.h>

#include <TcpSocket.h>
#include <Proactor.h>
#include <Act.h>

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
