/*
 * STM32F769I-DISCO.cpp
 *
 *  Created on: Dec 9, 2016
 *      Author: John Smith
 */
#if defined(STM32F469I_DISCO)

#include <board/STM32F469I-DISCO/defs.h>
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
	{ "led-0"		,pin_mux_convert_to_pin(IOG,  6)	,true	,CfgGpio::GPIO_OUT_PUSH_PULL	, false },// Pin 0 mapped
	{ "led-1"		,pin_mux_convert_to_pin(IOD,  4)	,true	,CfgGpio::GPIO_OUT_PUSH_PULL	, false },// Pin 1 mapped
	{ "led-2"		,pin_mux_convert_to_pin(IOD,  5)	,true	,CfgGpio::GPIO_OUT_PUSH_PULL	, false },// Pin 2 mapped
	{ "led-3"		,pin_mux_convert_to_pin(IOK,  3)	,true	,CfgGpio::GPIO_OUT_PUSH_PULL	, false },// Pin 3 mapped
	{ "hbtn-0"		,pin_mux_convert_to_pin(IOA,  0)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false },// Pin 2 mapped
	{ "bcklight"	,pin_mux_convert_to_pin(IOA,  3)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false },// Pin 3 mapped
	{ "disprst"		,pin_mux_convert_to_pin(IOH,  7)	,false	,CfgGpio::GPIO_OUT_PUSH_PULL	, false },// Pin 4 mapped
	{ "touchirq"	,pin_mux_convert_to_pin(IOJ,  5)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false },// Pin 5 mapped
	{ "mmcsdcd"		,pin_mux_convert_to_pin(IOG,  2)	,false	,CfgGpio::GPIO_IN_PULL_UP		, false },// Pin 6 mapped
	{ ""			,0									,false	,CfgGpio::GPIO_IN_PULL_UP		, false }// End of table
};
/*
 * !GPIO settings
 */


/*
 * SPI settings
 */
CfgSpi spiCfg[] =
{/*name,sck, mosi, miso, cs*/
//{ "spi-0.0", 0, 0, 0, 0, },
//{ "spi-0.1", 0, 0, 0, 0, },
//{ "spi-0.2", 0, 0, 0, 0, },
//{ "spi-0.3", 0, 0, 0, 0, },
{ "spi-1.0", pin_mux_convert_to_pin(IOA,  5), pin_mux_convert_to_pin(IOB,  5), pin_mux_convert_to_pin(IOB,  4), pin_mux_convert_to_pin(IOA, 15), 25000000, CfgSpi::spiMode0, false},
//{ "spi-1.1"	, pin_mux_convert_to_pin(IOA, 12), pin_mux_convert_to_pin(IOB, 15), pin_mux_convert_to_pin(IOB, 14), 0, 25000000, CfgSpi::spiMode0, false},
//{ "spi-1.2"	, pin_mux_convert_to_pin(IOA, 12), pin_mux_convert_to_pin(IOB, 15), pin_mux_convert_to_pin(IOB, 14), 0, 25000000, CfgSpi::spiMode0, false},
//{ "spi-1.3"	, pin_mux_convert_to_pin(IOA, 12), pin_mux_convert_to_pin(IOB, 15), pin_mux_convert_to_pin(IOB, 14), 0, 25000000, CfgSpi::spiMode0, false},
//{ "spi-2.0", 0, 0, 0, 0, },
//{ "spi-2.1", 0, 0, 0, 0, },
//{ "spi-2.2", 0, 0, 0, 0, },
//{ "spi-2.3", 0, 0, 0, 0, },
//{ "spi-3.0", 0, 0, 0, 0, },
//{ "spi-3.1", 0, 0, 0, 0, },
//{ "spi-3.2", 0, 0, 0, 0, },
//{ "spi-3.3", 0, 0, 0, 0, },
//{ "spi-4.0", 0, 0, 0, 0, },
//{ "spi-4.1", 0, 0, 0, 0, },
//{ "spi-4.2", 0, 0, 0, 0, },
//{ "spi-4.3", 0, 0, 0, 0, },
//{ "spi-5.0", 0, 0, 0, 0, },
//{ "spi-5.1", 0, 0, 0, 0, },
//{ "spi-5.2", 0, 0, 0, 0, },
//{ "spi-5.3", 0, 0, 0, 0, },
//{ "spi-6.0", 0, 0, 0, 0, },
//{ "spi-6.1", 0, 0, 0, 0, },
//{ "spi-6.2", 0, 0, 0, 0, },
//{ "spi-6.3", 0, 0, 0, 0, },
//{ "spi-7.0", 0, 0, 0, 0, },
//{ "spi-7.1", 0, 0, 0, 0, },
//{ "spi-7.2", 0, 0, 0, 0, },
//{ "spi-7.3", 0, 0, 0, 0, }
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
//{ "i2c-1", 0, 0, },
{ "i2c-2", pin_mux_convert_to_pin(IOH, 4), pin_mux_convert_to_pin(IOH, 5), },
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
//{ "uart-0", pin_mux_convert_to_pin(IOA, 9)	, pin_mux_convert_to_pin(IOA, 10)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC},
//{ "uart-1", pin_mux_convert_to_pin(IOA, 0)	, pin_mux_convert_to_pin(IOA, 0)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC},
{ "uart-2", pin_mux_convert_to_pin(IOB, 10)	, pin_mux_convert_to_pin(IOB, 11)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC},
//{ "uart-3", pin_mux_convert_to_pin(IOA, 0)	, pin_mux_convert_to_pin(IOA, 0)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC},
//{ "uart-4", pin_mux_convert_to_pin(IOA, 0)	, pin_mux_convert_to_pin(IOA, 0)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC},
//{ "uart-5", pin_mux_convert_to_pin(IOA, 0)	, pin_mux_convert_to_pin(IOA, 0)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC},
//{ "uart-6", pin_mux_convert_to_pin(IOA, 0)	, pin_mux_convert_to_pin(IOA, 0)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC},
//{ "uart-7", pin_mux_convert_to_pin(IOA, 0)	, pin_mux_convert_to_pin(IOA, 0)	, 115200	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC}
{ "", 0	, 0	, 0	, CfgUart::WORD_LEN_8	, CfgUart::STOP_BITS_ONE	, CfgUart::PAR_NONE	, CfgUart::MODE_ASYNC}// End of table
};
/*
 * UART settings
 */

#endif/*!STM32F769I_DISCO*/
