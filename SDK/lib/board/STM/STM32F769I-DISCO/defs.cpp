/*
 * defs.cpp
 */
#if defined(STM32F769I_DISCO)

#include <board/STM/STM32F769I-DISCO/defs.h>
#include <stdio.h>

#include <api/init.h>
#include <api/gpio.h>
#include <api/spi.h>
#include <api/i2c.h>
#include <api/uart.h>

/* IO initialization table */
ioSetCfgGpio(led_0, 	"led-0",	pin_mux_convert_to_pin(IOJ, 13),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 0);
ioSetCfgGpio(led_1, 	"led-1",	pin_mux_convert_to_pin(IOJ,  5),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 0);
ioSetCfgGpio(hbtn_0, 	"hbtn-0",	pin_mux_convert_to_pin(IOA,  0),CfgGpio::GPIO_IN_PULL_UP, 		false, false, 0);
ioSetCfgGpio(bcklight, 	"bcklight",	pin_mux_convert_to_pin(IOI, 14),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(disprst, 	"disprst",	pin_mux_convert_to_pin(IOJ, 15),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(touchirq, 	"touchirq",	pin_mux_convert_to_pin(IOI, 13),CfgGpio::GPIO_IN_PULL_UP, 		false, false, 0);
ioSetCfgGpio(mmcsdcd, 	"mmcsdcd",	pin_mux_convert_to_pin(IOI, 15),CfgGpio::GPIO_IN_PULL_UP, 		false, false, 1);
ioSetCfgUart(uart_0, 	"uart-0", 	pin_mux_convert_to_pin(IOA, 9)	, 	pin_mux_convert_to_pin(IOA,10)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);
ioSetCfgUart(usbcdc_0, 	"usbcdc-0", pin_mux_convert_to_pin(IOA, 0)	, 	pin_mux_convert_to_pin(IOA, 0)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);
ioSetCfgI2c(i2c_0, 		"i2c-0", 	pin_mux_convert_to_pin(IOB, 8), 	pin_mux_convert_to_pin(IOB, 9), 100000);
ioSetCfgI2c(i2c_3, 		"i2c-3", 	pin_mux_convert_to_pin(IOD,12), 	pin_mux_convert_to_pin(IOB, 7), 100000);
ioSetCfgSpi(spi_1_0, 	"spi-1.0", 	pin_mux_convert_to_pin(IOA,12), 	pin_mux_convert_to_pin(IOB,15), pin_mux_convert_to_pin(IOB,14), pin_mux_convert_to_pin(IOA,11), CfgSpi::spiMode0, 500000, false);
ioSetCfgEnd(ioTableEnd);

ioSettings *ioSetCfg[] = {
		&led_0,
		&led_1,
		&hbtn_0,
		&bcklight,
		&disprst,
		&touchirq,
		&mmcsdcd,
		&uart_0,
		&usbcdc_0,
		&i2c_0,
		&i2c_3,
		&spi_1_0,
		&ioTableEnd,
		(ioSettings *)-1/* This is an artifice to force linker to put this structure into .data section, to be sure that this will be initiated before the call of init() function. */
};

#endif/*!STM32F769I_DISCO*/
