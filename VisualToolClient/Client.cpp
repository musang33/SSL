#include "stdafx.h"
#include "Client.h"

#include "..\FrameWork\Network\NetworkClient\ClientIOCP.h"
#include "..\FrameWork\Event\EventGame.h"

namespace SSL
{
	Client::Client( )
		:m_dispatcher(this )		
		, m_network( nullptr )
	{
		m_network = new ClientIOCP;
		if( false == m_network->Initialize( 1, 1 ) )
		{
			return;
		}
		
		m_network->SetRecvCallback( SSL::EventGame::eResAddPlayer, new EventCallBack<ResAddPlayer>( this, "ResAddPlayer" ) );
		m_network->SetRecvCallback( SSL::EventGame::eNtfMoveEntity, new EventCallBack<NtfMoveEntity>( this, "NtfMoveEntity" ) );

		m_dispatcher.RegisterFunction( SSL::EventGame::eResAddPlayer, Client::onResAddPlayer );
		m_dispatcher.RegisterFunction( SSL::EventGame::eNtfMoveEntity, Client::onNtfMoveEntity );

	}

	Client::~Client( )
	{
	}

	bool Client::Connect( const char* ip, short port )
	{
		return m_network->Connect( 0, ip, port );
	}

	bool Client::Send( EventPtr& ptr )
	{
		return m_network->Send( 0, ptr );
	}

	void Client::onResAddPlayer( EventPtr& ptr )
	{ 

	}

	void Client::onNtfMoveEntity( EventPtr& ptr )
	{
	}

}