/*
 *  api/init.h
 */

#ifndef STM32F7_BOARD_INIT_H_
#define STM32F7_BOARD_INIT_H_

#include <api/std.h>

#include <api/cursor_ctl_def.h>
#include "sys/core_init.h"

#include "api/gpio.h"

#include "api/i2c.h"

#include "api/spi.h"

#include "api/uart.h"

#include "interface/mmcsd.h"
#include "interface/mmcsd_def.h"

#include "interface/usbh_msc.h"
#include "interface/usbh_msc_def.h"

#include "interface/screen.h"
#include <device/mi0283_gpio.h>
#include <device/ssd1306.h>
#include <device/st7565.h>
#include "interface/screen_def.h"

#if (USE_LWIP == 1)
#include <interface/eth.h>
#include <api/eth.h>
#endif

#if (USE_LWIP_HTTPD_SERVER == 1)
#include <app/lwip/httpd/httpd.h>
#endif

#include<app/lwip/ftpserver/ftpd.h>
#include<app/lwip/tftpserver/tftpserver.h>
#include<app/lwip/sntp/sntp.h>

#include <device/ft5x06.h>
#include <device/stmpe811.h>


namespace GI {
namespace Board{
class Init{
public:
	Init();
	~Init();
	void idle();
	void ioTableInit();
	friend GI::Std;
	GI::Std *_stdout;
	GI::Std *_stdin;
	GI::Std *_stderr;
	ioSettings **ioTable;
#if (MMCSD_INTERFACE_COUNT > 0)
	GI::Dev::MmcSd *MMCSD[SCREEN_INTERFACE_COUNT];
#endif
#if (USBHMSC_INTERFACE_COUNT > 0)
	GI::Dev::UsbHMsc *USBHMSC[USBHMSC_INTERFACE_COUNT];
#endif
#if (defined(SCREEN_CONFIG_1) && defined(USED_SCREEN_CONTROLLER_1) && (SCREENS_COUNT > 0))
	GI::Dev::Screen *SCREEN[SCREENS_COUNT];
#ifdef USED_TOUCHSCREEN_1
	GI::Sensor::USED_TOUCHSCREEN_1 *CURSORCTRL[1];
#endif
#endif
#if (USE_LWIP == 1)
	GI::Dev::Eth *LWIP[1];
#endif
#if (USE_LWIP_HTTPD_SERVER == 1 && USE_LWIP == 1)
	GI::App::HttpServer *LWIP_HTTP_SERVER[1];
#endif
#if (USE_SNTP == 1 && USE_LWIP == 1)
	GI::App::SntpClient *SNTP_CLIENT;
#endif
};
}
}

extern GI::Sys::Clock coreClk;
extern GI::Board::Init dev;

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "init.cpp"
#endif
/*#####################################################*/

#endif /* STM32F7_BOARD_INIT_H_ */
