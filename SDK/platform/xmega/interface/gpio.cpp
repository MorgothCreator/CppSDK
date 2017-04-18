/*
 * gpio.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: John Smith
 */
#include <string.h>
#include <avr/pgmspace.h>
#include <api/gpio.h>

const unsigned char BIT_MASK_TABLE [] PROGMEM = {
		0b00000001,
		0b00000010,
		0b00000100,
		0b00001000,
		0b00010000,
		0b00100000,
		0b01000000,
		0b10000000
};

PORT_t *GPIO_BASE_PTRS[] =
{
		&PORTA
#ifdef PORTB
		, PORTB
#else
		, NULL
#endif
#ifdef PORTC
		, &PORTC
#else
		, NULL
#endif
#ifdef PORTD
		, &PORTD
#else
		, NULL
#endif
#ifdef PORTE
		, &PORTE
#else
		, NULL
#endif
#ifdef PORTF
		, &PORTF
#else
		, NULL
#endif
#ifdef PORTG
		, &PORTG
#else
		, NULL
#endif
#ifdef PORTH
		, &PORTH
#else
		, NULL
#endif
#ifdef PORTI
		, &PORTI
#else
		, NULL
#endif
#ifdef PORTJ
		, &PORTJ
#else
		, NULL
#endif
#ifdef PORTK
		, &PORTK
#else
		, NULL
#endif
#ifdef PORTL
		,&PORTL
#else
		, NULL
#endif
#ifdef PORTM
		,&PORTM
#else
		, NULL
#endif
#ifdef PORTN
		,&PORTN
#else
		, NULL
#endif
#ifdef PORTO
		,&PORTO
#else
		, NULL
#endif
#ifdef PORTP
		,&PORTP
#else
		, NULL
#endif
#ifdef PORTR
		,&PORTR
#else
		, NULL
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
	setMode(CfgGpio::GPIO_IN_FLOATING);
}
/*#####################################################*/
SysErr GI::Dev::Gpio::setOut(unsigned int value)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
	PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.pin >> 5];
	if (cfg.multiPin)
	{
		BaseAddr->OUT = (BaseAddr->OUT & ~(cfg.pin % 8))
				| (value & (cfg.pin % 8));
	}
	else
	{
		unsigned int state = value;
		if (cfg.reverse)
			state = (~state) & 0x01;
		if (state)
			BaseAddr->OUTSET = pgm_read_byte(&BIT_MASK_TABLE[cfg.pin % 8]);
		else
			BaseAddr->OUTCLR = pgm_read_byte(&BIT_MASK_TABLE[cfg.pin % 8]);
	}
	return SYS_ERR_OK;
}
/*#####################################################*/
signed int GI::Dev::Gpio::in()
{
	if (!this)
		return -1;
	PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.pin >> 5];
	if (cfg.multiPin)
	{
		return BaseAddr->IN & (cfg.pin % 8);
	}
	else
	{
		if (cfg.reverse)
		{
			if(BaseAddr->IN & pgm_read_byte(&BIT_MASK_TABLE[cfg.pin % 8]))
				return false;
			else
				return true;
		}
		else
			return BaseAddr->IN & pgm_read_byte(&BIT_MASK_TABLE[cfg.pin % 8]);
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
	PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.pin >> 5];

	/*if (cfg.multiPin == false)
		GPIO_InitStructure.Pin = 1 << (cfg.pin % 32);
	else
		GPIO_InitStructure.Pin = (cfg.pin % 32);*/
	volatile unsigned char *ctl_pin = &BaseAddr->PIN0CTRL;

	switch (mode)
	{
	case CfgGpio::GPIO_IN_PULL_UP:
		ctl_pin[cfg.pin % 8] = PORT_OPC_PULLUP_gc;
		BaseAddr->DIRCLR = pgm_read_byte(&BIT_MASK_TABLE[cfg.pin % 8]);
		break;
	case CfgGpio::GPIO_IN_PULL_DOWN:
		ctl_pin[cfg.pin % 8] = PORT_OPC_PULLDOWN_gc;
		BaseAddr->DIRCLR = pgm_read_byte(&BIT_MASK_TABLE[cfg.pin % 8]);
		break;
	case CfgGpio::GPIO_IN_FLOATING:
		ctl_pin[cfg.pin % 8] = PORT_OPC_TOTEM_gc;
		BaseAddr->DIRCLR = pgm_read_byte(&BIT_MASK_TABLE[cfg.pin % 8]);
		break;
	case CfgGpio::GPIO_OUT_PUSH_PULL:
		ctl_pin[cfg.pin % 8] = PORT_OPC_TOTEM_gc;
		BaseAddr->DIRSET = pgm_read_byte(&BIT_MASK_TABLE[cfg.pin % 8]);
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


