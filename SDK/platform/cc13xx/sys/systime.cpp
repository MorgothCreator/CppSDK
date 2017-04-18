/*
 * sysdelay.c
 *
 * Created: 2/8/2013 10:53:14 PM
 *  Author: XxXx
 */ 

#include <sys/systime.h>
#include <driver/systick.h>
/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
static volatile unsigned long long flagIsr = 1;
volatile unsigned long long STimerCnt;
//Rtc_t RtcStruct;

GI::Sys::Systime sysTime = GI::Sys::Systime();

void TimerCnt_Isr_Increment(void)
{
    STimerCnt++;
}
//#####################################################
GI::Sys::Systime::Systime()
{
    SysTickEnable();
    SysTickPeriodSet(48000);
    SysTickIntRegister(TimerCnt_Isr_Increment);
    SysTickIntEnable();
}
//#####################################################
GI::Sys::Systime::~Systime()
{

}
//#####################################################
void GI::Sys::Systime::sysDelay(unsigned long milliSec)
{
	flagIsr = STimerCnt + milliSec;
	while(flagIsr < STimerCnt);
}
//#####################################################
/*ISR(RTC_OVF_vect)
{
	STimerCnt++;
}*/
