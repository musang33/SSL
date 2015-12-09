#include "stdafx.h"

#include <Core/TimerWorker.h>
#include <atltime.h>

namespace SSL
{
	UINT16 TimerJob::getLastDay( INT32 year, INT32 month )
	{
		struct tm when;
		time_t lastday;

		// Set up current month
		when.tm_hour = 0;
		when.tm_min = 0;
		when.tm_sec = 0;
		when.tm_mday = 1;

		// Next month 0=Jan
		if ( month == 12 )
		{
			when.tm_mon = 0;
			when.tm_year = year - 1900 + 1;
		}
		else
		{
			when.tm_mon = month;
			when.tm_year = year - 1900;
		}
		// Get the first day of the next month
		lastday = mktime( &when );

		// Subtract 1 day
		lastday -= 86400;

		// Convert back to date and time
		localtime_s( &when, &lastday );

		return static_cast<UINT16>( when.tm_mday );
	}

	void TimerJob::updateDayMonthYear( UINT16 currentYear, UINT16 currentMonth, SYSTEMTIME& systemTime )
	{
		UINT16 lastDay = getLastDay( currentYear, currentMonth );
		systemTime.wDay += 1;

		if ( systemTime.wDay > lastDay )
		{
			systemTime.wDay = 1;
			systemTime.wMonth += 1;
			if ( systemTime.wMonth > 12 )
			{
				systemTime.wMonth = 1;
				systemTime.wYear++;
			}
		}
	}

	UINT64 TimerJob::getDiffTick( SYSTEMTIME systemTime )
	{		
		CTime time( CTime::GetCurrentTime() );

		SYSTEMTIME timeDest;
		time.GetAsSystemTime( timeDest );

		systemTime.wMonth = timeDest.wMonth;
		systemTime.wYear = timeDest.wYear;

		if ( 0 != systemTime.wDayOfWeek )
		{
			// 특정 요일이 지정되면

			if ( systemTime.wDayOfWeek > 7 )
			{
				// 일요일이 1, 토요일이 7
				return 0;
			}

			UINT16 lastDay = getLastDay( timeDest.wYear, timeDest.wMonth );
			INT32 currentDayOfWeek = time.GetDayOfWeek();

			if ( systemTime.wDayOfWeek < currentDayOfWeek )
			{
				systemTime.wDay = timeDest.wDay + static_cast<UINT16>( ( 7 - ( currentDayOfWeek - systemTime.wDayOfWeek ) ) );
			}
			else if ( systemTime.wDayOfWeek > currentDayOfWeek )
			{
				systemTime.wDay = timeDest.wDay + static_cast<UINT16>( ( systemTime.wDayOfWeek - currentDayOfWeek ) );
			}
			else
			{
				systemTime.wDay = timeDest.wDay;
				if ( systemTime.wHour < timeDest.wHour )
				{
					systemTime.wDay += 7;
				}
				else if ( systemTime.wHour == timeDest.wHour )
				{
					if ( systemTime.wMinute < timeDest.wMinute )
					{
						systemTime.wDay += 7;
					}
					else if ( systemTime.wMinute == timeDest.wMinute )
					{
						if ( systemTime.wSecond < timeDest.wSecond )
						{
							systemTime.wDay += 7;
						}
					}
				}
			}

			if ( systemTime.wDay > lastDay )
			{
				systemTime.wMonth += 1;
				if ( systemTime.wMonth > 12 )
				{
					systemTime.wMonth = 1;
					systemTime.wYear++;
				}

				systemTime.wDay -= lastDay;
			}
		}
		else
		{
			// 일 시 분 초가 지정이 되면
			INT32 currentDay = time.GetDay();
			if ( systemTime.wDay != 0 )
			{
				// 특정 '일' 지정
				if ( systemTime.wDay < currentDay )
				{
					systemTime.wMonth += 1;
					if ( systemTime.wMonth > 12 )
					{
						systemTime.wMonth = 1;
						systemTime.wYear++;
					}
				}
				else if ( systemTime.wDay > currentDay )
				{

				}
				else
				{
					if ( systemTime.wHour <= timeDest.wHour && systemTime.wMinute <= timeDest.wMinute && systemTime.wSecond < timeDest.wSecond )
					{
						systemTime.wMonth += 1;
						if ( systemTime.wMonth > 12 )
						{
							systemTime.wMonth = 1;
							systemTime.wYear++;
						}
					}
				}
			}
			else
			{
				systemTime.wDay = timeDest.wDay;
				if ( systemTime.wHour != 0 )
				{
					if ( systemTime.wHour <= timeDest.wHour && systemTime.wMinute <= timeDest.wMinute && systemTime.wSecond < timeDest.wSecond )
					{
						updateDayMonthYear( timeDest.wYear, timeDest.wMonth, systemTime );
					}
				}
				else
				{
					systemTime.wHour = timeDest.wHour;
					if ( systemTime.wMinute != 0 )
					{
						if ( systemTime.wMinute <= timeDest.wMinute && systemTime.wSecond < timeDest.wSecond )
						{
							systemTime.wHour += 1;
							if ( systemTime.wHour > 23 )
							{
								systemTime.wHour = 0;

								updateDayMonthYear( timeDest.wYear, timeDest.wMonth, systemTime );
							}
						}
					}
					else
					{
						systemTime.wMinute = timeDest.wMinute;
						if ( systemTime.wSecond != 0 )
						{
							if ( systemTime.wSecond < timeDest.wSecond )
							{
								systemTime.wMinute += 1;
								if ( systemTime.wMinute > 59 )
								{
									systemTime.wMinute = 0;

									systemTime.wHour += 1;
									if ( systemTime.wHour > 23 )
									{
										systemTime.wHour = 0;

										updateDayMonthYear( timeDest.wYear, timeDest.wMonth, systemTime );
									}
								}
							}
						}
					}
				}
			}

			INT32 lastDay = getLastDay( timeDest.wYear, timeDest.wMonth );
			if ( systemTime.wDay > lastDay )
			{
				return 0;
			}
		}

		timeDest.wYear = systemTime.wYear;
		timeDest.wMonth = systemTime.wMonth;
		timeDest.wDay = systemTime.wDay;
		timeDest.wHour = systemTime.wHour;
		timeDest.wMinute = systemTime.wMinute;
		timeDest.wSecond = systemTime.wSecond;

		CTime destTime( timeDest );

		UINT64 osBinaryTime = destTime.GetTime();
		UINT64 diff = osBinaryTime - time.GetTime();

		return diff;
	}
} // namespace SSL
