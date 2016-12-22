/*
 * init_modules.cpp
 *
 *  Created on: Dec 10, 2016
 *      Author: John Smith
 */

#include <stdio.h>
#include "init.h"
#include <interface/screen.h>
#include <interface/i2c.h>
#include <device/ft5x06.h>

GI::Sys::Clock coreClk;
#if (SCREEN_ENABLE == 1)
GI::Dev::Screen screen(&lcd_OTM8009A_PORTRAIT, NULL);
#endif
GI::Board::Init dev;

GI::Board::Init::Init()
{
	memset(this, 0, sizeof(*this));
	unsigned int dev_cnt = 0;
	unsigned int table_len = 0;
/*******************************************************************/
	/*
	 * Scan to get the pin table items number.
	 */
	for(table_len = 0; table_len < 512; table_len++)
	{
		if(gpioCfg[table_len].name[0] == 0)
			break;
	}
	/*
	 * Allocate memory to store pins handlers pointers.
	 */
	GPIO = (GI::Dev::Gpio **)calloc(1, (table_len + 1) * sizeof(GI::Dev::Gpio *));
	/*
	 * Allocate create and initialize pins.
	 */
	for(dev_cnt = 0; dev_cnt < table_len; dev_cnt++)
	{
		if(gpioCfg[dev_cnt].name[0] != 0)
		{
			GPIO[dev_cnt] = new GI::Dev::Gpio(&gpioCfg[dev_cnt]);
		}
	}
	/*******************************************************************/
	/*
	 * Scan to get the uarts table items number.
	 */
	for(table_len = 0; table_len < 512; table_len++)
	{
		if(uartCfg[table_len].name[0] == 0)
			break;
	}
	/*
	 * Allocate memory to store uarts handlers pointers.
	 */
	UART = (GI::Dev::Uart **)calloc(1, (table_len + 1) * sizeof(GI::Dev::Uart *));
	/*
	 * Allocate create and initialize uarts.
	 */
	for(dev_cnt = 0; dev_cnt < table_len; dev_cnt++)
	{
		if(uartCfg[dev_cnt].name[0] != 0)
		{
			UART[dev_cnt] = new GI::Dev::Uart(uartCfg[dev_cnt].name);
		}
	}
/*******************************************************************/
	/*
	 * Scan to get the i2c table items number.
	 */
	for(table_len = 0; table_len < 512; table_len++)
	{
		if(i2cCfg[table_len].name[0] == 0)
			break;
	}
	/*
	 * Allocate memory to store i2c handlers pointers.
	 */
	I2C = (GI::Dev::I2c **)calloc(1, (table_len + 1) * sizeof(GI::Dev::I2c *));
	/*
	 * Allocate create and initialize i2c.
	 */
	for(dev_cnt = 0; dev_cnt < table_len; dev_cnt++)
	{
		if(i2cCfg[dev_cnt].name[0] != 0)
		{
			I2C[dev_cnt] = new GI::Dev::I2c(i2cCfg[dev_cnt].name);
		}
	}
/*******************************************************************/
	/*
	 * Scan to get the spi table items number.
	 */
	for(table_len = 0; table_len < 512; table_len++)
	{
		if(spiCfg[table_len].name[0] == 0)
			break;
	}
	/*
	 * Allocate memory to store spi handlers pointers.
	 */
	SPI = (GI::Dev::Spi **)calloc(1, (table_len + 1) * sizeof(GI::Dev::Spi *));
	/*
	 * Allocate create and initialize spi.
	 */
	for(dev_cnt = 0; dev_cnt < TWI_INTERFACE_COUNT; dev_cnt++)
	{
		if(spiCfg[dev_cnt].name[0] != 0)
		{
			SPI[dev_cnt] = new GI::Dev::Spi(spiCfg[dev_cnt].name);
		}
	}
/*******************************************************************/
	SCREEN[0] = &screen;
	CAPTOUCH[0] = new GI::Screen::Cursor(SCREEN[0], (char *)CAP_TOUCHSCREEN_I2C_UNIT, (char *)CAP_TOUCHSCREEN_IRQ_PIN);
#if defined(STD_OUT_PATH)
	_stdout = new GI::Std((char *)STD_OUT_PATH);
#endif
#if defined(STD_IN_PATH)
	_stdin = new GI::Std((char *)STD_IN_PATH);
#endif
#if defined(STD_ERR_PATH)
	_stderr = new GI::Std((char *)STD_ERR_PATH);
#endif
#if (USE_MMCSD_ENABLE == 1)
	MMCSD[0] = new GI::Dev::MmcSd(0, (char *)MMCSD_CARD_DETECT_PIN, (char *)MMCSD_STATUS_LED_PIN);
	MMCSD[0]->idle(0);
#endif
#if (USE_LWIP == 1)
	LWIP[0] = new GI::Dev::Eth(0, 0);
#endif
#if (USE_LWIP_HTTPD_SERVER == 1)
	LWIP_HTTP_SERVER[0] = new GI::App::HttpServer(LWIP_HTTPD_SERVER_PORT);
#endif
}

GI::Board::Init::~Init()
{

}

void GI::Board::Init::idle()
{
#if (USE_MMCSD_ENABLE == 1)
		MMCSD[0]->idle(0);
#endif
#if (USE_LWIP == 1)
		LWIP[0]->idle(0);
#endif

}

