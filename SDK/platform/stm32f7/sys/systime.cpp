/*
 * sysdelay.c
 *
 * Created: 2/8/2013 10:53:14 PM
 *  Author: XxXx
 */

#include <sys/systime.h>
#include "sys/include/core_cm7.h"

extern unsigned long FCPU;
/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
volatile unsigned long long TimerTimeoutValue = 1;
volatile unsigned long long STimerCnt = 0;
//#####################################################
GI::Sys::Systime::Systime()
{
	SysTick_Config(FCPU / 1000);
}

GI::Sys::Systime::~Systime()
{

}
//#####################################################
void GI::Sys::Systime::sysDelay(unsigned long milliSec)
{
	TimerTimeoutValue = STimerCnt + (unsigned long long)milliSec;
	while(1) {
		if(TimerTimeoutValue < STimerCnt) break;
	}
}
