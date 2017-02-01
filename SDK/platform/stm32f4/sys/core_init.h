/*
 * core_init.h
 *
 * Created: 2/8/2013 12:30:47 AM
 *  Author: XxXx
 */
/*#####################################################*/
#ifndef CORE_INIT_H_
#define CORE_INIT_H_
/*#####################################################*/
/*#####################################################*/

#include <board.h>

extern unsigned long FCPU;
extern unsigned long PLL_REF_CLK;

#if defined (STM32F410Tx) || defined (STM32F410Cx) || defined (STM32F410Rx)  || defined (STM32F411xE)  || defined (STM32F405xx)  || defined (STM32F415xx)  || defined (STM32F407xx)  || defined (STM32F417xx)
#define HSI_VALUE	16000000//Mhz
#endif /* STM32F40_41xxx */

#if defined (STM32F427xx) || defined (STM32F437xx) || defined (STM32F429xx) || defined (STM32F439xx) || defined (STM32F469xx) || defined(STM32F479xx)
#define HSI_VALUE	8000000//Mhz
#endif /* STM32F427_437x || STM32F429_439xx */

#if defined (STM32F401xC) || defined (STM32F401xE)
#define HSI_VALUE	8000000//Mhz
#endif /* STM32F401xx */

extern unsigned long FCPU;
extern unsigned long PLL_REF_CLK;

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
		static void changeCoreClk(unsigned long fCpu);
		static void sleep();
};
}
}
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "core_init.c"
#endif
/*#####################################################*/
#endif /* CORE_INIT_H_ */
/*#####################################################*/
