/*
 * defs.cpp
 */
#if defined(FeatherAdaloggerM0)

#include "defs.h"
#include <stdio.h>

#include <api/init.h>
#include <api/uart.h>
#include <api/gpio.h>
#include <api/spi.h>
#include <api/i2c.h>

ioSetCfgGpio(led_0, 	"led-0",	pin_mux_convert_to_pin(IOA, 17),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 0);
ioSetCfgGpio(led_1, 	"led-1",	pin_mux_convert_to_pin(IOA,  6),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 0);
//ioSetCfgGpio(hbtn_0, 	"hbtn-0",	pin_mux_convert_to_pin(IOB,  4),CfgGpio::GPIO_IN_FLOATING, 		false, true, 0);

ioSetCfgUart(uart_5, "uart-5", pin_mux_convert_to_pin(IOA, 0)	, pin_mux_convert_to_pin(IOA, 0)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);

//ioSetCfgI2c(i2c_0, "i2c-0", pin_mux_convert_to_pin(IOB, 8), pin_mux_convert_to_pin(IOB, 9), CfgI2c::i2cMode0, 100000);

//ioSetCfgSpi(spi_0_0, "spi-0.0", pin_mux_convert_to_pin(IOA,  5), pin_mux_convert_to_pin(IOB,  5), pin_mux_convert_to_pin(IOB,  4), pin_mux_convert_to_pin(IOA, 15), CfgSpi::spiMode0, 500000, false);

ioSetCfgEnd(ioTableEnd);

ioSettings *ioSetCfg[] = {
	&led_0,
	&led_1,
	//hbtn_0,
	&uart_5,
	//i2c_0,
	//spi_0_0,
	&ioTableEnd,
	(ioSettings *)-1/* This is an artifice to force linker to put this structure ito .data section, to be secure that this will be initiated before the call of init() function. */
};

#endif /*!FRDM_KEAZ*/
