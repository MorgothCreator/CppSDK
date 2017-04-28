# CppSDK
CPP embedded standalone collection software.

This SDK is a mix of drivers provided by microcontroller manufacturers and library's developed by me and other developers.

I started this project with ideea that any beginner developer can start a standalone project portable on multiple platforms with no or few changes made on the project.

This project integrate drivers for most common interfaces like GPIO, UART, I2C and SPI but even for most advenced interfaces like MMCSD, ETH, USB and LCD's with a nice and simple to create GUI interfaces with or without touch screens.

This project is a translation of <a href="https://github.com/MorgothCreator/mSdk">mSdk prtoject</a> from C to C++ offering a more flexible function manipulation.

In furure this project will be extended to AVR32, AM335x, STM32F0, STM32F1, STM32F3, ATxmega and other platforms.

This project will provide some example applications, to use this examples without changes extract or clone this project in "C:\GitHub" folder.

For different platforms I use different IDE's to develop this SDK, see description on each platform.

-For STM32 I use <a href="http://www.openstm32.org/HomePage">System Workbench for STM32 IDE</a>, I decide to begin with this platform because this manufacturer offer a huge low level driver support.

-For NXP Kinetis E I use <a href="http://www.nxp.com/products/software-and-tools/hardware-development-tools/freedom-development-boards/kinetis-design-studio-integrated-development-environment-ide:KDS_IDE">Kinetis Design Studio IDE</a>, I extend the development on kinetis E series because is one of the 32bit cortex platform that can work on 5V power voltage, this has the advantage of high noise immunity and is designed for high reliable industrial applications.

-For Atmel devices I use <a href="http://atmel.com">AtmelStudio 6.2 IDE</a>

-For TI CC13xx I use <a href="http://processors.wiki.ti.com/index.php/Download_CCS">Code Composer Studio IDE</a>

At this moment include next features:

	STM32F4 (System Workbench for STM32 IDE):
		Board:
			MikromediaForSTM32M4 configuration files.
			STM32F429I-DISCO configuration files.
			STM32F469I-DISCO configuration files.
		Drivers:
			I2C.
			UART.
			SPI.
			GPIO.
			MMCSD (no SPI mode).
			LCD using DSI interface.
		Dedicated evices:
 			OTM8009a TFT display controller.
			ILI9341 TFT display controller.
      
      
 	STM32F7 (System Workbench for STM32 IDE):
		Board:
			STM32F769I configuration files.
		Drivers:
			I2C.
			UART.
			SPI.
			GPIO.
			MMCSD.
			ETH with LwIp.
			LCD using DSI interface.
		Dedicated evices:
 			OTM8009a TFT display controller.
			ILI9341 TFT display controller.
      
 	NXP Kinetis E seties (Kinetis Design Studio IDE):
		Board:
			FRDM-KEAZ128Q80 configuration files.
		Drivers:
			I2C(NOT TESTED).
			UART.
			SPI(NOT TESTED).
			GPIO.
      
 	CC13xx (Code Compose Studio):
		Board:
			SmartRF06 configuration files.
		Drivers:
			I2C(NOT TESTED).
			UART.
			SPI(NOT TESTED).
			GPIO.
      
 	ATxmega E5 (Atmel studio):
		Board:
			Xmega_E5_Xplained.
		Drivers:
			I2C(NOT TESTED).
			UART.
			SPI.
			GPIO.
      
 	ATSAMD21 (Atmel studio):
		Board:
			FeatherAdaloggerM0.
		Drivers:
			I2C(NOT TESTED).
			UART.
			SPI(NOT TESTED).
			GPIO.
      
> On all platforms will be ready to work with next list of sensors:

* AK8975 magnetometer sensor using I2C interface.
* BMP180 pressure, altimeter and temperature sensor using I2C interface.
* FT5x06 capacitive touch screen using I2C interface
* HIH613x humidity and temperature sensor using I2C interface.
* ILI9341 TFT display controller using SPI interface.
* L3GD20 Gyroscope using SPI interface.
* Lepton FLIR sensor using SPI interface.
* MI0283 color LCD display controller using GPIO interface.
* MPL3115a2 pressure altimeter and temperature sensor using I2C interface.
* MPR121 capacitive touch controller using I2C interface.
* MPU60x0 and MPU9x50 accelerometer, giroscope and temperature sensor using I2C interface.
* STMPE811 resistive touchscreen controller using I2C interface.

> On several platforms will be disponible next interfaces:

* For platforms that are provided the ETH interface driver will come with the LwIp library and some applications like HTTP server, TFTP server and other applications.
* For platforms that are provided the USB MSC host driver will mount the FS automaticaly when a USB pen drive is inserted (the supported FS are FAT12, FAT16, FAT32 and exFAT).
* For platforms that are provided the USB MSC device you need to link a MMCSD interface or a custom disk driver.

> All namespaces and classes are under "GI" namespace.

Please loock for available interfaces in "SDK/lib/board/[Manufacturer]/[Board directory]/def.cpp" file.
To request controll of one of initialized interface use next examples:

> Example of blinking led 1 on the board:

```CPP
/*
 * Put the '#include <api/init.h>' first on file because first
 *     need to call the init function from this file.
 */
#include <api/init.h>
#include <api/io_handle.h>

int main(void)
{
	/*
	 * Take controll to "led-0" pin.
	 */
	GI::IO led_pin = GI::IO((char *)"led-0");
	while(1)
	{
		led_pin.write(true);
		GI::Sys::Timer::delay(500);
		led_pin.write(false);
		GI::Sys::Timer::delay(500);
	}
}
```
OR
```CPP
/*
 * Put the '#include <api/init.h>' first on file because first
 *     need to call the init function from this file.
 */
#include <api/init.h>
#include <api/io_handle.h>

int main(void)
{
	while(1)
	{
		GI::IO::write((char *)"led-0", true);
		GI::Sys::Timer::delay(500);
		GI::IO::write((char *)"led-0", false);
		GI::Sys::Timer::delay(500);
	}
}
```

> Heartbeating one led using non blocking timer:

```CPP
/*
 * Put the '#include <api/init.h>' first on file because first 
 *     need to call the init function from this file.
 */
#include <api/init.h>
#include <api/io_handle.h>
#include <api/timer.h>

int main(void)
{
	bool led_state = false;
	/*
	 * Create a 500ms nonblocking timer.
	 */
	GI::Sys::Timer blink_timer = GI::Sys::Timer(500);
	/*
	 * Take controll to "led-0" pin.
	 */
	GI::IO led_pin = GI::IO((char *)"led-0");
	while(1)
	{
		/*
		 * Ceck if is a tick.
		 */
		if(blink_timer.tick())
		{
			bool state;
			/*
			 * Get 'led-0' pin state.
			 */
			led_pin.read(&state);
			if(state)
			{
				if(led_state)
				{
					/*
					 * If 'led_state == true' will be a long pause.
					 */
					blink_timer.interval(780);
					led_state = false;
				}
				else
				{
					/*
					 * If 'led_state == false' will be a short pause.
					 */
					blink_timer.interval(180);
					led_state = true;
				}
				led_pin.write(false);
			}
			else
			{
				blink_timer.interval(20);
				led_pin.write(true);
			}
		}
	}
}
```
OR
```CPP
/*
 * Put the '#include <api/init.h>' first on file because first 
 *     need to call the init function from this file.
 */
#include <api/init.h>
#include <api/io_handle.h>
#include <api/timer.h>

int main(void)
{
	bool led_state = false;
	/*
	 * Create a 500ms nonblocking timer.
	 */
	GI::Sys::Timer blink_timer = GI::Sys::Timer(500);
	while(1)
	{
		/*
		 * Ceck if is a tick.
		 */
		if(blink_timer.tick())
		{
			bool state;
			/*
			 * Get 'led-0' pin state.
			 */
			GI::IO::read((char *)"led-0", &state);
			if(state)
			{
				if(led_state)
				{
					/*
					 * If 'led_state == true' will be a long pause.
					 */
					blink_timer.interval(780);
					led_state = false;
				}
				else
				{
					/*
					 * If 'led_state == false' will be a short pause.
					 */
					blink_timer.interval(180);
					led_state = true;
				}
				GI::IO::write((char *)"led-0", false);
			}
			else
			{
				blink_timer.interval(20);
				GI::IO::write((char *)"led-0", true);
			}
		}
	}
}
```
> To create a working window:

```CPP
/*
 * Put the '#include <api/init.h>' first on file because first  need to call the init function from this file.
 */
#include <api/init.h>
#include "lib/gfx/window.h"
/*
 * Make 'MainWindow' globally visible.
 */
GI::Screen::Gfx::Window *MainWindow = NULL;
int main(void)
{
	/*
	 * Create one parent window.
	 */
	MainWindow = new GI::Screen::Gfx::Window(NULL, dev.SCREEN[0]);/*The parent window for this window is NULL, the screen is SCREEN 0*/
	MainWindow->AllowHScroll = false;/*If items on this window is outsiede borders do not allow horizontal scroll bar*/
	MainWindow->AllowVScroll = false;/*If items on this window is outsiede borders do not allow vertical scroll bar*/
	MainWindow->HideHScroll = true;/*Make horizontal scroll bal hidden*/
	MainWindow->HideVScroll = true;/*Make vertical scroll bal hidden*/
	MainWindow->MaxMinEnabled = false;/*Deactivate minimize/maximize button on header*/
	MainWindow->MinimizeButonEnabled = false;/*Deactivate minimize button on header*/
	MainWindow->CloseButonEnabled = false;/*Deactivate close button on header*/
	MainWindow->Caption->textAlign = alignLeft;/*Allign caption text to left*/
	
	/*
	 * Create a text box.
	 */
	newTextBox(MainWindow, SensorResultTextbox);
	SensorResultTextbox->Position.X = 10;
	SensorResultTextbox->Position.Y = 10;
	SensorResultTextbox->Size.X = 460;
	SensorResultTextbox->Size.Y = 200;
	SensorResultTextbox->text->setText((char *)"This is a sensor result textbox");

	/*
	 * Create a button.
	 */
	newButton(MainWindow, Buton1);
	Buton1->Position.X = 10;
	Buton1->Position.Y = SensorResultTextbox->Position.Y + SensorResultTextbox->Size.Y + 10;
	Buton1->Size.X = 100;
	Buton1->Size.Y = 50;

	/*
	 * Create a progress bar.
	 */
	newProgressBar(MainWindow, ProgressBar1);
	ProgressBar1->MinimValue  = 0;
	ProgressBar1->MaximValue = 100;
	ProgressBar1->Value = 50;
	ProgressBar1->Position.X = 10;
	ProgressBar1->Position.Y = Buton1->Position.Y + Buton1->Size.Y + 10;
	ProgressBar1->Size.X = 460;
	ProgressBar1->Size.Y = 30;

	/*
	 * Create a check box.
	 */
	newCheckBox(MainWindow, CheckBox1);
	CheckBox1->Position.X = 10;
	CheckBox1->Position.Y = ProgressBar1->Position.Y + ProgressBar1->Size.Y + 10;
	CheckBox1->Size.X = 460;
	CheckBox1->Size.Y = 30;

	/*
	 * Create a list box.
	 */
	newListBox(MainWindow, ListBox);
	ListBox->Position.X = 10;
	ListBox->Position.Y = CheckBox1->Position.Y + CheckBox1->Size.Y + 10;
	ListBox->Size.X = 460;
	ListBox->Size.Y = MainWindow->Internals.pDisplay->LcdTimings->Y - (CheckBox1->Position.Y + CheckBox1->Size.Y + 30);
	ListBox->Size.ScrollSize = 50;/* The scroll btn width size. */
	ListBox->Size.ItemSizeY = 30/* The item hecht size. */;
	ListBox->Size.MinScrollBtnSize = 30;/* minimum scroll button height size. */
	ListBox->Caption->textAlign = alignCenter;/*The text on each item will be alligned to center*/
	/*
	 * Populate list box with 256 items.
	 */
	unsigned int cnt = 0;
	char listbox_buff[32];
	char buff_tmp[10];
	for(; cnt < 256; cnt++)
	{
		strcpy((char *)listbox_buff, "Device ID ");
		utoa(cnt + 1, buff_tmp, 10);
		strcat(listbox_buff, buff_tmp);
		ListBox->add(listbox_buff);
	}
	/*
	 * Put a second window inside current window, a password protection window.
	 *
	 * The parrent window will be MainWindow, the name will be pass, 
	 *	the x space between buttons wil be 2 pixels, the y space between buttons will be 2 pixels.
	 */
	newWindowPasswordNumeric(MainWindow, pass, 2, 2);
	
	/*
	 * Create a nonblocking timmer to call the idle function of parrent window every 20 miliseconds.
	 * The 20ms is the maximum refresh rate of cursor and window.
	 */
	GI::Sys::Timer timer_touch = GI::Sys::Timer(20);

	while(1)
	{
		/*
		 * Ceck if is a tick.
		 */
		if(timer_touch.tick())
		{
			MainWindow->idle(dev.CURSORCTRL[0]->idle());
			/*
			 * The idle function of 'pass' window will be called by parent window, 
			 *	but the idle function on 'pass' controll need to be call by user 
			 *	returning true if the "OK" button has been hitted or false if no.
			 */
			if(pass->idle())
			{
				/*
				 * The "OK" button has been pressed, check if password is correct.
				 */
				if(pass->password->equal((char *)"1234"))
					/*
					 * If password match, make pasword window invisible.
					 * At this moment is not implemented the destroy window function, 
					 *	for this reason I will use visible = false, to hide password window.
					 */
					pass->internals.windowHandler->Visible = false;
				else
				{
					/*
					 * If password does not match print "Wrong password!" on password texbox.
					 */
					pass->clearText->set((char *)"Wrong password!");
				}
			}
		}
	}
}
```

More detalied example you can see <a href="https://github.com/MorgothCreator/CppSDK/tree/master/Common">here</a>, it is a common application thet work on all platforms.
