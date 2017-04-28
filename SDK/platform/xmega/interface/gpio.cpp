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
		, &PORTB
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
#ifdef PORTQ
		,&PORTQ
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
	pinNr = int_cfg->pin % 8;
	baseAddr = (void *)GPIO_BASE_PTRS[int_cfg->pin >> 5];
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
	volatile PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->pin >> 5];
	if (int_cfg->multiPin)
	{
		BaseAddr->OUT = (BaseAddr->OUT & ~(int_cfg->pin % 8))
				| (value & (int_cfg->pin % 8));
	}
	else
	{
		unsigned int state = value;
		if (int_cfg->reverse)
			state = (~state) & 0x01;
		if (state)
			BaseAddr->OUTSET = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->pin % 8]);
		else
			BaseAddr->OUTCLR = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->pin % 8]);
	}
	return SYS_ERR_OK;
}
/*#####################################################*/
signed int GI::Dev::Gpio::in()
{
	if (!this)
		return -1;
	CfgGpio *int_cfg = (CfgGpio *)cfg->cfg;
	volatile PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->pin >> 5];
	if (int_cfg->multiPin)
	{
		return BaseAddr->IN & (int_cfg->pin % 8);
	}
	else
	{
		if (int_cfg->reverse)
		{
			if(BaseAddr->IN & pgm_read_byte(&BIT_MASK_TABLE[int_cfg->pin % 8]))
				return false;
			else
				return true;
		}
		else
			return BaseAddr->IN & pgm_read_byte(&BIT_MASK_TABLE[int_cfg->pin % 8]);
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
	volatile PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->pin >> 5];

	if(int_cfg->multiPin)
		return SYS_ERR_INVALID_COMMAND;
	multiPinMask = 1 << (int_cfg->pin % 8);
	
	volatile unsigned char *ctl_pin = &BaseAddr->PIN0CTRL;
	switch (mode)
	{
	case CfgGpio::GPIO_IN_PULL_UP:
		ctl_pin[int_cfg->pin % 8] = PORT_OPC_PULLUP_gc;
		BaseAddr->DIRCLR = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->pin % 8]);
		break;
	case CfgGpio::GPIO_IN_PULL_DOWN:
		ctl_pin[int_cfg->pin % 8] = PORT_OPC_PULLDOWN_gc;
		BaseAddr->DIRCLR = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->pin % 8]);
		break;
	case CfgGpio::GPIO_IN_FLOATING:
		ctl_pin[int_cfg->pin % 8] = PORT_OPC_TOTEM_gc;
		BaseAddr->DIRCLR = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->pin % 8]);
		break;
	case CfgGpio::GPIO_OUT_PUSH_PULL:
		ctl_pin[int_cfg->pin % 8] = PORT_OPC_TOTEM_gc;
		BaseAddr->DIRSET = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->pin % 8]);
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
	volatile PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->pin >> 5];
    multiPinMask = mask;

	volatile unsigned char *ctl_pin = &BaseAddr->PIN0CTRL;
	unsigned int cnt_pins = 0;
	for(; cnt_pins < 8; cnt_pins++)
	{
		if(mask & BIT_MASK_TABLE[cnt_pins])
		{
			switch (mode)
			{
			case CfgGpio::GPIO_IN_PULL_UP:
				ctl_pin[cnt_pins] = PORT_OPC_PULLUP_gc;
				BaseAddr->DIRCLR = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->pin % 8]);
				break;
			case CfgGpio::GPIO_IN_PULL_DOWN:
				ctl_pin[cnt_pins] = PORT_OPC_PULLDOWN_gc;
				BaseAddr->DIRCLR = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->pin % 8]);
				break;
			case CfgGpio::GPIO_IN_FLOATING:
				ctl_pin[cnt_pins] = PORT_OPC_TOTEM_gc;
				BaseAddr->DIRCLR = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->pin % 8]);
				break;
			case CfgGpio::GPIO_OUT_PUSH_PULL:
				ctl_pin[cnt_pins] = PORT_OPC_TOTEM_gc;
				BaseAddr->DIRSET = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->pin % 8]);
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


