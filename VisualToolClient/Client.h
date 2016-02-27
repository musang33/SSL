#pragma once

#include "..\FrameWork\Core\\Dispatcher.h"
#include "..\FrameWork\Core\Singleton.h"

#include "..\FrameWork\Event\EventCallBack.h"

namespace SSL
{	
	class ClientIOCP;
	class Client : public EventListener, public Singleton< Client >
	{
	public:
		Client( );
		~Client( );

	public:
		bool Connect( const char* ip, short port );
		bool Send( EventPtr& ptr );

	public:
		ClientIOCP				*m_network;
		Dispatcher<Client>		m_dispatcher;		

	private:
		void onResAddPlayer( EventPtr& ptr );
		void onNtfMoveEntity( EventPtr& ptr );
	};

}