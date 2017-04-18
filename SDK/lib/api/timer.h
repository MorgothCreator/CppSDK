/*
 *  api/timer.h
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
