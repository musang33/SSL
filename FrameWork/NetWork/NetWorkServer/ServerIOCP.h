#pragma once

#include <mutex>
#include <vector>
#include "Thread.h"


namespace SSL
{
	class Acceptor;
	class Sender;
	class Receiver;
	class Connector;
	class DisConnector;
	class TcpListenSocket;
	class TcpSocket;
	class Proactor;


	class ServerIOCP : public Thread
	{
	public:
		ServerIOCP( );
		~ServerIOCP( );

	public:
		bool Create( UINT32 numIoThread );
		void Destroy( );
		bool CreateListen( USHORT port, UINT32 backlog, UINT32 maxConnect, const INT32& type );
		TcpSocket* Connect( USHORT idx, const char* ipAddress, const SHORT& port );

	private:
		TcpSocket* getsockFromConnectPool( );

	private:
		Proactor*		m_proactor;
		Acceptor*		m_acceptor;
		Sender*			m_sender;
		Receiver*		m_receiver;
		Connector*		m_connector;

		std::vector<TcpListenSocket*>	m_listenSockets;
		std::vector<TcpSocket*>			m_clientPool;
		std::vector<TcpSocket*>			m_connectPool;

		BOOL			m_stop;
		std::mutex		g_i_mutex;
		SHORT			m_poolIndex;
	};

}