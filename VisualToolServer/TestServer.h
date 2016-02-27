#pragma once


#include <..\FrameWork\Event\EventCallBack.h>
#include <..\FrameWork\Event\Event.h>

#include <..\FrameWork\Core\\Dispatcher.h>
#include <..\FrameWork\Core\Singleton.h>


namespace SSL
{
	class ServerIOCP;
	class TestServer : public EventListener, public Singleton< TestServer >
	{
	public:
		TestServer( );
		~TestServer( );

	public:
		bool Initialize( );
		bool CreateListen( USHORT port, UINT32 backlog, UINT32 maxConnect, const INT32& type );
		void Update( );
		void Send( Entity* entity, EventPtr& ptr );
		void SendAll( EventPtr& ptr );

	private:
		ServerIOCP*				m_network;
		Dispatcher<TestServer>	m_dispatcher;

	private:		
		void onAccept( EventPtr& evt );
		void onReqAddPlayer( EventPtr& evt );
		void onReqMoveEntity( EventPtr& evt );
	};

}