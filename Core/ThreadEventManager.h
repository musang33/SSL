#pragma once
#include "CommonData.h"
#include <atomic>
#include <functional>
#include <concurrent_queue.h>
#include "Singleton.h"
#include "BaseEntity.h"

namespace SSL
{	
	class ThreadEventManager : public Singleton<ThreadEventManager>
	{
	public:
		class EventStruct
		{
			BaseEntity::BaseEntityPtr	baseEntity;			
			std::function<void()>		func;
		};

		typedef std::shared_ptr<EventStruct> EventStructPtr;

	private:
		concurrency::concurrent_queue< EventStructPtr > events;

	public:
		ThreadEventManager();
		~ThreadEventManager();

	public:
		void PushEvent( EventStructPtr _event )
		{
			events.push( _event );
		}
	};

}

