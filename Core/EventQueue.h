#pragma once

#include <queue>
#include <functional>
#include <future>

class EventQueue
{
public:
	EventQueue();
	~EventQueue();

public:
	template<class F, class... Args>
	auto PushEvent( F&& f, Args&&... args )
		->std::future < typename std::result_of<F( Args... )>::type >;

public:
	void UpdateQueue();

private:	
	// the task queue
	std::queue< std::function<void()> > tasks;

	// synchronization
	std::mutex queue_mutex;	
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
	{
		std::unique_lock<std::mutex> lock( queue_mutex );

		tasks.emplace( [task](){ ( *task )( ); } );
	}
	
	return res;
}