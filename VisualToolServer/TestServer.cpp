#include "stdafx.h"
#include "TestServer.h"

#include <..\FrameWork\Network\NetworkServer\ServerIOCP.h>

#include <Action\ActionMove.h>
#include <Action\ActionPlayer.h>

#include <Server\Factory\FactoryNpcImpl.h>
#include <Server\Factory\FactoryPlayerImpl.h>

#include <Manager\LuaManager.h>
#include <Manager\EntityManager.h>


#pragma comment( lib, "ws2_32.lib")

namespace SSL
{

	TestServer::TestServer( )
		:m_dispatcher( this )
	{
		m_network = new ServerIOCP;
		m_network->SetRecvCallback( Event::eEventAccept, new EventCallBack<EventAccept>( this, "EventAccept" ) );

		m_network->SetRecvCallback( EventGame::eReqAddPlayer, new EventCallBack<ReqAddPlayer>( this, "ReqAddPlayer" ) );		
		m_network->SetRecvCallback( EventGame::eReqMoveEntity, new EventCallBack<ReqMoveEntity>( this, "ReqMoveEntity" ) );		

		m_dispatcher.RegisterFunction( Event::eEventAccept, &TestServer::onAccept );
		m_dispatcher.RegisterFunction( EventGame::eReqAddPlayer, &TestServer::onReqAddPlayer );		
		m_dispatcher.RegisterFunction( EventGame::eReqMoveEntity, &TestServer::onReqMoveEntity );
		
	}


	TestServer::~TestServer( )
	{
	}


	bool TestServer::Initialize( )
	{
		WSADATA wsaData;

		if( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
		{
			return false;
		}

		SYSTEM_INFO info;
		GetSystemInfo( &info );

		m_network->Create( info.dwNumberOfProcessors );

		// Factory 초기화
		SSL::EntityFactory::GetInstance()->AddFactoryImpl( new FactoryNpcImpl );
		SSL::EntityFactory::GetInstance( )->AddFactoryImpl( new FactoryPlayerImpl );

		// NPC 배치
		SSL::LuaManager::GetInstance( )->init( "..\\.\\lua_script\\script_list.lua" );

		for( int i = 0; i < 10; i++ )
		{
			SSL::Entity* npc = SSL::EntityFactory::GetInstance( )->CreateEntity( SSL::EN_ENTITY_TYPE::ENTITY_NPC_TYPE );
			SSL::EntityManager::GetInstance( )->RegisterEntity( npc );			
		}		

		return true;
	}

	bool TestServer::CreateListen( USHORT port, UINT32 backlog, UINT32 maxConnect, const INT32& type )
	{
		return m_network->CreateListen( port, backlog, maxConnect, type );
	}

	void TestServer::Update( )
	{
		if( m_queue.IsEmpty( ) )
		{
			Sleep( 10 );			
		}
		else
		{
			EventPtr ep;
			while( m_queue.Pop( ep ) )
			{
				m_dispatcher.Dispatch( ep );
			}
		}		

		SSL::EntityManager::GetInstance( )->UpdateEntities( );

		

		// 패킷 처리 이후 주기적으로 해 주어야 할 것들을 한다. 
	}

	void TestServer::Send( Entity* entity, EventPtr& ptr )
	{
		SSL::ActionPlayer *ap = GetEntityAction( entity );
		if( ap != nullptr )
		{
			m_network->Send( ap->GetSocketIndex( ), ptr );
		}		
	}

	void TestServer::SendAll( EventPtr& ptr )
	{
		const EntityManager::ENTITY_MAP& entitymap = SSL::EntityManager::GetInstance( )->GetEntityMap( );

		for( auto it : entitymap )
		{
			SSL::ActionPlayer *ap = GetEntityAction( it.second );
			if( ap != nullptr )
			{
				m_network->Send( ap->GetSocketIndex( ), ptr );
			}			
		}		
	}
		
	void TestServer::onAccept( EventPtr& ep )
	{
		
	}

	void TestServer::onReqAddPlayer( EventPtr& ep )
	{
		ReqAddPlayer *req = static_cast< ReqAddPlayer* >( ep.get( ) );

		SSL::Entity* player = SSL::EntityFactory::GetInstance( )->CreateEntity( SSL::EN_ENTITY_TYPE::ENTITY_PLAYER_TYPE );
		SSL::EntityManager::GetInstance( )->RegisterEntity( player );

		SSL::ActionPlayer *ap = GetEntityAction( player );
		ap->SetSocketIndex( req->GetSocketIndex( ) );

		ResAddPlayer* res = new ResAddPlayer;
		res->entityId = player->ID( );

		Send( player, EventPtr( res ) );		
	}

	void TestServer::onReqMoveEntity( EventPtr& ep )
	{
		ReqMoveEntity *req = static_cast< ReqMoveEntity* >( ep.get( ) );
		SSL::Entity* player = SSL::EntityManager::GetInstance( )->GetEntity( req->entityId );
		if( player->Type( ) != SSL::EN_ENTITY_TYPE::ENTITY_PLAYER_TYPE )
		{
			return;
		}

		SSL::ActionMove *am = GetEntityAction(player);
		am->Move( req->x, req->y );
	}

	
}