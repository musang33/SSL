#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include "Singleton.h"
#include "ThreadEventManager.h"

namespace SSL
{	
	__declspec( thread ) ThreadEventManager* threadEventManager;

	class ThreadPool : public Singleton<ThreadPool>
	{		
	public:
		ThreadPool()
			: stop( false ) {};
		ThreadPool( size_t );

		template<class F, class... Args>
		auto enqueue( F&& f, Args&&... args )
			->std::future < typename std::result_of<F( Args... )>::type > ;		

		~ThreadPool();
		void Init( size_t threads );
	private:
		template<class... Args>
		SSL::BaseEntity* getBaseEntity( int threadId, int id, Args&&... args );

		// need to keep track of threads so we can join them
		std::vector< std::thread > workers;

		// the task queue
		std::queue< std::function<void()> > tasks;		
		
		// synchronization
		std::mutex queue_mutex;
		std::condition_variable condition;
		bool stop;		

		std::vector< UINT32 > threadIds;		
	};

	// the constructor just launches some amount of workers
	inline ThreadPool::ThreadPool( size_t threads )
		: stop( false )
	{
		this->Init( threads );
	}

	void ThreadPool::Init( size_t threads )
	{				
		for ( size_t i = 0; i < threads; ++i )
			workers.emplace_back(
			[this]
		{		
			threadEventManager = new ThreadEventManager;
			for ( ;; )
			{
				std::function<void()> task;
				{
					// condition »ç¿ë
					std::unique_lock<std::mutex> lock( this->queue_mutex );
					this->condition.wait( lock,											  
										  [this]{ return this->stop || !this->tasks.empty(); } );

					if ( this->stop && this->tasks.empty() )
						return;				
					
					task = std::move( this->tasks.front() );
					this->tasks.pop();
				}

				task();
			}
		}
		);		
	}

	template<class F, class... Args>
	auto ThreadPool::enqueue( F&& f, Args&&... args )
		-> std::future < typename std::result_of<F( Args... )>::type >
	{			
		using return_type = typename std::result_of<F( Args... )>::type;		

		auto task = std::make_shared< std::packaged_task<return_type()> >(
			std::bind( std::forward<F>( f ), std::forward<Args>( args )... )
			);	
		
		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock( queue_mutex );

			// don't allow enqueueing after stopping the pool
			if ( stop )
				throw std::runtime_error( "enqueue on stopped ThreadPool" );
			
			tasks.emplace( ( [task](){ ( *task )( ); } ) );
		}
		condition.notify_one();

		return res;
	}

	// the destructor joins all threads
	inline ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock( queue_mutex );
			stop = true;
		}
		condition.notify_all();
		for ( std::thread &worker : workers )
			worker.join();
	}

	/*int threadFunc( int i )
	{
		std::cout << "\nhello " << i << std::endl;
		std::this_thread::sleep_for( std::chrono::seconds( 10 ) );
		std::cout << "world " << i << std::endl;

		return i*i;
	}
*/
	//int main()
	//{

	//	ThreadPool pool( 4 );
	//	std::vector< std::future<int> > results;

	//	for ( int i = 0; i < 8; ++i )
	//	{
	//		results.emplace_back( pool.enqueue( std::bind( &threadFunc, i ) ) );
	//		for ( auto && result : results )
	//		{
	//			if ( result.valid() )
	//			{
	//				std::cout << "result : " << result.get() << ' ';
	//			}
	//		}

	//	}

	//	/*for ( auto && result : results )
	//	std::cout << result.get() << ' ';*/
	//	std::cout << std::endl;

	//	return 0;
	//}
}