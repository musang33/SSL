#pragma once

#include <../Framework/Core/Dispatcher.h>
#include <../Framework/Event/EventQueue.h>
#include <../Framework/Thread/Thread.h>

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