/*
 * STM32F769I-DISCO.cpp
 *
 *  Created on: Dec 9, 2016
 *      Author: John Smith
 */
#if defined(SmartRf06)

#include "defs.h"
#include <stdio.h>

#include <api/gpio.h>
#include <api/spi.h>
#include <api/i2c.h>
#include <api/uart.h>

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
	{ "led-0"		,pin_mux_convert_to_pin(IOA, 25)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 0 },// Pin 0 mapped
	{ "led-1"		,pin_mux_convert_to_pin(IOA, 27)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 0 },// Pin 1 mapped
	{ "led-2"		,pin_mux_convert_to_pin(IOA,  7)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 0 },// Pin 1 mapped
	{ "led-3"		,pin_mux_convert_to_pin(IOA,  6)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 0 },// Pin 1 mapped
	{ "hbtn-1"		,pin_mux_convert_to_pin(IOA, 19)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 },// Pin 2 mapped
	{ "hbtn-2"		,pin_mux_convert_to_pin(IOA, 12)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 },// Pin 2 mapped
	{ "hbtn-3"		,pin_mux_convert_to_pin(IOA, 15)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 },// Pin 2 mapped
	{ "hbtn-4"		,pin_mux_convert_to_pin(IOA, 18)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 },// Pin 2 mapped
	{ "hbtn-5"		,pin_mux_convert_to_pin(IOA, 11)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 },// Pin 2 mapped
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
//{ "spi-0.0", pin_mux_convert_to_pin(IOA, 10), pin_mux_convert_to_pin(IOA,  9), pin_mux_convert_to_pin(IOA,  8), pin_mux_convert_to_pin(IOA, 30), 5000000, CfgSpi::spiMode0, false},
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
//{ "i2c-0", pin_mux_convert_to_pin(IOB, 8), pin_mux_convert_to_pin(IOB, 9), 100000, },
//{ "i2c-1", 0, 0, },
//{ "i2c-2", pin_mux_convert_to_pin(IOA, 8), pin_mux_convert_to_pin(IOC, 9), 100000, },
//{ "i2c-3", pin_mux_convert_to_pin(IOD, 12), pin_mux_convert_to_pin(IOB, 7), },
//{ "i2c-4", 0, 0, },
//{ "i2c-5", 0, 0, },
//{ "i2c-6", 0, 0, },
//{ "i2c-7", 0, 0, }
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
{ "uart-0", pin_mux_convert_to_pin(IOA, 3)	, pin_mux_convert_to_pin(IOA, 2)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC},
{ "", 0	, 0	, 0	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC}// End of table
};
/*
 * UART settings
 */

#endif/*!STM32F769I_DISCO*/
