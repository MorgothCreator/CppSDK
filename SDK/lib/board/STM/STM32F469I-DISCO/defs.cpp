/*
 * defs.cpp
 */
#if defined(STM32F469I_DISCO)

#include <board/STM/STM32F469I-DISCO/defs.h>
#include <stdio.h>

#include <api/init.h>
#include <api/gpio.h>
#include <api/spi.h>
#include <api/i2c.h>
#include <api/uart.h>

/* IO initialization table */
ioSetCfgGpio(led_0, 	"led-0",	pin_mux_convert_to_pin(IOG,  6),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, true, 0);
ioSetCfgGpio(led_1, 	"led-1",	pin_mux_convert_to_pin(IOD,  4),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, true, 0);
ioSetCfgGpio(led_2, 	"led-2",	pin_mux_convert_to_pin(IOD,  5),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, true, 0);
ioSetCfgGpio(led_3, 	"led-3",	pin_mux_convert_to_pin(IOK,  3),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, true, 0);
ioSetCfgGpio(hbtn_0, 	"hbtn-0",	pin_mux_convert_to_pin(IOA,  0),CfgGpio::GPIO_IN_PULL_UP, 		false, false, 0);
ioSetCfgGpio(bcklight, 	"bcklight",	pin_mux_convert_to_pin(IOA,  3),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(disprst, 	"disprst",	pin_mux_convert_to_pin(IOH,  7),CfgGpio::GPIO_OUT_PUSH_PULL, 	false, false, 1);
ioSetCfgGpio(touchirq, 	"touchirq",	pin_mux_convert_to_pin(IOJ,  5),CfgGpio::GPIO_IN_PULL_UP, 		false, false, 0);
ioSetCfgGpio(mmcsdcd, 	"mmcsdcd",	pin_mux_convert_to_pin(IOG,  2),CfgGpio::GPIO_IN_PULL_UP, 		false, false, 1);
ioSetCfgUart(uart_2, 	"uart-2", 	pin_mux_convert_to_pin(IOB, 10)	, pin_mux_convert_to_pin(IOB,11)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);
ioSetCfgUart(uart_5, 	"uart-5", 	pin_mux_convert_to_pin(IOG, 14)	, pin_mux_convert_to_pin(IOG, 9)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);
ioSetCfgUart(usbcdc_0, 	"usbcdc-0", pin_mux_convert_to_pin(IOA, 0)	, pin_mux_convert_to_pin(IOA, 0)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC);
ioSetCfgI2c(i2c_0, 		"i2c-0", 	pin_mux_convert_to_pin(IOB, 8)	, pin_mux_convert_to_pin(IOB, 9), 100000);
ioSetCfgI2c(i2c_2, 		"i2c-2", 	pin_mux_convert_to_pin(IOH, 4)	, pin_mux_convert_to_pin(IOH, 5), 100000);
ioSetCfgSpi(spi_1_0, 	"spi-1.0", 	pin_mux_convert_to_pin(IOA, 5)	, pin_mux_convert_to_pin(IOB, 5), pin_mux_convert_to_pin(IOB, 4), pin_mux_convert_to_pin(IOA,15), CfgSpi::spiMode0, 2500000, false);
ioSetCfgEnd(ioTableEnd);

ioSettings *ioSetCfg[] = {
		&led_0,
		&led_1,
		&led_2,
		&led_3,
		&hbtn_0,
		&bcklight,
		&disprst,
		&touchirq,
		&mmcsdcd,
		&uart_2,
		&uart_5,
		&usbcdc_0,
		&i2c_0,
		&i2c_2,
		&spi_1_0,
		&ioTableEnd,
		(ioSettings *)-1/* This is an artifice to force linker to put this structure ito .data section, to be secure that this will be initiated before the call of init() function. */
};



/*
 * GPIO settings
 */
CfgGpio gpioCfg[] =
{/*sck, mosi, miso, cs*/
	{ "led-0"		,pin_mux_convert_to_pin(IOG,  6)	,true	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 0 },// Pin 0 mapped
	{ "led-1"		,pin_mux_convert_to_pin(IOD,  4)	,true	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 0 },// Pin 1 mapped
	{ "led-2"		,pin_mux_convert_to_pin(IOD,  5)	,true	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 0 },// Pin 2 mapped
	{ "led-3"		,pin_mux_convert_to_pin(IOK,  3)	,true	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 0 },// Pin 3 mapped
	{ "hbtn-0"		,pin_mux_convert_to_pin(IOA,  0)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 },// Pin 2 mapped
	{ "bcklight"	,pin_mux_convert_to_pin(IOA,  3)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 1 },// Pin 3 mapped
	{ "disprst"		,pin_mux_convert_to_pin(IOH,  7)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 1 },// Pin 4 mapped
	{ "touchirq"	,pin_mux_convert_to_pin(IOJ,  5)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 },// Pin 5 mapped
	{ "mmcsdcd"		,pin_mux_convert_to_pin(IOG,  2)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 1 },// Pin 6 mapped
	{ ""			,0									,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 }// End of table
};

#endif/*!STM32F769I_DISCO*/
