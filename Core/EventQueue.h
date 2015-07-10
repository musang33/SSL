#pragma once

#include <functional>
#include <future>
#include <concurrent_queue.h>

namespace SSL
{
	class EventQueue
	{
	public:
		EventQueue();
		~EventQueue();

	public:
		template<class F, class... Args>
		auto PushEvent( F&& f, Args&&... args )
			->std::future < typename std::result_of<F( Args... )>::type > ;

	public:
		void UpdateQueue();

	private:
		// the task queue
		concurrency::concurrent_queue< std::function<void()> > tasks;
	};

	// add new work item to the pool
	template<class F, class... Args>
	auto EventQueue::PushEvent( F&& f, Args&&... args )
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