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
void GI::Sys::Timer::interval(unsigned long long _Value)
{
#ifdef USE_RTC_100_MS
	sTimmerInterval = (_Value & (unsigned long long)INT64_MAX) / 10;
#else
	sTimmerInterval = _Value & (unsigned long long)INT64_MAX;
#endif
	enable(1);
}
//#####################################################
void GI::Sys::Timer::enable(unsigned char Action)
{
	if (Action == 1)
	{
		sTimmerTickValue = (sTimmerInterval + STimerCnt)&(unsigned long long)INT64_MAX;
	}
	else
	{
		sTimmerTickValue = sTimmerTickValue | (unsigned long long)INT64_MIN;
	}
}
//#####################################################
bool GI::Sys::Timer::tick()
{
	if(!(sTimmerTickValue & (unsigned long long)INT64_MIN))
	{
		unsigned long long Temp1 = sTimmerTickValue & (unsigned long long)INT64_MAX;
		unsigned long long Temp2 = STimerCnt & (unsigned long long)INT64_MAX;
		unsigned long long Temp = (Temp1 - Temp2 - 1)& (unsigned long long)INT64_MIN;
		if(Temp)
		{
			enable(1);
			return true;
		}
	}
	return false;
}
//#####################################################
bool GI::Sys::Timer::enabled()
{
	if(sTimmerTickValue & (unsigned long long)INT64_MIN) return false;
	else return true;
}
//#####################################################
void GI::Sys::Timer::delay(unsigned int milliSec) {
	sysDelay(milliSec);
}
//#####################################################
