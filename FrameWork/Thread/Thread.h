#pragma once

#include <Common/CommonData.h>

namespace SSL
{
	class Thread
	{
	public:
		Thread();
		virtual ~Thread();
		
	public:
		bool isActive() const;
		bool Start();
		void Stop();
		UINT32 GetThreadId() const { return m_threadId; };

	public:
		virtual UINT32 run() = 0;
		virtual void destory() {};

	private:
		HANDLE	m_thread;
		UINT32	m_threadId;
		bool	m_isActive;
	};
}
