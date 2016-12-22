/*
 * sysdelay.c
 *
 * Created: 2/8/2013 10:53:14 PM
 *  Author: XxXx
 */

#include <sys/systime.h>
#include "sys/include/core_cm4.h"

extern unsigned long FCPU;
/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
volatile unsigned long long TimerTimeoutValue = 1;
volatile unsigned long long STimerCnt = 0;
//#####################################################
void SysDelayTimerSetup(void)
{
	SysTick_Config(FCPU / 1000);
}
//#####################################################
void sysDelay(unsigned long milliSec)
{
	TimerTimeoutValue = STimerCnt + (unsigned long long)milliSec;
	while(1) {
		if(TimerTimeoutValue < STimerCnt) break;
	}
}
//#####################################################
void TimerCnt_Isr_Increment(void)
{
	STimerCnt++;
}
