#include <Thread/ThreadWorker.h>
#include <Entity/Entity.h>
#include "EntityManager.h"
#include <Event/EventQueue.h>

#include "ActionAI.h"
#include "ActionState.h"
#include "ActionPlayerFight.h"

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
		const SSL::Entity* entityPtr = SSL::EntityManager::GetInstance()->GetEntity( ptr->entityIndex );
			
		ST_ADD_HP *stAddHP = static_cast< ST_ADD_HP* >( ptr.get() );
		ActionState* as = GetEntityAction( entityPtr );
		as->AddHP( stAddHP->addHP );
	}

	void ThreadWorker::OnUpdateEntity( EVENTPtr& ptr )
	{
		const SSL::Entity* entityPtr = SSL::EntityManager::GetInstance()->GetEntity( ptr->entityIndex );
				
		if( entityPtr->Type() == SSL::EN_ENTITY_TYPE::ENTITY_NPC_TYPE )
		{
			ActionAI* aa = GetEntityAction( entityPtr );
			aa->Update( );
		}		
	}
}