/*
 * defs.cpp
 */
#if defined(FRDM_KEAZ128)

#include "defs.h"
#include <stdio.h>

#include <api/init.h>
#include <api/gpio.h>
#include <api/spi.h>
#include <api/i2c.h>
#include <api/uart.h>

ioSetCfgGpio(led_0, 	"led-0",	pin_mux_convert_to_pin(IOB, 24),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, true, 0);
ioSetCfgGpio(led_1, 	"led-1",	pin_mux_convert_to_pin(IOB, 25),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, true, 0);
ioSetCfgGpio(led_2, 	"led-2",	pin_mux_convert_to_pin(IOB,  7),CfgGpio::GPIO_IN_FLOATING, 		false, true, 0);
ioSetCfgGpio(hbtn_0, 	"hbtn-0",	pin_mux_convert_to_pin(IOB,  4),CfgGpio::GPIO_IN_FLOATING, 		false, true, 0);
ioSetCfgGpio(hbtn_1, 	"hbtn-1",	pin_mux_convert_to_pin(IOB,  5),CfgGpio::GPIO_IN_FLOATING, 		false, true, 0);

ioSetCfgUart(uart_2, "uart-2", pin_mux_convert_to_pin(IOD, 7)	, pin_mux_convert_to_pin(IOD, 6)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);

ioSetCfgI2c(i2c_0, "i2c-0", pin_mux_convert_to_pin(IOB, 8), pin_mux_convert_to_pin(IOB, 9), CfgI2c::i2cMode0, 100000);

ioSetCfgSpi(spi_0_0, "spi-0.0", pin_mux_convert_to_pin(IOA,  5), pin_mux_convert_to_pin(IOB,  5), pin_mux_convert_to_pin(IOB,  4), pin_mux_convert_to_pin(IOA, 15), CfgSpi::spiMode0, 500000, false);

ioSetCfgEnd(ioTableEnd);

ioSettings *ioSetCfg[] = {
		&led_0,
		&led_1,
		&led_2,
		&hbtn_0,
		&hbtn_1,
		&uart_2,
		&i2c_0,
		&spi_0_0,
		&ioTableEnd,
		(ioSettings *)-1/* This is an artifice to force linker to put this structure ito .data section, to be secure that this will be initiated before the call of init() function. */
};

#endif/*!FRDM_KEAZ*/
