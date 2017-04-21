/*
 * sysdelay.c
 *
 * Created: 2/8/2013 10:53:14 PM
 *  Author: XxXx
 */

#include <sys/systime.h>
#include <driver/rtc_count.h>
#include <driver/rtc_count_interrupt.h>

GI::Sys::Systime time = GI::Sys::Systime();
//extern unsigned long FCPU;
/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
volatile unsigned long long TimerTimeoutValue = 1;
volatile unsigned long long STimerCnt = 0;
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
//#####################################################
//! [rtc_module_instance]
struct rtc_module rtc_instance;
//! [rtc_module_instance]

GI::Sys::Systime::Systime()
{
	//! [init_conf]
	struct rtc_count_config config_rtc_count;
	rtc_count_get_config_defaults(&config_rtc_count);
	//! [init_conf]

	//! [set_config]
	config_rtc_count.prescaler           = RTC_COUNT_PRESCALER_DIV_1;
	config_rtc_count.mode                = RTC_COUNT_MODE_16BIT;
#ifdef FEATURE_RTC_CONTINUOUSLY_UPDATED
	config_rtc_count.continuously_update = true;
#endif
	//! [set_config]
	//! [init_rtc]
	rtc_count_init(&rtc_instance, RTC, &config_rtc_count);
	//! [init_rtc]

	//! [enable]
	rtc_count_enable(&rtc_instance);
	//! [enable]
	//! [reg_callback]
	rtc_count_register_callback(
	&rtc_instance, SysTick_Handler, RTC_COUNT_CALLBACK_OVERFLOW);
	//! [reg_callback]
	//! [en_callback]
	rtc_count_enable_callback(&rtc_instance, RTC_COUNT_CALLBACK_OVERFLOW);
	//! [en_callback]
	//! [period]
	rtc_count_set_period(&rtc_instance, 32);
	//! [period]
}

GI::Sys::Systime::~Systime()
{
	//RTC.CTRL = RTC_PRESCALER_OFF_gc;
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
