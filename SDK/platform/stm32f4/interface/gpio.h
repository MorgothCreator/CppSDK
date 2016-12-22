/*
 * gpio_interface.hpp
 *
 *  Created on: Dec 9, 2016
 *      Author: John Smith
 */

#ifndef STM32F7_INTERFACE_GPIO_H_
#define STM32F7_INTERFACE_GPIO_H_

/*#####################################################*/
#include <api/gpio_def.h>
#include <stdbool.h>
#include "driver/stm32f4xx_hal_gpio.h"
#include <include/global.h>

extern GPIO_TypeDef *GET_GPIO_PORT_BASE_ADDR[];
/*#####################################################*/
#define _GPIO_FAST_SET_PIN(base_addr, pin)\
		((GPIO_TypeDef*)base_addr)->ODR |= 1 << (pin)

#define _GPIO_FAST_CLEAR_PIN(base_addr, pin)\
		((GPIO_TypeDef*)base_addr)->ODR &= ~(1 << (pin))

#define _GPIO_FAST_WRITE_MULTI_PIN(base_addr, mask, pin_mask)\
		((GPIO_TypeDef*)base_addr)->ODR = (((GPIO_TypeDef*)base_addr)->ODR & ~mask) | ((pin_mask) & mask)

/*#####################################################*/
namespace GI
{
namespace Dev
{
class Gpio
{
public:
	Gpio(unsigned int pin, CfgGpio::gpioMode mode, bool multiPin);
	Gpio(CfgGpio *gpioPin);
	~Gpio();
	bool setOut(unsigned int value);
	signed int in();
	bool setMode(unsigned char mode);
	bool getState();
	void idle();
	SysErr err;

	CfgGpio cfg;

	bool lastState;
	struct
	{
		struct
		{
			void (*onStateChanged)(void *param, bool state);
			void *onStateChangedHandler;
			void (*onUp)(void *param);
			void *onUpHandler;
			void (*onDown)(void *param);
			void *onDownHandler;
		} callback;
		bool stateUp;
		bool stateDown;
		bool stateChanged;
	}events;
};
}
}
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "gpio_interface.c"
#endif

#endif /* STM32F7_INTERFACE_GPIO_H_ */
