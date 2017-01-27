/*
 * core_init.h
 *
 * Created: 2/8/2013 12:30:47 AM
 *  Author: XxXx
 */
/*#####################################################*/
#ifndef CORE_INIT_H_
#define CORE_INIT_H_

#include <stdbool.h>
#include <string.h>
#include <board.h>
#include "system_stm32f7xx.h"
#include "driver/stm32f7xx_hal.h"
#include "driver/stm32f7xx_hal_rcc.h"
#include "driver/stm32f7xx_hal_rcc_ex.h"
#include "driver/stm32f7xx_hal_pwr_ex.h"
#include "driver/stm32f7xx_hal_flash_ex.h"
#include <driver/stm32769i_discovery_sdram.h>
/*#####################################################*/
/*#####################################################*/
extern unsigned long FCPU;
extern unsigned long PLL_REF_CLK;

#define HSI_VALUE	16000000//Mhz
#define LSI_VALUE  ((uint32_t)32000)
#define HSE_STARTUP_TIMEOUT    ((uint32_t)5000)   /*!< Time out for HSE start up, in ms */
#define LSE_STARTUP_TIMEOUT    ((uint32_t)5000U)   /*!< Time out for LSE start up, in ms */
//*****************************************************************************
//
// The ASSERT macro, which does the actual assertion checking.  Typically, this
// will be for procedure arguments.
//
//*****************************************************************************
#ifdef DEBUG
#define ASSERT(expr) {                                      \
                         if(!(expr))                        \
                         {                                  \
                             __error__(__FILE__, __LINE__); \
                         }                                  \
                     }
#else
#define ASSERT(expr)
#endif
/*#####################################################*/
#define pi 3.14159265358979323846264338327950288419716939937510
/*#####################################################*/
namespace GI
{
namespace Sys
{
	class Clock {
	public:
		Clock();
		~Clock();
		static void setClk(unsigned long fCpu);
};
}
}
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include <sys/core_init.cpp>
#endif
/*#####################################################*/
#endif /* CORE_INIT_H_ */
/*#####################################################*/
