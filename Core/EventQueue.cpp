#pragma once

#include "EventQueue.h"

namespace SSL
{
	EventQueue::EventQueue()
	{

	}

	EventQueue::~EventQueue()
	{

	}

	void EventQueue::UpdateQueue()
	{
		while ( false == this->tasks.empty() )
		{
			std::function<void()> task;
			this->tasks.try_pop( task );
			task();
		}
	}

}