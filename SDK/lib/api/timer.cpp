/*
 *  api/timer.cpp
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "sys/systime.h"
#include "timer.h"
/*#####################################################*/
extern volatile unsigned long long STimerCnt;
//#####################################################
/*void timer_init()
{
	SysDelayTimerSetup();
}*/
//#####################################################
GI::Sys::Timer::Timer()
{
	memset(this, 0, sizeof(*this));
}
//#####################################################
GI::Sys::Timer::Timer(u64 interval)
{
	memset(this, 0, sizeof(*this));
	sTimmerInterval = interval & (u64)__INT64_MAX__;
	enable(true);
}
//#####################################################
GI::Sys::Timer::~Timer()
{

}
//#####################################################
void GI::Sys::Timer::interval(u64 interval)
{
#ifdef USE_RTC_100_MS
	sTimmerInterval = (interval & (u64)INT64_MAX) / 10;
#else
	sTimmerInterval = interval & (u64)__INT64_MAX__;
#endif
	enable(true);
}
//#####################################################
void GI::Sys::Timer::enable(bool action)
{
	if (action)
	{
		sTimmerTickValue = (sTimmerInterval + STimerCnt)&(u64)__INT64_MAX__;
	}
	else
	{
		sTimmerTickValue = sTimmerTickValue | (u64)(-__INT64_MAX__ - 1);
	}
}
//#####################################################
bool GI::Sys::Timer::tick()
{
	if(!(sTimmerTickValue & (u64)(-__INT64_MAX__ - 1)))
	{
		u64 Temp1 = sTimmerTickValue & (u64)__INT64_MAX__;
		u64 Temp2 = STimerCnt & (u64)__INT64_MAX__;
		u64 Temp = (Temp1 - Temp2 - 1)& (u64)(-__INT64_MAX__ - 1);
		if(Temp)
		{
			enable(true);
			return true;
		}
	}
	return false;
}
//#####################################################
bool GI::Sys::Timer::enabled()
{
	if(sTimmerTickValue & (u64)(-__INT64_MAX__ - 1))
		return false;
	else
		return true;
}
//#####################################################
void GI::Sys::Timer::delay(u32 milliSec) {
	GI::Sys::Systime::sysDelay(milliSec);
}
//#####################################################
