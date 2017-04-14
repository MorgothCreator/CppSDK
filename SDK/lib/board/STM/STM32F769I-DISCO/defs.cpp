/*
 * STM32F769I-DISCO.cpp
 *
 *  Created on: Dec 9, 2016
 *      Author: John Smith
 */
#if defined(STM32F769I_DISCO)

#include <board/STM/STM32F769I-DISCO/defs.h>
#include <stdio.h>
/*
 * Screen settings
 */

/*
 * !Screen settings
 */

/*
 * GPIO settings
 */
CfgGpio gpioCfg[] =
{/*sck, mosi, miso, cs*/
	{ "led-0"		,pin_mux_convert_to_pin(IOJ, 13)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 0 },// Pin 0 mapped
	{ "led-1"		,pin_mux_convert_to_pin(IOJ,  5)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 0 },// Pin 1 mapped
	{ "hbtn-0"		,pin_mux_convert_to_pin(IOA,  0)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 },// Pin 2 mapped
	{ "bcklight"	,pin_mux_convert_to_pin(IOI, 14)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 1 },// Pin 3 mapped
	{ "disprst"		,pin_mux_convert_to_pin(IOJ, 15)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 1 },// Pin 4 mapped
	{ "touchirq"	,pin_mux_convert_to_pin(IOI, 13)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 },// Pin 5 mapped
	{ "mmcsdcd"		,pin_mux_convert_to_pin(IOI, 15)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 1 },// Pin 6 mapped
	{ ""			,0									,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 }// End of table
};
/*
 * !GPIO settings
 */

/*
 * SPI settings
 */
CfgSpi spiCfg[] =
{/*name,sck, mosi, miso, cs*/
{ "spi-1.0", pin_mux_convert_to_pin(IOA, 12), pin_mux_convert_to_pin(IOB, 15), pin_mux_convert_to_pin(IOB, 14), pin_mux_convert_to_pin(IOA, 11), 16000000, CfgSpi::spiMode3, false},
{ "", 0, 0, 0, 0, }// End of table
};
/*
 * !SPI settings
 */

/*
 * I2C settings
 */
CfgI2c i2cCfg[] =
{
{ "i2c-0", pin_mux_convert_to_pin(IOB, 8), pin_mux_convert_to_pin(IOB, 9), 100000, },
{ "i2c-3", pin_mux_convert_to_pin(IOD, 12), pin_mux_convert_to_pin(IOB, 7), },
{ "", 0, 0, }// End of table
};
/*
 * !I2C settings
 */

/*
 * !UART settings
 */
CfgUart uartCfg[] =
{
{ "uart-0", pin_mux_convert_to_pin(IOA, 9)	, pin_mux_convert_to_pin(IOA, 10)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC},
{ "usbcdc-0", pin_mux_convert_to_pin(IOA, 0)	, pin_mux_convert_to_pin(IOA, 0)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC},
{ "", 0	, 0	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC}// End of table
};
/*
 * UART settings
 */

#endif/*!STM32F769I_DISCO*/
