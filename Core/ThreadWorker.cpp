#include "ThreadWorker.h"
#include "BaseEntity.h"
#include "EntityManager.h"
#include "EventQueue.h"
#include "NPC.h"
#include "Player.h"

namespace SSL
{
	ThreadWorker::ThreadWorker()
		:m_dispatcher(this)
	{		
		m_dispatcher.RegisterFunction( EN_EVENT::ADD_HP, &ThreadWorker::OnAddHP );
		m_dispatcher.RegisterFunction( EN_EVENT::ENTITY_UPDATE, &ThreadWorker::OnUpdateEntity );
	}


	ThreadWorker::~ThreadWorker()
	{
	}	

	UINT32 ThreadWorker::run()
	{
		while ( true )
		{			
			ProcessEventQueue();
		}

		return 0;
	}

	void ThreadWorker::ProcessEventQueue()
	{
		EVENTPtr ptr;
		while ( events.try_pop( ptr ) )
		{
			m_dispatcher.Dispatch( ptr );
		}
	}

	void ThreadWorker::OnAddHP( EVENTPtr& ptr )
	{
		const SSL::BaseEntity::BaseEntityPtr entityPtr = SSL::EntityManager::GetInstance()->GetEntity( ptr->entityIndex );
		if ( ptr->entityIndex < EN_ENTITY_ID_RANGE::ID_RANGE_PLAYER )
		{
			SSL::NPC *npc = static_cast<NPC*>( entityPtr.get() );
			ST_ADD_HP *stAddHP = static_cast< ST_ADD_HP* >( ptr.get() );
			npc->AddHP( stAddHP->addHP );			
		}
		else
		{
			SSL::Player *player = static_cast<Player*>( entityPtr.get() );
			ST_ADD_HP *stAddHP = static_cast< ST_ADD_HP* >( ptr.get() );
			player->AddHP( stAddHP->addHP );			
		}

	}

	void ThreadWorker::OnUpdateEntity( EVENTPtr& ptr )
	{
		const SSL::BaseEntity::BaseEntityPtr entityPtr = SSL::EntityManager::GetInstance()->GetEntity( ptr->entityIndex );
		if ( ptr->entityIndex < EN_ENTITY_ID_RANGE::ID_RANGE_PLAYER )
		{
			SSL::NPC *npc = static_cast<NPC*>( entityPtr.get() );
			npc->Update();			
		}
		else
		{
			SSL::Player *player = static_cast<Player*>( entityPtr.get() );
			player->Update();
		}
		
	}

}