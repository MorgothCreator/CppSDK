/*
 * sysdelay.c
 *
 * Created: 2/8/2013 10:53:14 PM
 *  Author: XxXx
 */

#include <sys/systime.h>
#include "core_cm0plus.h"

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
//#####################################################
#ifdef __cplusplus
 extern "C" {
#endif
void SysTick_Handler(void)
{
	STimerCnt++;
}
#ifdef __cplusplus
}
#endif
