/*
 * defs.cpp
 */
#if defined(STM32F429I_DISCO)

#include <board/STM/STM32F469I-DISCO/defs.h>
#include <stdio.h>

#include <api/init.h>
#include <api/gpio.h>
#include <api/spi.h>
#include <api/i2c.h>
#include <api/uart.h>

/* IO initialization table */
ioSetCfgGpio(led_0, 	"led-0",	pin_mux_convert_to_pin(IOG, 13),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 0);
ioSetCfgGpio(led_1, 	"led-1",	pin_mux_convert_to_pin(IOG, 14),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 0);
ioSetCfgGpio(hbtn_0, 	"hbtn-0",	pin_mux_convert_to_pin(IOA,  0),CfgGpio::GPIO_IN_PULL_UP, 		false, false, 0);
ioSetCfgGpio(lcd_ncs, 	"lcd_ncs",	pin_mux_convert_to_pin(IOC,  2),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(lcd_wrx, 	"lcd_wrx",	pin_mux_convert_to_pin(IOD, 13),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(lcd_rdx, 	"lcd_rdx",	pin_mux_convert_to_pin(IOD, 12),CfgGpio::GPIO_IN_PULL_UP, 		false, false, 1);
ioSetCfgGpio(touchirq, 	"touchirq",	pin_mux_convert_to_pin(IOA, 15),CfgGpio::GPIO_IN_PULL_UP, 		false, false, 0);
ioSetCfgUart(uart_0, 	"uart-0", 	pin_mux_convert_to_pin(IOA,  9)	, pin_mux_convert_to_pin(IOA,10)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);
ioSetCfgUart(usbcdc_0, 	"usbcdc-0", pin_mux_convert_to_pin(IOA,  0)	, pin_mux_convert_to_pin(IOA, 0)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);
ioSetCfgI2c(i2c_2, 		"i2c-2", 	pin_mux_convert_to_pin(IOA,  8)	, pin_mux_convert_to_pin(IOC, 9), CfgI2c::i2cMode0, 100000);
ioSetCfgSpi(spi_4_0, 	"spi-4.0", 	pin_mux_convert_to_pin(IOF,  7)	, pin_mux_convert_to_pin(IOF, 9), pin_mux_convert_to_pin(IOF, 8), pin_mux_convert_to_pin(IOC, 2), CfgSpi::spiMode0, 500000, false);
ioSetCfgSpi(spi_4_1, 	"spi-4.1", 	pin_mux_convert_to_pin(IOF,  7)	, pin_mux_convert_to_pin(IOF, 9), pin_mux_convert_to_pin(IOF, 8), pin_mux_convert_to_pin(IOC, 1), CfgSpi::spiMode0, 500000, false);
ioSetCfgEnd(ioTableEnd);

ioSettings *ioSetCfg[] = {
		&led_0,
		&led_1,
		&hbtn_0,
		&lcd_ncs,
		&lcd_wrx,
		&lcd_rdx,
		&touchirq,
		&uart_0,
		&usbcdc_0,
		&i2c_2,
		&spi_4_0,
		&spi_4_1,
		&ioTableEnd,
		(ioSettings *)-1/* This is an artifice to force linker to put this structure ito .data section, to be secure that this will be initiated before the call of init() function. */
};

#endif/*!STM32F769I_DISCO*/
