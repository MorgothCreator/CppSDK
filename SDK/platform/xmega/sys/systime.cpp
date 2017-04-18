/*
 * sysdelay.c
 *
 * Created: 2/8/2013 10:53:14 PM
 *  Author: XxXx
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <sys/systime.h>
GI::Sys::Systime systime = GI::Sys::Systime();
//extern unsigned long FCPU;
/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
volatile unsigned long long TimerTimeoutValue = 1;
volatile unsigned long long STimerCnt = 0;
//#####################################################
GI::Sys::Systime::Systime()
{
	asm("sei");
	unsigned char pmic_reg = PMIC.CTRL;
	pmic_reg |= PMIC_HILVLEN_bm;
	PROTECTED_WRITE(PMIC.CTRL, pmic_reg);
	RTC.PER = 9;
	RTC.COMP = 9;
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;
	RTC.INTCTRL = RTC_OVFINTLVL_HI_gc;
	RTC.INTFLAGS = RTC_OVFIF_bm;
	CLK.RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;
}

GI::Sys::Systime::~Systime()
{
	RTC.CTRL = RTC_PRESCALER_OFF_gc;
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
ISR(RTC_OVF_vect)
{
	STimerCnt += 10;
}
#ifdef __cplusplus
}
#endif
