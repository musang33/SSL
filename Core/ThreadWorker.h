#pragma once

#include "Thread.h"
#include "EventQueue.h"
#include "Dispatcher.h"

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