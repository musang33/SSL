#pragma once

#include <functional>
#include <future>
#include <concurrent_queue.h>
#include <Entity/Entity.h>
#include <Event/Event.h>

namespace SSL
{
	class EventQueue
	{
	public:
		struct EventStruct
		{
			Entity::BaseEntityPtr	baseEntity;
			std::function<void()>		func;
			EventStruct( Entity::BaseEntityPtr ptr, std::function<void()> _func )
				: baseEntity(ptr)
				, func(_func)
			{				
			};

		};
		typedef std::shared_ptr<EventStruct> EventStructPtr;

	public:
		EventQueue();
		~EventQueue();

	public:
		template<class F, class... Args>
		auto PushTask( F&& f, Args&&... args )
			->std::future < typename std::result_of<F( Args... )>::type > ;

		void PushEvent( EVENTPtr _event )
		{
			events.push( _event );
			ProcessEventQueue();
		}

	public:
		void UpdateTaskQueue();
		virtual void ProcessEventQueue() = 0;		

	protected:
		// the task queue
		concurrency::concurrent_queue< std::function<void()> > tasks;
		concurrency::concurrent_queue< EVENTPtr > events;
	};

	// add new work item to the pool
	template<class F, class... Args>
	auto EventQueue::PushTask( F&& f, Args&&... args )
		-> std::future < typename std::result_of<F( Args... )>::type >
	{
		using return_type = typename std::result_of<F( Args... )>::type;

		auto task = std::make_shared< std::packaged_task<return_type()> >(
			std::bind( std::forward<F>( f ), std::forward<Args>( args )... )
			);

		std::future<return_type> res = task->get_future();

		tasks.emplace( [task](){ ( *task )( ); } );

		return res;
	}

}