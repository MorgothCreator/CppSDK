/*
 * defs.cpp
 */
#if defined(STM32F429I_DISCO)

#include <board/STM/STM32F469I-DISCO/defs.h>
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
	{ "led-0"		,pin_mux_convert_to_pin(IOG, 13)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 0 },// Pin 0 mapped
	{ "led-1"		,pin_mux_convert_to_pin(IOG, 14)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 0 },// Pin 1 mapped
	{ "hbtn-0"		,pin_mux_convert_to_pin(IOA,  0)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 },// Pin 2 mapped
	//{ "bcklight"	,pin_mux_convert_to_pin(IOA,  3)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 1 },// Pin 3 mapped
	//{ "disprst"		,pin_mux_convert_to_pin(IOH,  7)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false, 1 },// Pin 4 mapped
	{ "lcd_ncs"	,pin_mux_convert_to_pin(IOC,  2)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL		, false, 1 },// Pin 5 mapped
	{ "lcd_wrx"	,pin_mux_convert_to_pin(IOD, 13)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL		, false, 1 },// Pin 5 mapped
	{ "lcd_rdx"	,pin_mux_convert_to_pin(IOD, 12)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL		, false, 1 },// Pin 5 mapped
	{ "touchirq"	,pin_mux_convert_to_pin(IOJ,  5)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 0 },// Pin 5 mapped
	//{ "mmcsdcd"		,pin_mux_convert_to_pin(IOG,  2)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false, 1 },// Pin 6 mapped
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
{ "spi-4.0", pin_mux_convert_to_pin(IOF,  7), pin_mux_convert_to_pin(IOF,  9), pin_mux_convert_to_pin(IOF,  8), pin_mux_convert_to_pin(IOC,  2), 5000000, CfgSpi::spiMode0, false},
{ "spi-4.1", pin_mux_convert_to_pin(IOF,  7), pin_mux_convert_to_pin(IOF,  9), pin_mux_convert_to_pin(IOF,  8), pin_mux_convert_to_pin(IOC,  1), 5000000, CfgSpi::spiMode0, false},
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
{ "i2c-2", pin_mux_convert_to_pin(IOA, 8), pin_mux_convert_to_pin(IOC, 9), 100000, },
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
{ "uart-con", pin_mux_convert_to_pin(IOA, 9)	, pin_mux_convert_to_pin(IOA, 10)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC},
{ "", 0	, 0	, 0	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC}// End of table
};
/*
 * UART settings
 */

#endif/*!STM32F769I_DISCO*/
