#include "ThreadWorker.h"
#include "BaseEntity.h"
#include "EntityManager.h"
#include "EventQueue.h"

namespace SSL
{
	ThreadWorker::ThreadWorker()
	{
	}


	ThreadWorker::~ThreadWorker()
	{
	}

	void ThreadWorker::initTLS()
	{
		eventQueue = new EventQueue;
	}

	void ThreadWorker::Run()
	{
		initTLS();

		while ( true )
		{			
			eventQueue->UpdateQueue();
		}
	}

}