/*
 *  api/init.h
 *
 *  Copyright (C) 2016  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of CppSDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STM32F7_BOARD_INIT_H_
#define STM32F7_BOARD_INIT_H_

#include <api/std.h>

#include <api/cursor_ctl_def.h>
#include "sys/core_init.h"

#include "api/i2c.h"

#include "api/spi.h"

#include "api/uart.h"

#include "interface/mmcsd.h"
#include "interface/mmcsd_def.h"

#include "interface/usbh_msc.h"
#include "interface/usbh_msc_def.h"

#include "interface/screen.h"
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
#if (MMCSD_INTERFACE_COUNT > 0)
	GI::Dev::MmcSd *MMCSD[SCREEN_INTERFACE_COUNT];
#endif
#if (USBHMSC_INTERFACE_COUNT > 0)
	GI::Dev::UsbHMsc *USBHMSC[USBHMSC_INTERFACE_COUNT];
#endif
#if (defined(SCREEN_ENABLE) && SCREEN_INTERFACE_COUNT > 0)
	GI::Dev::Screen *SCREEN[SCREEN_INTERFACE_COUNT];
    GI::Screen::Cursor *CAPTOUCH[1];
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

#endif /* STM32F7_BOARD_INIT_H_ */
