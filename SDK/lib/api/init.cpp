/*
 *  api/init.cpp
 */
#if __AVR_XMEGA__
#include <avr/pgmspace.h>
#endif
#include <board.h>
#include <main.h>
#include <stdio.h>
#include <api/lcd_def.h>
#include <api/uart.h>
#include <api/spi.h>
#include <api/i2c.h>
#include <api/gpio.h>
#include <api/init.h>
#include <api/init_def.h>
#include <lib/operators.h>

extern ioSettings *ioSetCfg[];

GI::Sys::Clock coreClk = GI::Sys::Clock();
GI::Board::Init dev = GI::Board::Init();

GI::Board::Init::Init()
{
    memset(this, 0, sizeof(*this));
/*******************************************************************/
    unsigned int io_cnt = 0;
    for(; io_cnt < 1024; io_cnt++)
	{
		ioSettings *io_item = ioSetCfg[io_cnt];
		if(io_item->cfg)
		{
			switch((unsigned int)io_item->info.ioType)
			{
#if (USE_GPIO == true)
			case ioSettings::info_s::ioType_GPIO:
				io_item->ioConstruct = (void *)new GI::Dev::Gpio(io_item);
				break;
#endif
#if (USE_UART == true)
			case ioSettings::info_s::ioType_UART:
				io_item->ioConstruct = (void *)new GI::Dev::Uart(io_item);
				break;
#endif
#if (USE_I2C == true)
			case ioSettings::info_s::ioType_I2C:
				io_item->ioConstruct = (void *)new GI::Dev::I2c(io_item);
				break;
#endif
#if (USE_SPI == true)
			case ioSettings::info_s::ioType_SPI:
				io_item->ioConstruct = (void *)new GI::Dev::Spi(io_item);
				break;
#endif
			}
		}
		else
			break;
	}
/*******************************************************************/
/*
 * Register drivers.
 */
#if (defined(SCREEN_CONFIG_1) && defined(USED_SCREEN_CONTROLLER_1))
    SCREEN[0] = new GI::Dev::USED_SCREEN_CONTROLLER_1(&SCREEN_CONFIG_1, (char *)OTHER_SCREEN_1_PARAMS);
#ifdef USED_TOUCHSCREEN_1
    CURSORCTRL[0] = new GI::Sensor::USED_TOUCHSCREEN_1(SCREEN[0], (char *)CAP_TOUCHSCREEN_I2C_UNIT, (char *)CAP_TOUCHSCREEN_IRQ_PIN);
    CURSORCTRL[0]->touchCalibrate();
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


