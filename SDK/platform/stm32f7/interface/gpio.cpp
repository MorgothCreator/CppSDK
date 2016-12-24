/*
 * gpio_interface.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: Iulian Gheorghiu
 */
#include <api/gpio.h>
#include <interface/gpio.h>
#include <stdlib.h>
#include <string.h>
//#include "stm32f7xx_conf.h"
#include "include/stm32f7xx.h"
#include "driver/stm32f7xx_hal_gpio.h"
#include "driver/stm32f7xx_hal_rcc.h"
#include "sys/system_stm32f7xx.h"
#include "api/init.h"

GPIO_TypeDef *GET_GPIO_PORT_BASE_ADDR[] =
{
		GPIOA
#ifdef GPIOB
		, GPIOB
#endif
#ifdef GPIOC
		, GPIOC
#endif
#ifdef GPIOD
		, GPIOD
#endif
#ifdef GPIOE
		, GPIOE
#endif
#ifdef GPIOF
		, GPIOF
#endif
#ifdef GPIOG
		, GPIOG
#endif
#ifdef GPIOH
		, GPIOH
#endif
#ifdef GPIOI
		, GPIOI
#endif
#ifdef GPIOJ
		, GPIOJ
#endif
#ifdef GPIOK
		, GPIOK
#endif
#ifdef GPIOL
		,GPIOL
#endif
};
/*#####################################################*/
GI::Dev::Gpio::Gpio(unsigned int pin, CfgGpio::gpioMode_e mode, bool multiPin)
{
	memset(this, 0, sizeof(*this));
	cfg.pin = pin;
	cfg.gpioMode = mode;
	cfg.multiPin = multiPin;
	setMode(mode);
}

GI::Dev::Gpio::Gpio(CfgGpio *gpioPin)
{
	memset(this, 0, sizeof(*this));
	memcpy(&this->cfg, gpioPin, sizeof(CfgGpio));
	setMode(cfg.gpioMode);
}

/*#####################################################*/
GI::Dev::Gpio::~Gpio()
{
	HAL_GPIO_DeInit((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.pin >> 5],
			(unsigned int) (1 << (cfg.pin % 32)));
}
/*#####################################################*/
/*#####################################################*/
SysErr GI::Dev::Gpio::setOut(unsigned int value)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
	GPIO_TypeDef *BaseAddr = GET_GPIO_PORT_BASE_ADDR[cfg.pin >> 5];
	if (cfg.multiPin)
	{
		BaseAddr->ODR = (BaseAddr->ODR & ~(cfg.pin % 32))
				| (value & (cfg.pin % 32));
	}
	else
	{
		unsigned int state = value;
		if (cfg.reverse)
			state = (~state) & 0x01;
		if (state)
			BaseAddr->BSRR |= 1 << (cfg.pin % 32);
		else
			BaseAddr->BSRR |= 1 << ((cfg.pin % 32) + 16);
	}
	return SYS_ERR_OK;
}
/*#####################################################*/
signed int GI::Dev::Gpio::in()
{
	if (!this)
		return -1;
	GPIO_TypeDef *BaseAddr = GET_GPIO_PORT_BASE_ADDR[cfg.pin >> 5];
	if (cfg.multiPin)
	{
		return BaseAddr->IDR & (cfg.pin % 32);
	}
	else
	{
		if (cfg.reverse)
			return ~(HAL_GPIO_ReadPin(BaseAddr, (unsigned short)(1 << (cfg.pin % 32))));
		else
			return HAL_GPIO_ReadPin(BaseAddr, (unsigned short)(1 << (cfg.pin % 32)));
	}
}
/*#####################################################*/
SysErr GI::Dev::Gpio::getIn(unsigned long *value)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
	*value = (unsigned int)in();
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Gpio::setMode(CfgGpio::gpioMode_e mode)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
	GPIO_TypeDef *BaseAddr = GET_GPIO_PORT_BASE_ADDR[cfg.pin >> 5];

	GPIO_InitTypeDef GPIO_InitStructure;
	if (cfg.multiPin == false)
		GPIO_InitStructure.Pin = 1 << (cfg.pin % 32);
	else
		GPIO_InitStructure.Pin = (cfg.pin % 32);
	switch (mode)
	{
	case CfgGpio::GPIO_AIN:
		GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		break;
	case CfgGpio::GPIO_IN_FLOATING:
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		break;
	case CfgGpio::GPIO_IN_PULL_DOWN:
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		break;
	case CfgGpio::GPIO_IN_PULL_UP:
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		break;
	case CfgGpio::GPIO_OUT_OPEN_DRAIN:
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
		break;
	case CfgGpio::GPIO_OUT_PUSH_PULL:
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		break;
	default:
		return SYS_ERR_INVALID_COMMAND;

	}
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init((GPIO_TypeDef *) BaseAddr, &GPIO_InitStructure);
	return SYS_ERR_OK;
}
/*#####################################################*/
bool GI::Dev::Gpio::getState()
{
	return lastState;
}
/*#####################################################*/
void GI::Dev::Gpio::idle()
{
	if (in() == 0 && lastState == true)
	{
		lastState = false;
		events.stateDown = true;
		events.stateChanged = true;
		if (events.callback.onStateChanged)
			events.callback.onStateChanged(
					events.callback.onStateChangedHandler, false);
		if (events.callback.onDown)
			events.callback.onDown(
					events.callback.onDownHandler);
	}
	else if (in() != 0 && lastState == false)
	{
		lastState = true;
		events.stateUp = true;
		events.stateChanged = true;
		if (events.callback.onStateChanged)
			events.callback.onStateChanged(
					events.callback.onStateChangedHandler, true);
		if (events.callback.onUp)
			events.callback.onUp(
					events.callback.onUpHandler);
	}
}
/*#####################################################*/

