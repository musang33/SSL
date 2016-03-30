#include "stdafx.h"
#include "Client.h"
#include "Npc.h"
#include "Player.h"

#include <Network\NetworkClient\ClientIOCP.h>
#include <Event\EventGame.h>

#include <..\VisualToolServer\Server\Manager\EntityManager.h>

namespace SSL
{
	Client::Client( )
		:m_dispatcher(this )		
		, m_network( nullptr )
	{
		WORD wVersionRequesed = MAKEWORD( 2, 2 );
		WSADATA wsaData;
		int nErrorStatus = WSAStartup( wVersionRequesed, &wsaData );
		if( nErrorStatus != 0 )
		{
			return;
		}

		m_network = new ClientIOCP;
		if( false == m_network->Initialize( 1, 1 ) )
		{
			return;
		}
		
		m_network->SetRecvCallback( SSL::EventGame::eResAddPlayer, new EventCallBack<ResAddPlayer>( this, "ResAddPlayer" ) );
		m_network->SetRecvCallback( SSL::EventGame::eNtfAddNpc, new EventCallBack<NtfAddNpc>( this, "NtfAddNpc" ) );
		m_network->SetRecvCallback( SSL::EventGame::eNtfRemoveNpc, new EventCallBack<NtfRemoveNpc>( this, "NtfRemoveNpc" ) );
		m_network->SetRecvCallback( SSL::EventGame::eNtfMoveEntity, new EventCallBack<NtfMoveEntity>( this, "NtfMoveEntity" ) );

		m_dispatcher.RegisterFunction( SSL::EventGame::eResAddPlayer, &Client::onResAddPlayer );
		m_dispatcher.RegisterFunction( SSL::EventGame::eNtfAddNpc, &Client::onNtfAddNpc );
		m_dispatcher.RegisterFunction( SSL::EventGame::eNtfRemoveNpc, &Client::onNtfRemoveNpc );
		m_dispatcher.RegisterFunction( SSL::EventGame::eNtfMoveEntity, &Client::onNtfMoveEntity );
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

	void Client::Update( )
	{
		while( 1 )
		{
			if( false == m_queue.IsEmpty( ) )
			{
				EventPtr ptr;
				while( m_queue.Pop( ptr ) )
				{
					m_dispatcher.Dispatch( ptr );
				}				
			}

			Sleep( 10 );
		}		
	}

	void Client::onResAddPlayer( EventPtr& ptr )
	{ 
		ResAddPlayer* ntf = static_cast< ResAddPlayer* >( ptr.get( ) );
		m_cliententity = new Player( ntf->entityId );

		SSL::EntityManager::GetInstance( )->RegisterEntity( m_cliententity );
	}

	void Client::onNtfAddNpc( EventPtr& ptr )
	{
		NtfAddNpc* ntf = static_cast< NtfAddNpc* >( ptr.get( ) );
		Entity* npc = new Npc( ntf->entityId );

		SSL::EntityManager::GetInstance( )->RegisterEntity( npc );
	}

	void Client::onNtfRemoveNpc( EventPtr& ptr )
	{		
	}

	void Client::onNtfMoveEntity( EventPtr& ptr )
	{
		NtfMoveEntity* ntf = static_cast< NtfMoveEntity* >( ptr.get( ) );

		Entity* entity = SSL::EntityManager::GetInstance( )->GetEntity( ntf->entityId );
		if( nullptr == entity )
		{
			NtfAddNpc* ntf = static_cast< NtfAddNpc* >( ptr.get( ) );
			entity = new Npc( ntf->entityId );

			SSL::EntityManager::GetInstance( )->RegisterEntity( entity );
		}

		if( entity->Type() == SSL::EN_ENTITY_TYPE::ENTITY_NPC_TYPE )
		{
			Npc* npc = static_cast< Npc* >( entity );
			npc->SetLocation( ntf->x, ntf->y );
		}
		else
		{
			Player* player = static_cast< Player* >( entity );
			player->SetLocation( ntf->x, ntf->y );
		}

	}

}