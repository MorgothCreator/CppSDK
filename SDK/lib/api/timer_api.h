/*
 *  lib/api/timer_api.h
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
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
/*#####################################################*/
namespace GI
{
namespace Sys {
	class Timer {
	public:
		void interval(unsigned long long _Value);
		void enable(unsigned char Action);
		bool tick();
		bool enabled();
		static void delay(unsigned int milliSec);
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
#ifdef HEADER_INCLUDE_C_FILES
#include <api/timer_api.cpp>
#endif
/*#####################################################*/
#endif /* SYS_TIMER_H_ */
