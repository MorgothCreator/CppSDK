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
GI::Dev::Gpio::Gpio(ioSettings *cfg)
{
	memset(this, 0, sizeof(*this));
	if(cfg->info.ioType != ioSettings::info_s::ioType_GPIO)
		return;
	this->cfg = cfg;
	CfgGpio *int_cfg = (CfgGpio *)cfg->cfg;
	setMode(int_cfg->gpioMode);
	setOut(int_cfg->defValue);
}

/*#####################################################*/
GI::Dev::Gpio::~Gpio()
{
	//HAL_GPIO_DeInit((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->pin >> 5],
			//(unsigned int) (1 << (int_cfg->pin % 32)));
}
/*#####################################################*/
SysErr GI::Dev::Gpio::setOut(unsigned int value)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
	CfgGpio *int_cfg = (CfgGpio *)cfg->cfg;
	if (int_cfg->multiPin)
	{
		//BaseAddr->OUT = (BaseAddr->OUT & ~(int_cfg->pin % 32))
				//| (value & (int_cfg->pin % 32));
	}
	else
	{
		unsigned int state = value;
		if (int_cfg->reverse)
			state = (~state) & 0x01;
		port_pin_set_output_level(int_cfg->pin, state);
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
		return false;//BaseAddr->DIR & (int_cfg->pin % 32);
	}
	else
	{
		if (int_cfg->reverse)
		{
			if(port_pin_get_input_level(int_cfg->pin))
				return false;
			else
				return true;
		}
		else
			return port_pin_get_input_level(int_cfg->pin);
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
	struct port_config config_port_pin;
	
	switch (mode)
	{
	case CfgGpio::GPIO_IN_PULL_UP:
		config_port_pin.direction  = PORT_PIN_DIR_INPUT;
		config_port_pin.input_pull = PORT_PIN_PULL_UP;
		break;
	case CfgGpio::GPIO_IN_PULL_DOWN:
		config_port_pin.direction  = PORT_PIN_DIR_INPUT;
		config_port_pin.input_pull = PORT_PIN_PULL_DOWN;
		break;
	case CfgGpio::GPIO_IN_FLOATING:
		config_port_pin.direction  = PORT_PIN_DIR_INPUT;
		config_port_pin.input_pull = PORT_PIN_PULL_NONE;
		break;
	case CfgGpio::GPIO_OUT_PUSH_PULL:
		config_port_pin.direction  = PORT_PIN_DIR_OUTPUT_WTH_READBACK;
		config_port_pin.input_pull = PORT_PIN_PULL_NONE;
		break;
	default:
		return SYS_ERR_INVALID_COMMAND;
	}
	port_pin_set_config(int_cfg->pin, &config_port_pin);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Gpio::setModeMultipin(CfgGpio::gpioMode_e mode, unsigned int mask)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
	CfgGpio *int_cfg = (CfgGpio *)cfg->cfg;
	if(!int_cfg->multiPin)
		return SYS_ERR_INVALID_COMMAND;
	struct port_config config_port_pin;
	
	switch (mode)
	{
	case CfgGpio::GPIO_IN_PULL_UP:
		config_port_pin.direction  = PORT_PIN_DIR_INPUT;
		config_port_pin.input_pull = PORT_PIN_PULL_UP;
		break;
	case CfgGpio::GPIO_IN_PULL_DOWN:
		config_port_pin.direction  = PORT_PIN_DIR_INPUT;
		config_port_pin.input_pull = PORT_PIN_PULL_DOWN;
		break;
	case CfgGpio::GPIO_IN_FLOATING:
		config_port_pin.direction  = PORT_PIN_DIR_INPUT;
		config_port_pin.input_pull = PORT_PIN_PULL_NONE;
		break;
	case CfgGpio::GPIO_OUT_PUSH_PULL:
		config_port_pin.direction  = PORT_PIN_DIR_OUTPUT_WTH_READBACK;
		config_port_pin.input_pull = PORT_PIN_PULL_NONE;
		break;
	default:
		return SYS_ERR_INVALID_COMMAND;
	}
	unsigned int cnt_pins = 0;
	for(; cnt_pins < 32; cnt_pins++)
	{
		if(mask & 1 << cnt_pins)
		{
			port_pin_set_config((int_cfg->pin & ~0x3F) + cnt_pins, &config_port_pin);
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


