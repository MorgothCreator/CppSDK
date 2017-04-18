/*
 * interface/usbh_msc.h
 */ 


#ifndef USB_INTERFACE_H_
#define USB_INTERFACE_H_

#include "driver/stm32f7xx_hal_hcd.h"

#include <stdbool.h>
#include "api/gpio.h"
#include <board.h>


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
