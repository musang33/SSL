#include "Acceptor.h"

#include "TcpSocket.h"
#include "Proactor.h"
#include <atomic>

namespace SSL
{
	
	std::atomic<USHORT> m_sessionSeed;

	Acceptor::Acceptor( Proactor* proactor )
		: Actor( proactor )
	{
		m_sessionSeed = 0;		
	}


	Acceptor::~Acceptor( )
	{
	}


	void Acceptor::Complete( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;
		tcpSocket->OnAccepted( );

		m_proactor->RegisterSocket( tcpSocket->GetSocket( ) );

		tcpSocket->Setseq( m_sessionSeed++ );
		tcpSocket->SetState( TcpSocket::EN_SOCKET_STATE::SOCKET_ALIVE );
		tcpSocket->RequestRecv( );

		EventAccept* e = new EventAccept;
		e->socketIndex = tcpSocket->GetUniqueId( );
		e->result = 0;
		e->type = tcpSocket->GetType( );
		m_proactor->PostEvent( EventPtr( e ) );
	}

	void Acceptor::Error( Act* act, unsigned int transferedBytes )
	{
		TcpSocket* tcpSocket = act->m_tcpSocket;

		tcpSocket->Disconnect( );

		tcpSocket->Create( );
		tcpSocket->RegisterAccept( );		
	}

}