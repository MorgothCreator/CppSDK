/*
 *  api/timer.cpp
 *
 *  Copyright (C) 2016  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of CppSDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
