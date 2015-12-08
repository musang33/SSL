#include "Sender.h"

#include "TcpSocket.h"
#include "Act.h"

namespace SSL
{

	Sender::Sender( Proactor* proactor)
		: Actor(proactor)
	{
	}


	Sender::~Sender()
	{
	}

	void Sender::Complete( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;
		SendAct* sendAct = static_cast< SendAct* >( act );

		if ( transferedBytes == 0 )
		{
			tcpSocket->Disconnect();
			tcpSocket->OnSendCompleted( sendAct );

			return;
		}

		sendAct->sentTransferred += transferedBytes;
		if ( sendAct->sentTransferred >= sendAct->bytesRequested )
		{
			tcpSocket->OnSendCompleted( sendAct );
		}
		else
		{
			tcpSocket->RequestResend( sendAct );
		}
	}
	void Sender::Error( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;
		tcpSocket->Disconnect();

		SendAct* sendAct = static_cast< SendAct* >( act );
		tcpSocket->OnSendCompleted(sendAct);
	}

}