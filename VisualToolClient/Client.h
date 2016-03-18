#pragma once

#include "..\FrameWork\Core\\Dispatcher.h"
#include "..\FrameWork\Core\Singleton.h"

#include "..\FrameWork\Event\EventCallBack.h"

namespace SSL
{	
	class Player;
	class ClientIOCP;
	class Client : public EventListener, public Singleton< Client >
	{
	public:
		Client( );
		~Client( );

	public:
		bool Connect( const char* ip, short port );
		bool Send( EventPtr& ptr );
		Player* GetPlayerEntity( )
		{
			return m_cliententity;
		}

	public:
		ClientIOCP				*m_network;
		Dispatcher<Client>		m_dispatcher;		

	private:
		void onResAddPlayer( EventPtr& ptr );
		void onNtfAddNpc( EventPtr& ptr );
		void onNtfRemoveNpc( EventPtr& ptr );
		void onNtfMoveEntity( EventPtr& ptr );

	private:
		Player* m_cliententity;
	};

}