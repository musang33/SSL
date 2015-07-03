#pragma once

#include "EventQueue.h"

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
		{
			std::unique_lock<std::mutex> lock( this->queue_mutex );			

			task = std::move( this->tasks.front() );
			this->tasks.pop();
		}

		task();
	}
}