#include "ThreadWorker.h"
#include "BaseEntity.h"
#include "EntityManager.h"
#include "EventQueue.h"

namespace SSL
{
	ThreadWorker::ThreadWorker()
		:m_dispatcher(this)
	{		
		m_dispatcher.RegisterFunction( EN_EVENT::ADD_HP, &ThreadWorker::OnAddHP );
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
		while ( false == events.empty() )
		{
			EVENTPtr ptr;
			events.try_pop( ptr );
			

			m_dispatcher.Dispatch( ptr );
		}
	}

	void ThreadWorker::OnAddHP( EVENTPtr& ptr )
	{

	}

}