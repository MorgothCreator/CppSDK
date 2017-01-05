/*
 * gpio.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: John Smith
 */
#include <string.h>
#include <api/gpio.h>
#include <driver/gpio.h>

/*#####################################################*/
GI::Dev::Gpio::Gpio(unsigned int pin, CfgGpio::gpioMode_e mode, bool multiPin)
{
	memset(this, 0, sizeof(*this));
	cfg.pin = pin;
	cfg.gpioMode = mode;
	cfg.multiPin = multiPin;
	//setMode(mode);
}

GI::Dev::Gpio::Gpio(CfgGpio *gpioPin)
{
	memset(this, 0, sizeof(*this));
	memcpy(&this->cfg, gpioPin, sizeof(CfgGpio));
	//setMode(cfg.gpioMode);
}

/*#####################################################*/
GI::Dev::Gpio::~Gpio()
{
	//HAL_GPIO_DeInit((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.pin >> 5],
			//(unsigned int) (1 << (cfg.pin % 32)));
}
/*#####################################################*/
SysErr GI::Dev::Gpio::setOut(unsigned int value)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
	GPIO_Type *Addr[] = GPIO_BASE_PTRS;
	GPIO_Type *BaseAddr = Addr[cfg.pin >> 5];
	if (cfg.multiPin)
	{
		BaseAddr->PDDR = (BaseAddr->PDDR & ~(cfg.pin % 32))
				| (value & (cfg.pin % 32));
	}
	else
	{
		unsigned int state = value;
		if (cfg.reverse)
			state = (~state) & 0x01;
		if (state)
			BaseAddr->PDDR |= 1 << (cfg.pin % 32);
		else
			BaseAddr->PDDR &= ~(1 << (cfg.pin % 32));
	}
	return SYS_ERR_OK;
}
/*#####################################################*/
signed int GI::Dev::Gpio::in()
{
	if (!this)
		return -1;
	GPIO_Type *Addr[] = GPIO_BASE_PTRS;
	GPIO_Type *BaseAddr = Addr[cfg.pin >> 5];
	if (cfg.multiPin)
	{
		return BaseAddr->PDIR & (cfg.pin % 32);
	}
	else
	{
		if (cfg.reverse)
		{
			if(READ_INPUT(BaseAddr, cfg.pin % 32))
				return false;
			else
				return true;
		}
		else
			return READ_INPUT(BaseAddr, cfg.pin % 32);
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
	GPIO_Type *Addr[] = GPIO_BASE_PTRS;
	GPIO_Type *BaseAddr = Addr[cfg.pin >> 5];

	/*if (cfg.multiPin == false)
		GPIO_InitStructure.Pin = 1 << (cfg.pin % 32);
	else
		GPIO_InitStructure.Pin = (cfg.pin % 32);*/
	if(mode == CfgGpio::GPIO_IN_PULL_UP)
	{
		switch(cfg.pin >> 5)
		{
		case 0:
			PORT_PUE0 |= 1 << (cfg.pin % 32);
			break;
		case 1:
			PORT_PUE1 |= 1 << (cfg.pin % 32);
			break;
		case 2:
			PORT_PUE2 |= 1 << (cfg.pin % 32);
			break;
		}
	}
	else
	{
		switch(cfg.pin >> 5)
		{
		case 0:
			PORT_PUE0 &= ~(1 << (cfg.pin % 32));
			break;
		case 1:
			PORT_PUE1 &= ~(1 << (cfg.pin % 32));
			break;
		case 2:
			PORT_PUE2 &= ~(1 << (cfg.pin % 32));
			break;
		}
	}
	switch (mode)
	{
	case CfgGpio::GPIO_IN_FLOATING:
		ENABLE_INPUT(BaseAddr, cfg.pin % 32);
		CONFIG_PIN_AS_GPIO(BaseAddr, cfg.pin % 32, INPUT);

		break;
	case CfgGpio::GPIO_OUT_PUSH_PULL:
		CONFIG_PIN_AS_GPIO(BaseAddr, cfg.pin % 32, OUTPUT);
		DISABLE_INPUT(BaseAddr, cfg.pin % 32);
		break;
	default:
		return SYS_ERR_INVALID_COMMAND;

	}
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


