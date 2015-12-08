#pragma once


#include "EventQueue.h"
#include "Dispatcher.h"
#include "Thread.h"

namespace SSL
{
	class ThreadWorker : public Thread, public EventQueue
	{
	public:
		ThreadWorker();
		~ThreadWorker();	

	public:
		UINT32 run() override;
		void ProcessEventQueue() override;

	public:
		void OnAddHP( EVENTPtr& ptr );
		void OnUpdateEntity( EVENTPtr& ptr );

	private:
		Dispatcher<ThreadWorker> m_dispatcher;

		
	};

}