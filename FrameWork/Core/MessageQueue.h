#pragma once

#include <concurrent_queue.h>

namespace SSL
{
	template<typename T>
	class MessageConcurrentQueue
	{
	public:		
		bool Push(const T& val)
		{
			m_queue.push(val);

			return true;
		}

		bool Pop(T& val)
		{
			return m_queue.try_pop(val);
		}

		size_t GetSize()
		{
			return m_queue.unsafe_size();
		}

		bool IsEmpty( )
		{
			return m_queue.empty( );
		}

	private:
		Concurrency::concurrent_queue<T>	m_queue;
	};


}
