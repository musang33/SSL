
#include "stdafx.h"

#include <../Framework/Entity/Entity.h>
#include <../Framework/Event/EventQueue.h>

#include <Action/ActionAI.h>
#include <Action/ActionState.h>
#include <Action/ActionPlayerFight.h>

#include <Manager/EntityManager.h>
#include <Thread/ThreadWorker.h>

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
		EventPtr ptr;
		while ( events.try_pop( ptr ) )
		{
			m_dispatcher.Dispatch( ptr );
		}
	}

	void ThreadWorker::OnAddHP( EventPtr& /*ptr*/ )
	{
		/*const SSL::Entity* entityPtr = SSL::EntityManager::GetInstance()->GetEntity( ptr->entityIndex );
			
		ST_ADD_HP *stAddHP = static_cast< ST_ADD_HP* >( ptr.get() );
		ActionState* as = GetEntityAction( entityPtr );
		as->AddHP( stAddHP->addHP );*/
	}

	void ThreadWorker::OnUpdateEntity( EventPtr& ptr )
	{
		const SSL::Entity* entityPtr = SSL::EntityManager::GetInstance()->GetEntity( ptr->entityIndex );
				
		if( entityPtr->Type() == SSL::EN_ENTITY_TYPE::ENTITY_NPC_TYPE )
		{
			ActionAI* aa = GetEntityAction( entityPtr );
			aa->Update( );
		}		
	}
}