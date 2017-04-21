/*
 * gpio.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: John Smith
 */
#include <string.h>
#include <api/gpio.h>
#include <driver/gpio.h>

/*PORT_t *GPIO_BASE_PTRS[] =
{
		&PORTA
#ifdef PORTB
		, PORTB
#endif
#ifdef PORTC
		, &PORTC
#endif
#ifdef PORTD
		, &PORTD
#endif
#ifdef PORTE
		, &PORTE
#endif
#ifdef PORTF
		, &PORTF
#endif
#ifdef PORTG
		, &PORTG
#endif
#ifdef PORTH
		, &PORTH
#endif
#ifdef PORTI
		, &PORTI
#endif
#ifdef PORTJ
		, &PORTJ
#endif
#ifdef PORTK
		, &PORTK
#endif
#ifdef PORTL
		,&PORTL
#endif
#ifdef PORTM
		,&PORTM
#endif
#ifdef PORTN
		,&PORTN
#endif
#ifdef PORTO
		,&PORTO
#endif
#ifdef PORTP
		,&PORTP
#endif
#ifdef PORTR
		,&PORTR
#endif
};*/
/*#####################################################*/
GI::Dev::Gpio::Gpio(unsigned int pin, CfgGpio::gpioMode_e mode, bool multiPin)
{
	memset(this, 0, sizeof(*this));
	cfg.pin = pin;
	cfg.gpioMode = mode;
	cfg.multiPin = multiPin;
	setMode(mode);
	setOut(cfg.defValue);
}

GI::Dev::Gpio::Gpio(CfgGpio *gpioPin)
{
	memset(this, 0, sizeof(*this));
	memcpy(&this->cfg, gpioPin, sizeof(CfgGpio));
	setMode(cfg.gpioMode);
	setOut(cfg.defValue);
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
	//PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.pin >> 5];
	if (cfg.multiPin)
	{
		//BaseAddr->OUT = (BaseAddr->OUT & ~(cfg.pin % 32))
				//| (value & (cfg.pin % 32));
	}
	else
	{
		unsigned int state = value;
		if (cfg.reverse)
			state = (~state) & 0x01;
		port_pin_set_output_level(cfg.pin, state);
	}
	return SYS_ERR_OK;
}
/*#####################################################*/
signed int GI::Dev::Gpio::in()
{
	if (!this)
		return -1;
	//PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.pin >> 5];
	if (cfg.multiPin)
	{
		return false;//BaseAddr->DIR & (cfg.pin % 32);
	}
	else
	{
		if (cfg.reverse)
		{
			if(port_pin_get_input_level(cfg.pin))
				return false;
			else
				return true;
		}
		else
			return port_pin_get_input_level(cfg.pin);
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
	port_pin_set_config(cfg.pin, &config_port_pin);
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


