/*
 * gpio_interface.hpp
 *
 *  Created on: Dec 9, 2016
 *      Author: John Smith
 */

#ifndef STM32F7_INTERFACE_GPIO_H_
#define STM32F7_INTERFACE_GPIO_H_

/*#####################################################*/
#include <stdbool.h>
#include <include/global.h>
#include "inc/hw_gpio.h"

#define HAVE_GPIO_IN_FLOATING
#define HAVE_GPIO_IN_PULL_DOWN
#define HAVE_GPIO_IN_PULL_UP
#define HAVE_GPIO_OUT_OPEN_DRAIN
#define HAVE_GPIO_OUT_PUSH_PULL
/*#####################################################*/
#define _GPIO_FAST_SET_PIN(base_addr, pin)\
		HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << (pin % 32)

#define _GPIO_FAST_CLEAR_PIN(base_addr, pin)\
		HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << (pin % 32)

#define _GPIO_FAST_WRITE_MULTI_PIN(base_addr, mask, pin_mask)\
		HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) & ~mask) | ((pin_mask) & mask)

/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "gpio_interface.c"
#endif

#endif /* STM32F7_INTERFACE_GPIO_H_ */
