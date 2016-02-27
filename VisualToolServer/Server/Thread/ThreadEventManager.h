#pragma once

#include <atomic>
#include <functional>
#include <concurrent_queue.h>
#include <map>

#include <../Framework/Core/Singleton.h>
#include <../Framework/Event/EventQueue.h>
#include <../Framework/Common/CommonData.h>

#include <Manager/EntityManager.h>
#include <Thread/ThreadWorker.h>


namespace SSL
{	
	class ThreadEventManager : public Singleton<ThreadEventManager>, public EventQueue
	{
	private:		
		std::map<UINT32, ThreadWorker*> m_threadWorker; // < threadId, thread >
		std::vector<ThreadWorker*> m_threadWorkerVector;

	public:
		ThreadEventManager();
		~ThreadEventManager();

	public:
		void ProcessEventQueue() override 
		{
			static INT32 threadOrderIndex = 0;
			EventPtr ptr = nullptr;
			while ( events.try_pop( ptr ) )
			{						
				Entity* baseEntityPtr = EntityManager::GetInstance()->GetEntity( ptr->entityIndex );
				if ( baseEntityPtr->currentProcessingThreadID )
				{
					auto it = m_threadWorker.find( baseEntityPtr->currentProcessingThreadID );
					if ( it != m_threadWorker.end() )
					{
						it->second->PushEvent( ptr );
						continue;
					}
				}

				baseEntityPtr->currentProcessingThreadID = threadOrderIndex;
				m_threadWorkerVector[threadOrderIndex++]->PushEvent( ptr );				
				if ( threadOrderIndex >= static_cast<INT32>(m_threadWorkerVector.size()) )
				{
					threadOrderIndex = 0;
				}
			}
		}

		void StartThreadWorker( UINT32 threadSize )
		{
			for ( UINT32 i = 0; i < threadSize; i++ )
			{
				ThreadWorker *temp = new ThreadWorker;
				temp->Start();
				m_threadWorker.insert( std::map<UINT32, ThreadWorker*>::value_type( temp->GetThreadId(), temp ) );
				m_threadWorkerVector.push_back( temp );
			}			
		}
	};

}

