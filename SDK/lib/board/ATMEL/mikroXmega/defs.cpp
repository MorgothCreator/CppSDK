/*
 * defs.cpp
 */
#if defined(mikroXmega)

#if __AVR_XMEGA__
#include <avr/pgmspace.h>
#endif

#include "defs.h"
#include <stdio.h>

#include <api/gpio.h>
#include <api/spi.h>
#include <api/i2c.h>
#include <api/uart.h>

ioSetCfgGpio(rf_dio_1, 		"rf_dio_1",		pin_mux_convert_to_pin(IOB,  0),CfgGpio::GPIO_IN_PULL_UP, 		false,	false,	0);
ioSetCfgGpio(rf_dio_2, 		"rf_dio_2",		pin_mux_convert_to_pin(IOB,  1),CfgGpio::GPIO_IN_PULL_UP, 		false,	false,	0);
ioSetCfgGpio(rf_dio_3, 		"rf_dio_3",		pin_mux_convert_to_pin(IOB,  2),CfgGpio::GPIO_IN_PULL_UP, 		false,	false,	0);
ioSetCfgGpio(rf_dio_4, 		"rf_dio_4",		pin_mux_convert_to_pin(IOB,  3),CfgGpio::GPIO_IN_PULL_UP, 		false,	false,	0);
ioSetCfgGpio(rf_dio_5, 		"rf_dio_5",		pin_mux_convert_to_pin(IOB,  4),CfgGpio::GPIO_IN_PULL_UP, 		false,	false,	0);
ioSetCfgGpio(rf_rst, 		"rf_rst",		pin_mux_convert_to_pin(IOE,  2),CfgGpio::GPIO_OUT_PUSH_PULL, 	false,	false,	0);
ioSetCfgGpio(rf_irq, 		"rf_irq",		pin_mux_convert_to_pin(IOE,  3),CfgGpio::GPIO_IN_PULL_UP, 		false,	false,	0);

ioSetCfgUart(uart_0, "uart-0", pin_mux_convert_to_pin(IOC, 3)	, pin_mux_convert_to_pin(IOC, 2)	, 2000000	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);

//ioSetCfgI2c(i2c_0, "i2c-0", pin_mux_convert_to_pin(IOB, 8), pin_mux_convert_to_pin(IOB, 9), 100000);

ioSetCfgSpi(spi_2_0, "spi-2.0", pin_mux_convert_to_pin(IOE,  5), pin_mux_convert_to_pin(IOE,  7), pin_mux_convert_to_pin(IOE,  6), pin_mux_convert_to_pin(IOE, 4), CfgSpi::spiMode0, 25000000, false);
ioSetCfgSpi(spi_3_0, "spi-3.0", pin_mux_convert_to_pin(IOF,  5), pin_mux_convert_to_pin(IOF,  7), pin_mux_convert_to_pin(IOF,  6), pin_mux_convert_to_pin(IOF, 4), CfgSpi::spiMode0, 25000000, false);

ioSetCfgEnd(ioTableEnd);

ioSettings *ioSetCfg[] = {
	&rf_dio_1,
	&rf_dio_2,
	&rf_dio_3,
	&rf_dio_4,
	&rf_dio_5,
	&rf_rst,
	&rf_irq,
	&uart_0,
	//&i2c_0,
	&spi_2_0,
	&spi_3_0,
	&ioTableEnd,
	(ioSettings *)-1/* This is an artifice to force linker to put this structure ito .data section, to be secure that this will be initiated before the call of init() function. */
};

#endif/*!FRDM_KEAZ*/
