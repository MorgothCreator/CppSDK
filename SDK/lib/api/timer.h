/*
 *  api/timer.h
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

#ifndef SYS_TIMER_H_
#define SYS_TIMER_H_
/*#####################################################*/
#include <stdbool.h>
#include "sys/systime.h"
#include <include/global.h>
/*#####################################################*/
namespace GI
{
namespace Sys {
	class Timer {
	public:
		Timer();
		Timer(u64 interval);
		~Timer();
		void interval(u64 interval);
		void enable(bool action);
		bool tick();
		bool enabled();
		static void delay(u32 milliSec);
	private:
		unsigned long long sTimmerInterval;				//Here, user will store the address of buffer used to receive data
		unsigned long long sTimmerTickValue;				//Here, user will store the address of buffer used to receive data
	};
}
}
/*#####################################################*/

/*#####################################################*/
#define timer(Name)  STimer_t Name
/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include <api/timer.cpp>
#endif
/*#####################################################*/
#endif /* SYS_TIMER_H_ */
