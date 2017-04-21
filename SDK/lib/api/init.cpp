/*
 *  api/init.cpp
 */
#if __AVR_XMEGA__
#include <avr/pgmspace.h>
#endif
#include <main.h>
#include <board.h>
#include <stdio.h>
#include "init.h"
#include <api/i2c.h>
#include <api/uart.h>
#include <api/lcd_def.h>
#include <api/uart.h>
#include <api/spi.h>
#include <api/i2c.h>
#include <api/gpio.h>
#include <api/init.h>
#include <lib/operators.h>

#if (USE_GPIO == 1)
#if __AVR_XMEGA__
extern const CfgGpio *gpioCfg[];
#else
extern CfgGpio gpioCfg[];
#endif
#endif

#if (USE_SPI == 1)
extern CfgSpi spiCfg[];
#endif

#if (USE_I2C == 1)
extern CfgI2c i2cCfg[];
#endif

#if (USE_UART == 1)
extern CfgUart uartCfg[];
#endif

GI::Sys::Clock coreClk = GI::Sys::Clock();
GI::Board::Init dev = GI::Board::Init();
#if (defined(SCREEN_ENABLE) && SCREEN_INTERFACE_COUNT > 0)
GI::Dev::IntScreen *screen;
#endif

GI::Board::Init::Init()
{
    memset(this, 0, sizeof(*this));
    unsigned int dev_cnt = 0;
    unsigned int table_len = 0;
    /*******************************************************************/
#if (USE_GPIO == 1)
    /*
     * Scan to get the pin table items number.
     */
    for (table_len = 0; table_len < 512; table_len++)
    {
#if __AVR_XMEGA__
		if (pgm_read_byte(&gpioCfg[table_len]->name[0]) == 0)
			break;
#else
        if (gpioCfg[table_len].name[0] == 0)
            break;
#endif
    }
    /*
     * Allocate memory to store pins handlers pointers.
     */
    GPIO = (GI::Dev::Gpio **) calloc(1, (table_len + 1) * sizeof(GI::Dev::Gpio *));
    /*
     * Allocate create and initialize pins.
     */
    for (dev_cnt = 0; dev_cnt < table_len; dev_cnt++)
    {
#if __AVR_XMEGA__
        if (pgm_read_byte(&gpioCfg[dev_cnt]->name[0]) != 0)
#else
        if (gpioCfg[dev_cnt].name[0] != 0)
#endif
        {
#if __AVR_XMEGA__
			CfgGpio tmp;
			memcpy_P(&tmp, gpioCfg[dev_cnt], sizeof(CfgGpio));
            GPIO[dev_cnt] = new GI::Dev::Gpio(&tmp);
#else
            GPIO[dev_cnt] = new GI::Dev::Gpio(&gpioCfg[dev_cnt]);
#endif
        }
    }
#endif
    /*******************************************************************/
#if (USE_UART == 1)
    /*
     * Scan to get the uarts table items number.
     */
    for (table_len = 0; table_len < 512; table_len++)
    {
        if (uartCfg[table_len].name[0] == 0)
            break;
    }
    /*
     * Allocate memory to store uarts handlers pointers.
     */
    UART = (GI::Dev::Uart **) calloc(1, (table_len + 1) * sizeof(GI::Dev::Uart *));
    /*
     * Allocate create and initialize uarts.
     */
    for (dev_cnt = 0; dev_cnt < table_len; dev_cnt++)
    {
        if (uartCfg[dev_cnt].name[0] != 0)
        {
            UART[dev_cnt] = new GI::Dev::Uart(uartCfg[dev_cnt].name);
        }
    }
#endif
    /*******************************************************************/
#if (USE_I2C == 1)
    /*
     * Scan to get the i2c table items number.
     */
    for (table_len = 0; table_len < 512; table_len++)
    {
        if (i2cCfg[table_len].name[0] == 0)
            break;
    }
    /*
     * Allocate memory to store i2c handlers pointers.
     */
    I2C = (GI::Dev::I2c **) calloc(1, (table_len + 1) * sizeof(GI::Dev::I2c *));
    /*
     * Allocate create and initialize i2c.
     */
    for (dev_cnt = 0; dev_cnt < table_len; dev_cnt++)
    {
        if (i2cCfg[dev_cnt].name[0] != 0)
        {
            I2C[dev_cnt] = new GI::Dev::I2c(i2cCfg[dev_cnt].name);
        }
    }
#endif
    /*******************************************************************/
#if (USE_SPI == 1)
    /*
     * Scan to get the spi table items number.
     */
    for (table_len = 0; table_len < 512; table_len++)
    {
        if (spiCfg[table_len].name[0] == 0)
            break;
    }
    /*
     * Allocate memory to store spi handlers pointers.
     */
    SPI = (GI::Dev::Spi **) calloc(1, (table_len + 1) * sizeof(GI::Dev::Spi *));
    /*
     * Allocate create and initialize spi.
     */
    for (dev_cnt = 0; dev_cnt < table_len; dev_cnt++)
    {
        if (spiCfg[dev_cnt].name[0] != 0)
        {
            SPI[dev_cnt] = new GI::Dev::Spi(spiCfg[dev_cnt].name);
        }
    }
#endif
    /*******************************************************************/
#if (SCREEN_INTERFACE_COUNT > 0 && defined(SCREEN_CONFIG) && defined(USED_SCREEN_CONTROLLER))
    SCREEN[0] = new GI::Dev::USED_SCREEN_CONTROLLER(&SCREEN_CONFIG, NULL);
#ifdef USED_TOUCHSCREEN
    CAPTOUCH[0] = new GI::Sensor::USED_TOUCHSCREEN(SCREEN[0], (char *)CAP_TOUCHSCREEN_I2C_UNIT, (char *)CAP_TOUCHSCREEN_IRQ_PIN);
    CAPTOUCH[0]->touchCalibrate();
#endif
#endif
#if defined(STD_OUT_PATH)
    _stdout = new GI::Std((char *)STD_OUT_PATH);
#endif
#if defined(STD_IN_PATH)
    _stdin = new GI::Std((char *)STD_IN_PATH);
#endif
#if defined(STD_ERR_PATH)
    _stderr = new GI::Std((char *)STD_ERR_PATH);
#endif
#if (USE_MMCSD_ENABLE == 1 && MMCSD_INTERFACE_COUNT > 0)
    MMCSD[0] = new GI::Dev::MmcSd(0, (char *)MMCSD_CARD_DETECT_PIN, (char *)MMCSD_STATUS_LED_PIN);
    MMCSD[0]->idle(0);
#endif
#if (USE_LWIP == 1)
    LWIP[0] = new GI::Dev::Eth(0, 0);
#endif
#if (USE_LWIP_HTTPD_SERVER == 1 && USE_LWIP == 1)
    LWIP_HTTP_SERVER[0] = new GI::App::HttpServer(LWIP_HTTPD_SERVER_PORT);
#endif
#if (USE_TFTP == 1 && USE_LWIP == 1)
    tftpd_init(TFTP_SERVER_PORT);
#endif
#if (USE_FTP == 1 && USE_LWIP == 1)
    ftpd_init(FTP_SERVER_PORT);
#endif
#if (USE_SNTP == 1 && USE_LWIP == 1)
    SNTP_CLIENT = new GI::App::SntpClient();
#endif
}

GI::Board::Init::~Init()
{

}

void GI::Board::Init::idle()
{
#if (USE_MMCSD_ENABLE == 1 && MMCSD_INTERFACE_COUNT > 0)
    MMCSD[0]->idle(0);
#endif
#if (USE_LWIP == 1)
    LWIP[0]->idle(0);
#endif
}

