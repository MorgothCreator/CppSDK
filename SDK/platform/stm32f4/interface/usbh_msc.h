/*
 * usb_interface.h
 *
 * Created: 3/7/2013 6:44:43 PM
 *  Author: XxXx
 */ 


#ifndef USB_INTERFACE_H_
#define USB_INTERFACE_H_

#include <stdbool.h>
#include "driver/stm32f4xx_hal_hcd.h"
#include "driver/USBH/Core/Inc/usbh_core.h"
#include "driver/USBH/Class/MSC/inc/usbh_msc.h"
#include "interface/gpio.h"
#include "api/gpio_def.h"
#include "lib/fs/fat/inc/diskio.h"
#include "lib/fs/fat/inc/ff.h"
#include "board/board.h"

namespace GI
{
namespace Dev {
	class UsbHMsc {
	public:
		UsbHMsc(unsigned int instance, GI::Dev::Gpio* StatusLed);
		~UsbHMsc();
		static unsigned int read(void *heandler, void *ptr, unsigned long block,
									  unsigned int nblks);
		static unsigned int write(void *heandler, void *ptr, unsigned long block,
									  unsigned int nblks);
		static void ioctl(void *heandler, unsigned int  command,  unsigned int *buffer);
		void idle(unsigned int instance);
	private:
	};
}
}

extern HCD_HandleTypeDef  hhcd;

#ifdef HEADER_INCLUDE_C_FILES
#include "usb_interface.c"
#endif

#endif /* USB_INTERFACE_H_ */
