/*
 * defs.cpp
 */
#if defined(Xmega_E5_Xplained)

#if __AVR_XMEGA__
#include <avr/pgmspace.h>
#endif

#include "defs.h"
#include <stdio.h>

#include <api/gpio.h>
#include <api/spi.h>
#include <api/i2c.h>
#include <api/uart.h>

ioSetCfgGpio(led_0, 		"led-0",		pin_mux_convert_to_pin(IOD,  4),CfgGpio::GPIO_OUT_PUSH_PULL, 	false,	true,	0);
ioSetCfgGpio(led_1, 		"led-1",		pin_mux_convert_to_pin(IOD,  5),CfgGpio::GPIO_OUT_PUSH_PULL, 	false,	true,	0);
ioSetCfgGpio(hbtn_0, 		"hbtn-0",		pin_mux_convert_to_pin(IOD,  0),CfgGpio::GPIO_IN_FLOATING,		false,	true,	0);
ioSetCfgGpio(hbtn_1, 		"hbtn-1",		pin_mux_convert_to_pin(IOD,  2),CfgGpio::GPIO_IN_FLOATING,		false,	true,	0);

ioSetCfgGpio(ug2832_rst,	"ug2832-rst",	pin_mux_convert_to_pin(IOD,  3),CfgGpio::GPIO_OUT_PUSH_PULL, 	false,	false,	0);
ioSetCfgGpio(ug2832_cd,		"ug2832-cd",	pin_mux_convert_to_pin(IOR,  0),CfgGpio::GPIO_OUT_PUSH_PULL, 	false,	false,	0);

ioSetCfgUart(uart_3, "uart-3", pin_mux_convert_to_pin(IOA, 1)	, pin_mux_convert_to_pin(IOA, 1)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);

//ioSetCfgI2c(i2c_0, "i2c-0", pin_mux_convert_to_pin(IOB, 8), pin_mux_convert_to_pin(IOB, 9), CfgI2c::i2cMode0, 100000);

ioSetCfgSpi(spi_0_0, "spi-0.0", pin_mux_convert_to_pin(IOC,  5), pin_mux_convert_to_pin(IOC,  7), pin_mux_convert_to_pin(IOC,  6), pin_mux_convert_to_pin(IOR, 1), CfgSpi::spiMode0, 5000000, false);

ioSetCfgEnd(ioTableEnd);

ioSettings *ioSetCfg[] = {
	&led_0,
	&led_1,
	&hbtn_0,
	&hbtn_1,
	&ug2832_rst,
	&ug2832_cd,
	&uart_3,
	//&i2c_0,
	&spi_0_0,
	&ioTableEnd,
	(ioSettings *)-1/* This is an artifice to force linker to put this structure ito .data section, to be secure that this will be initiated before the call of init() function. */
};

#endif/*!FRDM_KEAZ*/
