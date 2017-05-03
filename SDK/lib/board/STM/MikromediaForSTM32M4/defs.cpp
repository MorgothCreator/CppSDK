/*
 * defs.cpp
 */
#if defined(MikromediaForSTM32M4)

#include <board/STM/STM32F469I-DISCO/defs.h>
#include <stdio.h>

#include <api/init.h>
#include <api/gpio.h>
#include <api/spi.h>
#include <api/i2c.h>
#include <api/uart.h>
#include <api/lcd_def.h>

/* IO initialization table */
ioSetCfgGpio(mi0283_dat,"mi0283-dat",	pin_mux_convert_to_pin(IOE,  0),CfgGpio::GPIO_OUT_PUSH_PULL, 	true,  false, 1);
ioSetCfgGpio(mi0283_rst,"mi0283-rst",	pin_mux_convert_to_pin(IOE,  8),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(mi0283_rs, "mi0283-rs"	,	pin_mux_convert_to_pin(IOE, 12),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(mi0283_cs, "mi0283-cs"	,	pin_mux_convert_to_pin(IOE, 15),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(mi0283_wr, "mi0283-wr"	,	pin_mux_convert_to_pin(IOE, 11),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(mi0283_rd, "mi0283-rd"	,	pin_mux_convert_to_pin(IOE, 10),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(backlight, "backlight"	,	pin_mux_convert_to_pin(IOE,  9),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(microsd_cd,"microsd-cd",	pin_mux_convert_to_pin(IOD, 15),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(vs1053_rst,"vs1053-rst",	pin_mux_convert_to_pin(IOC,  7),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(vs1053_dreq,"vs1053-dreq",	pin_mux_convert_to_pin(IOC,  6),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgUart(uart_0, 	"uart-1", 		pin_mux_convert_to_pin(IOD,  5)	, pin_mux_convert_to_pin(IOD, 6)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);
ioSetCfgUart(usbcdc_0, 	"usbcdc-0", 	pin_mux_convert_to_pin(IOA,  0)	, pin_mux_convert_to_pin(IOA, 0)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);
ioSetCfgI2c(i2c_0, 		"i2c-0", 		pin_mux_convert_to_pin(IOB,  6)	, pin_mux_convert_to_pin(IOB, 7), 100000);
ioSetCfgSpi(spi_2_0, 	"spi-2.0", 		pin_mux_convert_to_pin(IOC, 10)	, pin_mux_convert_to_pin(IOC,11), pin_mux_convert_to_pin(IOC,12), pin_mux_convert_to_pin(IOD, 3), CfgSpi::spiMode0,25000000, false);//uSD CS
ioSetCfgSpi(spi_2_1, 	"spi-2.1", 		pin_mux_convert_to_pin(IOC, 10)	, pin_mux_convert_to_pin(IOC,11), pin_mux_convert_to_pin(IOC,12), pin_mux_convert_to_pin(IOD, 7), CfgSpi::spiMode0,25000000, false);//M25P80 CS
ioSetCfgSpi(spi_2_2, 	"spi-2.2", 		pin_mux_convert_to_pin(IOC, 10)	, pin_mux_convert_to_pin(IOC,11), pin_mux_convert_to_pin(IOC,12), pin_mux_convert_to_pin(IOC, 8), CfgSpi::spiMode0, 5000000, false);//VS1053 CS
ioSetCfgSpi(spi_2_3, 	"spi-2.3", 		pin_mux_convert_to_pin(IOC, 10)	, pin_mux_convert_to_pin(IOC,11), pin_mux_convert_to_pin(IOC,12), pin_mux_convert_to_pin(IOC, 9), CfgSpi::spiMode0, 5000000, false);//VS1053 DCS
//ioSetCfgGpio(led_0, 	"led-0"		,	pin_mux_convert_to_pin(IOE,  9),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
//ioSetCfgScreen(screen_0, "screen-0", &lcd_MI0283_PORTRAIT, 0);
ioSetCfgEnd(ioTableEnd);

ioSettings *ioSetCfg[] = {
		&mi0283_dat,
		&mi0283_rst,
		&mi0283_rs,
		&mi0283_cs,
		&mi0283_wr,
		&mi0283_rd,
		&backlight,
		&microsd_cd,
		&vs1053_rst,
		&vs1053_dreq,
		&uart_0,
		&usbcdc_0,
		&i2c_0,
		&spi_2_0,
		&spi_2_1,
		&spi_2_2,
		&spi_2_3,
		//&led_0,
		&ioTableEnd,
		(ioSettings *)-1/* This is an artifice to force linker to put this structure ito .data section, to be secure that this will be initiated before the call of init() function. */
};

#endif/*!STM32F769I_DISCO*/
