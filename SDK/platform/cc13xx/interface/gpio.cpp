/*
 * gpio_interface.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: Iulian Gheorghiu
 */
#include <api/gpio.h>
#include <stdlib.h>
#include <string.h>

#include "driver/ioc.h"
#include "inc/hw_ioc.h"
#include "inc/hw_types.h"
#include "driver/prcm.h"
#include "driver/sys_ctrl.h"

extern CfgGpio gpioCfg[];

/*#####################################################*/
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
    baseAddr = (void *)GPIO_BASE;
}
/*#####################################################*/
GI::Dev::Gpio::~Gpio()
{
    setMode(CfgGpio::GPIO_IN_FLOATING, multiPinMask);
}
/*#####################################################*/
/*#####################################################*/
SysErr GI::Dev::Gpio::setOut(unsigned int value)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
    CfgGpio *int_cfg = (CfgGpio *)cfg->cfg;
	if (int_cfg->multiPin)
	{
		HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = (HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) & ~(int_cfg->pin % 32))
				| (value & (int_cfg->pin % 32));
	}
	else
	{
		unsigned int state = value;
		if (int_cfg->reverse)
			state = (~state) & 0x01;
		if(state)
			HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << (int_cfg->pin % 32);
		else
			HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << (int_cfg->pin % 32);
	}
	return SYS_ERR_OK;
}
/*#####################################################*/
signed int GI::Dev::Gpio::in()
{
	if (!this)
		return -1;
    CfgGpio *int_cfg = (CfgGpio *)cfg->cfg;
	if (int_cfg->multiPin)
	{
		return HWREG(GPIO_BASE + GPIO_O_DIN31_0) & (int_cfg->pin % 32);
	}
	else
	{
		if (int_cfg->reverse)
		{
			if(HWREG(GPIO_BASE + GPIO_O_DIN31_0) & (1 << (int_cfg->pin % 32)))
				return false;
			else
				return true;
		}
		else
			return (HWREG(GPIO_BASE + GPIO_O_DIN31_0) >> (int_cfg->pin % 32)) & 0x01;
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
    if (int_cfg->multiPin)
        return SYS_ERR_NOT_IMPLEMENTED;;
    multiPinMask = 1 << (int_cfg->pin % 32);
	unsigned long pin_func = 0;
	switch (mode)
	{
	case CfgGpio::GPIO_IN_FLOATING:
		pin_func = (IOC_CURRENT_8MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->pin % 32));
		break;
	case CfgGpio::GPIO_IN_PULL_DOWN:
		pin_func = (IOC_CURRENT_8MA | IOC_STRENGTH_AUTO | IOC_IOPULL_DOWN | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->pin % 32));
		break;
	case CfgGpio::GPIO_IN_PULL_UP:
		pin_func = (IOC_CURRENT_8MA | IOC_STRENGTH_AUTO | IOC_IOPULL_UP | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->pin % 32));
		break;
	case CfgGpio::GPIO_OUT_OPEN_DRAIN:
		pin_func = (IOC_CURRENT_8MA | IOC_STRENGTH_AUTO | IOC_IOPULL_UP | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_OPEN_DRAIN_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= 1 << (int_cfg->pin % 32);
		break;
	case CfgGpio::GPIO_OUT_PUSH_PULL:
		pin_func = (IOC_CURRENT_8MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= 1 << (int_cfg->pin % 32);
		break;
		break;
	default:
		return SYS_ERR_INVALID_COMMAND;
	}
    IOCPortConfigureSet((int_cfg->pin % 32), IOC_PORT_GPIO, pin_func);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Gpio::setMode(CfgGpio::gpioMode_e mode, unsigned int mask)
{
    if (!this)
        return SYS_ERR_INVALID_HANDLER;
    CfgGpio *int_cfg = (CfgGpio *)cfg->cfg;
    if (!int_cfg->multiPin)
        return SYS_ERR_NOT_IMPLEMENTED;
    multiPinMask = mask;
    unsigned long pin_func = 0;
    switch (mode)
    {
    case CfgGpio::GPIO_IN_FLOATING:
        pin_func = (IOC_CURRENT_8MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
        HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~mask;
        break;
    case CfgGpio::GPIO_IN_PULL_DOWN:
        pin_func = (IOC_CURRENT_8MA | IOC_STRENGTH_AUTO | IOC_IOPULL_DOWN | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
        HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~mask;
        break;
    case CfgGpio::GPIO_IN_PULL_UP:
        pin_func = (IOC_CURRENT_8MA | IOC_STRENGTH_AUTO | IOC_IOPULL_UP | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
        HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~mask;
        break;
    case CfgGpio::GPIO_OUT_OPEN_DRAIN:
        pin_func = (IOC_CURRENT_8MA | IOC_STRENGTH_AUTO | IOC_IOPULL_UP | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_OPEN_DRAIN_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
        HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= mask;
        break;
    case CfgGpio::GPIO_OUT_PUSH_PULL:
        pin_func = (IOC_CURRENT_8MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
        HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= mask;
        break;
        break;
    default:
        return SYS_ERR_INVALID_COMMAND;
    }
    unsigned int cnt_pins = 0;
    for(; cnt_pins < 32; cnt_pins++)
    {
        if(mask & (1 << cnt_pins))
        {
            IOCPortConfigureSet(cnt_pins, IOC_PORT_GPIO, pin_func);
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
