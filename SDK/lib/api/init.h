/*
 * init_modules.h
 *
 *  Created on: Dec 10, 2016
 *      Author: John Smith
 */

#ifndef STM32F7_BOARD_INIT_H_
#define STM32F7_BOARD_INIT_H_

#include <api/std.h>

#include <api/cursor_ctl_def.h>
#include "sys/core_init.h"

#include "interface/i2c.h"
#include "interface/i2c_def.h"

#include "interface/spi.h"
#include "interface/spi_def.h"

#include "interface/uart.h"
#include "interface/uart_def.h"

#include "interface/mmcsd.h"
#include "interface/mmcsd_def.h"

#include "interface/usbh_msc.h"
#include "interface/usbh_msc_def.h"

#include "interface/screen.h"
#include "interface/screen_def.h"

#if (USE_LWIP == 1)
#include <interface/eth.h>
#include <api/eth_def.h>
#endif

#if (USE_LWIP_HTTPD_SERVER == 1)
#include <app/lwip/httpd/httpd.h>
#endif


namespace GI {
namespace Board{
class Init{
public:
	Init();
	~Init();
	void idle();
	friend GI::Std;
	GI::Std *_stdout;
	GI::Std *_stdin;
	GI::Std *_stderr;
	GI::Dev::Gpio **GPIO;
	GI::Dev::I2c **I2C;
	GI::Dev::Spi **SPI;
	GI::Dev::Uart **UART;
	GI::Dev::MmcSd *MMCSD[SCREEN_INTERFACE_COUNT];
	GI::Dev::UsbHMsc *USBHMSC[USBHMSC_INTERFACE_COUNT];
	GI::Dev::Screen *SCREEN[SCREEN_INTERFACE_COUNT];
#if (USE_LWIP == 1)
	GI::Dev::Eth *LWIP[1];
#endif
#if (USE_LWIP_HTTPD_SERVER == 1)
	GI::App::HttpServer *LWIP_HTTP_SERVER[1];
#endif
	GI::Screen::Cursor *CAPTOUCH[1];
};
}
}

extern GI::Sys::Clock coreClk;
extern GI::Board::Init dev;

#endif /* STM32F7_BOARD_INIT_H_ */
