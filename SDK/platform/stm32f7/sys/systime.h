/*
 * sysdelay.h
 *
 * Created: 2/8/2013 10:53:25 PM
 *  Author: XxXx
 */
//#####################################################
#ifndef SYSDELAY_H_
#define SYSDELAY_H_
//#####################################################
extern volatile unsigned long long STimerCnt;

void SysDelayTimerSetup(void);
void sysDelay(unsigned long milliSec);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "sysdelay.c"
#endif
//#####################################################
#endif /* SYSDELAY_H_ */
//#####################################################
