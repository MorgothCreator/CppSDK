/*
 * interface/gpio.hpp
 */

#ifndef STM32F7_INTERFACE_GPIO_H_
#define STM32F7_INTERFACE_GPIO_H_
/*#####################################################*/
#include <stdbool.h>
/*#####################################################*/
#define HAVE_GPIO_AIN
#define HAVE_GPIO_IN_FLOATING
#define HAVE_GPIO_IN_PULL_DOWN
#define HAVE_GPIO_IN_PULL_UP
#define HAVE_GPIO_OUT_OPEN_DRAIN
#define HAVE_GPIO_OUT_PUSH_PULL

#define GPIO_OPTIMIZED_FUNCTIONS true
/*#####################################################*/
//extern GPIO_TypeDef *GET_GPIO_PORT_BASE_ADDR[];
/*#####################################################*/
#define _GPIO_FAST_SET_PIN(base_addr, pin)\
		((GPIO_TypeDef*)base_addr)->ODR |= 1 << (pin)

#define _GPIO_FAST_CLEAR_PIN(base_addr, pin)\
		((GPIO_TypeDef*)base_addr)->ODR &= ~(1 << (pin))

#define _GPIO_FAST_WRITE_MULTI_PIN(base_addr, mask, pin_mask)\
		((GPIO_TypeDef*)base_addr)->ODR = (((GPIO_TypeDef*)base_addr)->ODR & ~mask) | ((pin_mask) & mask)
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "gpio_interface.c"
#endif

#endif /* STM32F7_INTERFACE_GPIO_H_ */
