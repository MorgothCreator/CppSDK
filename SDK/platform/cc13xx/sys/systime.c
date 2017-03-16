/*
 * sysdelay.c
 *
 * Created: 2/8/2013 10:53:14 PM
 *  Author: XxXx
 */ 

#include <sys/systime.h>

/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
static volatile unsigned long long flagIsr = 1;
volatile unsigned long long STimerCnt;
//Rtc_t RtcStruct;

//#####################################################
void SysDelayTimerSetup(void)
{

}
//#####################################################
void sysDelay(unsigned int milliSec)
{
	flagIsr = STimerCnt + milliSec;
	while(flagIsr < STimerCnt);
}
//#####################################################
/*ISR(RTC_OVF_vect)
{
	STimerCnt++;
}*/
