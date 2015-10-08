#include "Receiver.h"

#include "TcpSocket.h"

namespace SSL
{

	Receiver::Receiver( Proactor* proactor )
		: Actor( proactor )
	{
	}


	Receiver::~Receiver()
	{
	}

	void Receiver::Complete( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;
		
		int result = tcpSocket->OnRecvCompleted( act, m_proactor, transferedBytes );
		if ( result < 0 )
		{
			tcpSocket->Disconnect();
		}		
	}

	void Receiver::Error( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;
		
		tcpSocket->Disconnect();
	}

}
