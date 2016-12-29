# CppSDK
CPP embedded standalone collection software.

This SDK is a mix of drivers provided by microcontroller manufacturers and library's developed by me and other developers.

I started this project with ideea that any beginner developer can start a standalone project portable on multiple platforms with no or few changes made on the project.

This project integrate drivers for most common interfaces like GPIO, UART, I2C and SPI but even for most advenced interfaces like MMCSD, ETH, USB and LCD's with a nice and simple to create GUI interfaces with or without touch screens.

This project is a translation of <a href="https://github.com/MorgothCreator/mSdk">mSdk prtoject</a> from C to C++ offering a more flexible function manipulation.

In furure this project will be extended to AVR32, AM335x, STM32F0, STM32F1, STM32F3, ATxmega and other platforms.

This project will provide some example applications, to use this examples without changes extract or clone this project in "C:\GitHub" folder.

For different platforms I use different IDE's to develop this SDK, see description on each platform.

For STM32 I use <a href="http://www.openstm32.org/HomePage">System Workbench for STM32 IDE</a>.

At this moment include next features:

	STM32F4 (System Workbench for STM32 IDE):
		Board:
			STM32F469I configuration board.
		Drivers:
			I2C.
			UART.
			SPI.
			GPIO.
			MMCSD.
			LCD using DSI interface.
		Devices:
			FT5x06 capacitive touch screen using I2C interface
 			OTM8009a TFT display controller.
 			Lepton FLIR sensor using SPI interface.
      
      
 	STM32F7 (System Workbench for STM32 IDE):
		Board:
			STM32F769I configuration board.
		Drivers:
			I2C.
			UART.
			SPI.
			GPIO.
			MMCSD.
			ETH with LwIp.
			LCD using DSI interface.
		Devices:
			FT5x06 capacitive touch screen using I2C interface
 			OTM8009a TFT display controller.
 			Lepton FLIR sensor using SPI interface.
      
* For platforms that are provided the ETH interface driver will come with the LwIp library and some applications like HTTP server, TFTP server and other applications.
* For platforms that are provided the USB MSC host driver will mount the FS automaticaly when a USB pen drive is inserted (the FS supported are FAT12, FAT16, FAT32 and exFAT).
* For platforms that are provided the USB MSC device you need to link a MMCSD interface or a custom disk driver.


Please watch for available interfaces in "[platform directory]/board/[Board directory]/def.cpp" file.
To request controll of one of initialized interface use next examples:

> Example of blinking led 1 on the board:

```CPP
#include<api/io_handle.h>
#include<api/timer.h>

int main(void)
{
	GI::IO led_pin = GI::IO((char *)"led-0");
	while(1)
	{
		unsigned long dummy;
		led_pin->ctl(GI::IO::IO_CTL_ASSERT_BIT, &dummy);
		GI::Sys::Timer::delay(500);
		led_pin->ctl(GI::IO::IO_CTL_DEASERT_BIT, &dummy);
		GI::Sys::Timer::delay(500);
	}
}
```

> Blinking led 1 using non blocking timer:

```CPP
#include<api/io_handle.h>
#include<api/timer.h>

int main(void)
{
	GI::Sys::Timer blink_timer = GI::Sys::Timer(500);
	GI::IO led_pin = GI::IO((char *)"led-0");
	while(1)
	{
		if(blink_timer.tick())
		{
			unsigned long state;
			led_pin.ctl(GI::IO::IO_CTL_GET_BIT, &state);
			if(state)
				led_pin.ctl(GI::IO::IO_CTL_DEASERT_BIT, &state);
			else
				led_pin.ctl(GI::IO::IO_CTL_ASSERT_BIT, &state);
		}
	}
}
```
