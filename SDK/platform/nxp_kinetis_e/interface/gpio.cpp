/*
 * gpio.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: John Smith
 */
#include <string.h>
#include <api/gpio.h>
#include <driver/gpio.h>
#include "gpio.h"

/*#####################################################*/
GI::Dev::Gpio::Gpio(ioSettings *cfg)
{
    memset(this, 0, sizeof(*this));
    if(cfg->info.ioType != ioSettings::info_s::ioType_GPIO)
        return;
    this->cfg = cfg;
    CfgGpio *int_cfg = (CfgGpio *)cfg->cfg;
    if(!int_cfg->multiPin)
    {
        setMode(int_cfg->gpioMode);
        setOut(int_cfg->defValue);
    }
    pinNr = int_cfg->pin % 32;
	GPIO_Type *Addr[] = GPIO_BASE_PTRS;
    baseAddr = (void *)Addr[int_cfg->pin >> 5];
}
/*#####################################################*/
GI::Dev::Gpio::~Gpio()
{
	setMode(CfgGpio::GPIO_IN_FLOATING, multiPinMask);
}
/*#####################################################*/
SysErr GI::Dev::Gpio::setOut(unsigned int value)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
	CfgGpio *int_cfg = (CfgGpio *)cfg->cfg;
	GPIO_Type *Addr[] = GPIO_BASE_PTRS;
	GPIO_Type *BaseAddr = Addr[int_cfg->pin >> 5];
	if (int_cfg->multiPin)
	{
		BaseAddr->PDDR = (BaseAddr->PDDR & ~(int_cfg->pin % 32))
				| (value & (int_cfg->pin % 32));
	}
	else
	{
		unsigned int state = value;
		if (int_cfg->reverse)
			state = (~state) & 0x01;
		if (state)
			BaseAddr->PSOR |= 1 << (int_cfg->pin % 32);
		else
			BaseAddr->PCOR |= 1 << (int_cfg->pin % 32);
	}
	return SYS_ERR_OK;
}
/*#####################################################*/
signed int GI::Dev::Gpio::in()
{
	if (!this)
		return -1;
	CfgGpio *int_cfg = (CfgGpio *)cfg->cfg;
	GPIO_Type *Addr[] = GPIO_BASE_PTRS;
	GPIO_Type *BaseAddr = Addr[int_cfg->pin >> 5];
	if (int_cfg->multiPin)
	{
		return BaseAddr->PDIR & (int_cfg->pin % 32);
	}
	else
	{
		if (int_cfg->reverse)
		{
			if(READ_INPUT(BaseAddr, int_cfg->pin % 32))
				return false;
			else
				return true;
		}
		else
			return READ_INPUT(BaseAddr, int_cfg->pin % 32);
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
	CfgGpio *int_cfg = (CfgGpio *)cfg->cfg;
	if(int_cfg->multiPin)
		return SYS_ERR_INVALID_COMMAND;
	GPIO_Type *Addr[] = GPIO_BASE_PTRS;
	GPIO_Type *BaseAddr = Addr[int_cfg->pin >> 5];
	multiPinMask = 1 << (int_cfg->pin % 32);

	if(mode == CfgGpio::GPIO_IN_PULL_UP)
	{
		switch(int_cfg->pin >> 5)
		{
		case 0:
			PORT_PUE0 |= 1 << (int_cfg->pin % 32);
			break;
		case 1:
			PORT_PUE1 |= 1 << (int_cfg->pin % 32);
			break;
		case 2:
			PORT_PUE2 |= 1 << (int_cfg->pin % 32);
			break;
		}
	}
	else
	{
		switch(int_cfg->pin >> 5)
		{
		case 0:
			PORT_PUE0 &= ~(1 << (int_cfg->pin % 32));
			break;
		case 1:
			PORT_PUE1 &= ~(1 << (int_cfg->pin % 32));
			break;
		case 2:
			PORT_PUE2 &= ~(1 << (int_cfg->pin % 32));
			break;
		}
	}
	switch (mode)
	{
	case CfgGpio::GPIO_IN_FLOATING:
		ENABLE_INPUT(BaseAddr, int_cfg->pin % 32);
		CONFIG_PIN_AS_GPIO(BaseAddr, int_cfg->pin % 32, INPUT);

		break;
	case CfgGpio::GPIO_OUT_PUSH_PULL:
		CONFIG_PIN_AS_GPIO(BaseAddr, int_cfg->pin % 32, OUTPUT);
		ENABLE_INPUT(BaseAddr, int_cfg->pin % 32);
		break;
	default:
		return SYS_ERR_INVALID_COMMAND;

	}
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Gpio::setMode(CfgGpio::gpioMode_e mode, unsigned int mask)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
	CfgGpio *int_cfg = (CfgGpio *)cfg->cfg;
	if(!int_cfg->multiPin)
		return SYS_ERR_INVALID_COMMAND;
	GPIO_Type *Addr[] = GPIO_BASE_PTRS;
	GPIO_Type *BaseAddr = Addr[int_cfg->pin >> 5];
	multiPinMask = mask;

	unsigned int cnt_pins = 0;
	for(; cnt_pins < 32; cnt_pins++)
	{
		if(mask & 1 << cnt_pins)
		{
			if(mode == CfgGpio::GPIO_IN_PULL_UP)
			{
				switch(int_cfg->pin >> 5)
				{
				case 0:
					PORT_PUE0 |= 1 << cnt_pins;
					break;
				case 1:
					PORT_PUE1 |= 1 << cnt_pins;
					break;
				case 2:
					PORT_PUE2 |= 1 << cnt_pins;
					break;
				}
			}
			else
			{
				switch(int_cfg->pin >> 5)
				{
				case 0:
					PORT_PUE0 &= ~(1 << cnt_pins);
					break;
				case 1:
					PORT_PUE1 &= ~(1 << cnt_pins);
					break;
				case 2:
					PORT_PUE2 &= ~(1 << cnt_pins);
					break;
				}
			}
			switch (mode)
			{
			case CfgGpio::GPIO_IN_FLOATING:
				ENABLE_INPUT(BaseAddr, cnt_pins);
				CONFIG_PIN_AS_GPIO(BaseAddr, cnt_pins, INPUT);

				break;
			case CfgGpio::GPIO_OUT_PUSH_PULL:
				CONFIG_PIN_AS_GPIO(BaseAddr, cnt_pins, OUTPUT);
				ENABLE_INPUT(BaseAddr, cnt_pins);
				break;
			default:
				return SYS_ERR_INVALID_COMMAND;

			}
		}
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


