#pragma once

#include "CommonData.h"
#include <queue>
#include <vector>
#include <functional>
#include <memory>

namespace SSL
{
	class TimerJob
	{
	public:
		TimerJob( std::function<void()> func_, UINT32 processCount_ )
			: func( func_ )
			, processCount( processCount_ )
			, expireTick( 0 )
		{
		}

		BOOL Check()
		{
			if ( ::GetTickCount64() > expireTick )
			{
				return true;
			}

			return false;
		}

		UINT64 RemainTime() const  { return expireTick; }
		UINT32 CanBeProcessed()
		{
			if ( processCount == 0 )
			{
				return false;
			}

			processCount--;

			return true;
		}
		BOOL IsRepeat()
		{
			if ( processCount > 0 )
			{
				return true;
			}

			return false;
		}

		virtual void Next() = 0;

		std::function<void()> func;

	protected:
		UINT64 expireTick;
		UINT32 processCount;

		UINT16 getLastDay( INT32 year, INT32 month );
		void updateDayMonthYear( UINT16 currentYear, UINT16 currentMonth, SYSTEMTIME& systemTime );
		UINT64 getDiffTick( SYSTEMTIME systemTime );
	};

	typedef std::shared_ptr<TimerJob> TimerJobPtr;

	// 상대시간 처리
	class RelativeTimerJob : public TimerJob
	{
	public:
		RelativeTimerJob() = delete;
		RelativeTimerJob( UINT32 interval_, UINT32 repeatCount_, std::function<void()> func_ )
			: TimerJob( func_, repeatCount_ )
			, interval( interval_ )
		{
			expireTick = ::GetTickCount64() + interval;
		}

		virtual void Next() { expireTick = ::GetTickCount64() + interval; };

	private:
		UINT32 interval;

	};

	// 절대시간으로 처리
	class AbsoluteTimerJob : public TimerJob
	{
	public:
		AbsoluteTimerJob() = delete;
		AbsoluteTimerJob( SYSTEMTIME& systemTime_, UINT32 repeatCount_, std::function<void()> func_ )
			: TimerJob( func_, repeatCount_ )
			, systemTime( systemTime_ )
		{
			expireTick = ::GetTickCount64() + getDiffTick( systemTime );
		}

		virtual void Next() { expireTick = ::GetTickCount64() + getDiffTick( systemTime ); };

	private:
		SYSTEMTIME systemTime;
	};

	class CompareTimerJob
	{
	public:
		BOOL operator()( const std::shared_ptr<TimerJob> lhs, const std::shared_ptr<TimerJob> rhs ) const
		{
			return lhs->RemainTime() > rhs->RemainTime();
		}
	};

	/*
	* 주기적으로 만료시간을 검사하여 실행
	*/
	class TimerWorker
	{
	public:
		template<typename  T>
		void Push( T& job )
		{
			m_q.emplace( job );
		}

		/*
		* 만료시간 기준으로 정렬, 만료시간까지만 처리
		*/
		void Run()
		{
			while ( !m_q.empty() )
			{
				auto& job = m_q.top();
				if ( job->Check() )
				{
					if ( job->CanBeProcessed() )
					{
						job->func();
					}

					if ( job->IsRepeat() )
					{
						auto jobRepeat = m_q.top();
						m_q.pop();
						jobRepeat->Next();
						m_q.emplace( jobRepeat );
					}
					else
					{
						m_q.pop();
					}
				}
				else
				{
					return;
				}
			}
		}

		typedef std::priority_queue<std::shared_ptr<TimerJob>, std::vector<std::shared_ptr<TimerJob>>, CompareTimerJob> TimerJobQueue;

	private:
		TimerJobQueue m_q;
	};
} // namespace SSL

