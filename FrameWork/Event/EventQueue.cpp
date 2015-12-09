#include "stdafx.h"

#include <Event/EventQueue.h>

namespace SSL
{
	EventQueue::EventQueue()
	{

	}

	EventQueue::~EventQueue()
	{

	}

	void EventQueue::UpdateTaskQueue()
	{
		while ( false == this->tasks.empty() )
		{
			std::function<void()> task;
			this->tasks.try_pop( task );
			task();
		}
	}

}