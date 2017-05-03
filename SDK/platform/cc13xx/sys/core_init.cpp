/*
 * core_init.c
 *
 * Created: 2/8/2013 12:30:34 AM
 *  Author: XxXx
 */ 

#include <stdbool.h>
#include "core_init.h"
//#include "board_properties.h"
#include "driver/sys_ctrl.h"
#include <sys/systime.h>
#include <driver/rfc.h>

unsigned long CoreFreq = 48000000;
unsigned long FCPU = CORE_CLOCK_DEFAULT;
unsigned long PLL_REF_CLK = HSE_VALUE;

static void SystemClock_Config(unsigned long int_osc_freq,
		unsigned long ext_osc_freq, unsigned long core_freq)
{
	SysCtrlPowerEverything();
    OSCHF_TurnOnXosc();
    do{}while(!OSCHF_AttemptToSwitchToXosc());
	CoreFreq = core_freq;
    volatile int cnt = 0;

    for(cnt = 0; cnt < 250000; cnt++);
    RFCClockEnable();//clocks bits are enabled properly
    for(cnt = 0; cnt < 250000; cnt++);
    PRCMPowerDomainOn(PRCM_DOMAIN_RFCORE | PRCM_DOMAIN_CPU | PRCM_DOMAIN_VIMS);
}

GI::Sys::Clock::Clock()
{
	setClk(FCPU);
}

GI::Sys::Clock::~Clock()
{
	//SystemClock_Config(HSI_VALUE, 0, 0);
}

void GI::Sys::Clock::setClk(unsigned long fCpu)
{
	SystemClock_Config(HSI_VALUE, HSE_VALUE, fCpu);
}

void GI::Sys::Clock::sleep()
{
	  /* Request to enter SLEEP mode */
}


void _core_init(void)
{
	SysCtrlPowerEverything();
	CoreFreq = 48000000;
}


