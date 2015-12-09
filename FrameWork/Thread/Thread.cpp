#include "stdafx.h"

#include <Thread/Thread.h>
#include <process.h>

namespace SSL
{
	unsigned __stdcall ThreadFunc( void* pArguments )
	{
		Thread* thread_class = static_cast< Thread* >( pArguments );
		while ( thread_class->isActive() )
		{
			thread_class->run();
		}

		thread_class->destory();

		return 0;
	}

	Thread::Thread()
		: m_thread(NULL)
		, m_threadId(0)
		, m_isActive(false)
	{
	}

	Thread::~Thread()
	{
		if ( NULL != m_thread )
		{
			CloseHandle( m_thread );
		}
	}

	bool Thread::isActive() const
	{
		return m_isActive;
	}

	bool Thread::Start()
	{
		if ( false == isActive() )
		{
			m_isActive = true;
			m_thread = reinterpret_cast<HANDLE>(_beginthreadex( nullptr, 0, ThreadFunc, this, 0, &m_threadId ));

			return true;
		}

		return false;
	}

	void Thread::Stop()
	{
		if ( isActive() )
		{
			m_isActive = false;
		}

		WaitForSingleObject( m_thread, INFINITE );
	}

}