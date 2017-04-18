/*
 * main.cpp
 *
 *  Created on: Dec 9, 2016
 *      Author: John Smith
 */

#include <main.h>

#include <api/init.h>
#include <stdio.h>
#include <api/io_handle.h>


#if (_USE_SCREEN == 1)
#include "lib/gfx/window.h"
#include "lib/gfx/PasswordWindowNumeric.h"
#endif


#if _USE_HIH613x == 1
#include <device/hih6130.h>
#endif
#if _USE_MPU60x0_9150 == 1
#include <device/mpu60x0_9x50.h>
#endif
#if (_USE_AK8975 == 1)
#include <device/ak8975.h>
#endif
#if _USE_BMP180 == 1
#include <device/bmp180.h>
#endif
#if _USE_MPL3115A2 == 1
#include <device/mpl3115a2.h>
#endif
#if _USE_MPR121 == 1
#include <device/mpr121.h>
#endif
#if _USE_L3GD20 == 1
#include <device/l3gd20.h>
#endif
#include <sys/core_init.h>
#include <app/sys/cmd.h>

#include <app/lwip/http_client/http_client.h>

#if (_USE_SCREEN == 1)
GI::Screen::Gfx::Window *MainWindow = NULL;
GI::Screen::Gfx::TextBox *SensorResultTextboxGlobal;
#endif

void main_app(void)
{
	GI::Sys::Timer timer_touch = GI::Sys::Timer(20);
	GI::Sys::Timer blink_timer = GI::Sys::Timer(100);

	bool led_state = false;

#if (_USE_HIH613x == 1)
	GI::Sys::Timer hih613timer = GI::Sys::Timer(1000);
#endif
#if (_USE_MPU60x0_9150 == 1 && _USE_AK8975 == 1)
	GI::Sys::Timer mpu60x0_9x50_timer = GI::Sys::Timer(200);
#endif
#if (_USE_BMP180 == 1)
	GI::Sys::Timer bmp180_timer = GI::Sys::Timer(1000);
#endif
#if (_USE_MPL3115A2 == 1)
	GI::Sys::Timer mpl3115_timer = GI::Sys::Timer(1000);
#endif
#if (_USE_MPR121 == 1)
	GI::Sys::Timer mpr121_timer = GI::Sys::Timer(50);
#endif
#if (_USE_L3GD20 == 1)
	GI::Sys::Timer l3gd20_timer = GI::Sys::Timer(50);
#endif


#if (_USE_SCREEN == 1)
	/*
	 * Create one parent window.
	 */
	MainWindow = new GI::Screen::Gfx::Window(NULL, dev.SCREEN[0]);
    MainWindow->AllowHScroll = false;
    MainWindow->AllowVScroll = false;
    MainWindow->HideHScroll = true;
    MainWindow->HideVScroll = true;
    MainWindow->MaxMinEnabled = false;
    MainWindow->MinimizeButonEnabled = false;
    MainWindow->CloseButonEnabled = false;
    MainWindow->Caption->textAlign = alignLeft;


	/*
	 * Create a picture box.
	 */
	newPictureBox(MainWindow, FlirPictureBox);
	//FlirPictureBox_Global = FlirPictureBox;
	FlirPictureBox->Position.X = 10;
	FlirPictureBox->Position.Y = 10;
	FlirPictureBox->Size.X = 460;
	FlirPictureBox->Size.Y = 160;
	FlirPictureBox->BackgroundColor = 0xFFFFFFFF;

	/*
	 * Create a text box.
	 */
	newTextBox(MainWindow, SensorResultTextbox);
	SensorResultTextboxGlobal = SensorResultTextbox;
	SensorResultTextbox->Position.X = 10;
	SensorResultTextbox->Position.Y = FlirPictureBox->Position.Y + FlirPictureBox->Size.Y + 10;
	SensorResultTextbox->Size.X = 460;
	SensorResultTextbox->Size.Y = 200;
	SensorResultTextbox->Size.ScrollSize = 50;
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
    ListBox->Size.ScrollSize = 50;
    ListBox->Size.ItemSizeY = 30;
    ListBox->Size.MinScrollBtnSize = 30;
    ListBox->Caption->textAlign = alignLeft;
	/*
	 * Populate list box with 256 items.
	 */
    u32 cnt = 0;
    char listbox_buff[32];
	char buff_tmp[10];
    for(; cnt < 256; cnt++)
    {
		strcpy((char *)listbox_buff, "Device ID ");
		utoa(cnt + 1, buff_tmp, 10);
		strcat(listbox_buff, buff_tmp);
		ListBox->add(listbox_buff);
    }


#if(_USE_PASSWORD_PROTECTION == 1)
    newWindowPasswordNumeric(MainWindow, pass, 2, 2);
#endif
#endif/*!(_USE_SCREEN == 1)*/
    /*
     * Put on parent window caption the IP of ETH interface.
     */

#if (USE_LWIP == 1)
    GI::String ipAddressStr = GI::String();
    GI::String ip_str_buff = GI::String();
    dev.LWIP[0]->getIp(&ip_str_buff);
	//MainWindow->Caption->setTextF("IP:%s\n\r", ip_str_buff.buff);
    ipAddressStr.set((char *)"IP:");
    ipAddressStr.append(ip_str_buff.buff);
	bool old_state_ip_assigned = false;
	//dev._stdout->printF("IP:%s\n\r", ip_str_buff.buff);
#endif

#if (_USE_HIH613x == 1)
	GI::Sensor::Hih613x hih6130 = GI::Sensor::Hih613x((char *)"i2c-0");
#endif
#if (_USE_MPU60x0_9150 == 1)
	GI::Sensor::Mpu60x0_9x50 mpu60x0_9x50 = GI::Sensor::Mpu60x0_9x50((char *)"i2c-0", 0);
#endif
#if (_USE_AK8975 == 1)
	/*
	 * If AK8975 is inside MPU9150 you need to put this after you initialize the MPU device.
	 */
	GI::Sensor::Ak8975 ak8975_0 = GI::Sensor::Ak8975((char *)"i2c-0", 0);
#endif
#if (_USE_BMP180 == 1)
	GI::Sensor::Bmp180 bmp180_0 = GI::Sensor::Bmp180((char *)"i2c-0", 0);
#endif
#if (_USE_MPL3115A2 == 1)
	GI::Sensor::Mpl3115a2 mpl3115a2 = GI::Sensor::Mpl3115a2((char *)"i2c-0");
#endif
#if (_USE_MPR121 == 1)
	GI::Sensor::Mpr121 mpr121_0 = GI::Sensor::Mpr121((char *)"i2c-0", (char *)"", 0);
#endif
#if (_USE_L3GD20 == 1)
	GI::Sensor::L3gd20 l3gd20_0 = GI::Sensor::L3gd20((char *)"spi-4.1");
#endif
#if (_USE_SCREEN == 1)
	tControlCommandData control_comand;
#endif

#if (USE_UART == 1 && USE_TERMINAL == 1)
	Cmd term = Cmd((char *)CONSOLE_UART_IN, (char *)CONSOLE_UART_OUT, (char *)CONSOLE_UART_ERR);
#endif
	/*GI::Sys::Clock::changeCoreClk(25000000);
	unsigned long baud = 1200;
	terminal.ctl(GI::IO::IO_CTL_SET_SPEED, &baud);*/

#if (SHOW_SENSORS_RESULT_ON_SCREEN == 0)
	GI::IO terminal = GI::IO((char *)CONSOLE_UART_OUT);
#endif
	//terminal.write((unsigned char *)buffer, bytesread);

	while(1)
	{
		//GI::Sys::Clock::sleep();
		dev.idle();
#if (USE_UART == 1 && USE_TERMINAL == 1)
		term.idle();
#endif
#if (USE_LWIP == 1)
		if(old_state_ip_assigned == false && dev.LWIP[0]->ipAssigned == true)
		{
			old_state_ip_assigned = true;
		    /*
		     * Update on parent window caption the IP of ETH interface if is assigned by DHCP.
		     */
		    GI::String ip_str_buff = GI::String();
		    dev.LWIP[0]->getIp(&ip_str_buff);
			//MainWindow->Caption->setTextF("IP:%s\n\r", ip_str_buff.buff);
			//dev._stdout->printF("IP:%s\n\r", ip_str_buff.buff);
		    ipAddressStr.set((char *)"IP:");
		    ipAddressStr.append(ip_str_buff.buff);
		}
#endif
#if (USE_LWIP == 1)
		if(ipAddressStr.modifyed || dev.SNTP_CLIENT->time_update)
		{
			ipAddressStr.modifyed = 0;
			dev.SNTP_CLIENT->time_update = 0;
			MainWindow->Caption->setTextF(&ipAddressStr);
			/*@todo: "The server does not respond"*/
			//tcp_setup((char *)"google.com",(char *)"GET /");
			if(dev.SNTP_CLIENT->time_str->length)
				MainWindow->Caption->appendF(",Time:%s", dev.SNTP_CLIENT->time_str->buff);
		}
#endif
		/*
		 * Call every 20ms the idle function of touch screen interface
		 * 	and idle function of parent window.
		 */
		if(timer_touch.tick())
		{
#if (_USE_SCREEN == 1)
			MainWindow->idle(dev.CAPTOUCH[0]->idle());
#endif
#if (_USE_PASSWORD_PROTECTION == 1)
			if(pass->idle())
			{
				if(pass->password->equal((char *)"1234"))
					pass->internals.windowHandler->Visible = false;
				else
				{
					pass->clearText->set((char *)"Wrong password!");
				}
			}
#endif
		}
		if(blink_timer.tick())
		{
			bool state;
			GI::IO::read((char *)"led-0", &state);
			if(state)
			{
				if(led_state)
				{
					blink_timer.interval(780);
					led_state = false;
				}
				else
				{
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
#if _USE_HIH613x == 1
			if(hih613timer.tick())
			{
				unsigned char hih613x_status = 0;;
				float hih613x_hum = 0;
				float hih613x_temp = 0;
				if(!hih6130.get(&hih613x_status, &hih613x_hum, &hih613x_temp))
				{
					switch(hih613x_status)
					{
					case 0:
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
						ListBox->Items[0]->Caption->setTextF((char *)"HIH613x: T = %2.2f, H = %2.1f", hih613x_temp, hih613x_hum);
#else
						GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"HIH613x: T = %2.2f, H = %2.1f", hih613x_temp, hih613x_hum);
#endif
						break;
					case 1:
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
						ListBox->Items[0]->Caption->setText((char *)"HIH613x: stale data");
#else
						GI::IO::write((char *)CONSOLE_UART_OUT, (char *)"HIH613x: stale data");
#endif
						break;
					case 2:
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
						ListBox->Items[0]->Caption->setText((char *)"HIH613x: in command mode");
#else
						GI::IO::write((char *)CONSOLE_UART_OUT, (char *)"HIH613x: in command mode");
#endif
						break;
					case 3:
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
						ListBox->Items[0]->Caption->setText((char *)"HIH613x: diagnostic");
#else
						GI::IO::write((char *)CONSOLE_UART_OUT, (char *)"HIH613x: diagnostic");
#endif
						break;
					}
				}
				else
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					SensorResultTextbox->text->append((char *)"HIH613x:  error reading temperature and humidity\n\r");
#else
					GI::IO::write((char *)CONSOLE_UART_OUT, (char *)"HIH613x:  error reading temperature and humidity\n\r");
#endif
			}
#endif
#if _USE_MPU60x0_9150 == 1
			bool mpu60x0_9x50_timer_ticked = false;
			if(mpu60x0_9x50_timer.tick())
			{
				mpu60x0_9x50_timer_ticked = true;
				float mpu60x0_9150_temp = 0.0;
				signed short mpu60x0_9150_gyro_Xg = 0;
				signed short mpu60x0_9150_gyro_Yg = 0;
				signed short mpu60x0_9150_gyro_Zg = 0;
				signed short mpu60x0_9150_accel_Xa = 0;
				signed short mpu60x0_9150_accel_Ya = 0;
				signed short mpu60x0_9150_accel_Za = 0;
				if(!mpu60x0_9x50.getTempData(&mpu60x0_9150_temp))
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[1]->Caption->setTextF((char *)"MPU60x0: Temp:  %2.2f Gr Celsius\n\r", mpu60x0_9150_temp);
#else
					GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MPU60x0: Temp:  %2.2f Gr Celsius\n\r", mpu60x0_9150_temp);
#endif
				else
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[1]->Caption->setText((char *)"MPU60x0: error reading temperature\n\r");
#else
					GI::IO::write((char *)CONSOLE_UART_OUT, (char *)"MPU60x0: error reading temperature\n\r");
#endif

				if(!mpu60x0_9x50.getGyroData(&mpu60x0_9150_gyro_Xg, &mpu60x0_9150_gyro_Yg, &mpu60x0_9150_gyro_Zg))
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[2]->Caption->setTextF((char *)"MPU60x0: Giro:  Xg = %6d, Yg = %6d, Zg = %6d", mpu60x0_9150_gyro_Xg, mpu60x0_9150_gyro_Yg, mpu60x0_9150_gyro_Zg);
#else
					GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MPU60x0: Giro:  Xg = %6d, Yg = %6d, Zg = %6d", mpu60x0_9150_gyro_Xg, mpu60x0_9150_gyro_Yg, mpu60x0_9150_gyro_Zg);
#endif
				else
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[2]->Caption->setText((char *)"MPU60x0: error reading giroscope");
#else
					GI::IO::write((char *)CONSOLE_UART_OUT, (char *)"MPU60x0: error reading giroscope");
#endif

				if(!mpu60x0_9x50.getAccelData(&mpu60x0_9150_accel_Xa, &mpu60x0_9150_accel_Ya, &mpu60x0_9150_accel_Za))
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[3]->Caption->setTextF((char *)"MPU60x0: Accel: Xa = %6d, Ya = %6d, Za = %6d", mpu60x0_9150_accel_Xa, mpu60x0_9150_accel_Ya, mpu60x0_9150_accel_Za);
#else
					GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MPU60x0: Accel: Xa = %6d, Ya = %6d, Za = %6d", mpu60x0_9150_accel_Xa, mpu60x0_9150_accel_Ya, mpu60x0_9150_accel_Za);
#endif
				else
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[3]->Caption->setText((char *)"MPU60x0: error reading accelerometer");
#else
					GI::IO::write((char *)CONSOLE_UART_OUT, (char *)"MPU60x0: error reading accelerometer");
#endif
			}
#endif
#if (_USE_AK8975 == 1)
			if(mpu60x0_9x50_timer_ticked)
			{
				signed short AK8975_X_Axis = 0, AK8975_Y_Axis = 0, AK8975_Z_Axis = 0;
				if(!ak8975_0.getMag(&AK8975_X_Axis, &AK8975_Y_Axis, &AK8975_Z_Axis))
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[4]->Caption->setTextF((char *)"AK8975: Magne: Xm = %6d, Ym = %6d, Zm = %6d", AK8975_X_Axis, AK8975_Y_Axis, AK8975_Z_Axis);
#else
					GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"AK8975: Magne: Xm = %6d, Ym = %6d, Zm = %6d", AK8975_X_Axis, AK8975_Y_Axis, AK8975_Z_Axis);
#endif
				else
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[4]->Caption->setText((char *)"AK8975: error reading magnetometer");
#else
					GI::IO::write((char *)CONSOLE_UART_OUT, (char *)"AK8975: error reading magnetometer");
#endif
			}
#endif
#if _USE_BMP180 == 1
			if(bmp180_timer.tick())
			{
				float bmp180_temperature = 0.0;
				float bmp180_pressure = 0.0;
				float bmp180_altitude = 0.0;
				if(!bmp180_0.getTemp(&bmp180_temperature) &&
						!bmp180_0.getPressure(&bmp180_pressure, GI::Sensor::Bmp180::BMP180_CTRL_MEAS_OSS_8) &&
							!bmp180_0.getAltitude(&bmp180_altitude, GI::Sensor::Bmp180::BMP180_CTRL_MEAS_OSS_8))
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[5]->Caption->setTextF((char *)"BMP180: Temp = %2.1f, Press = %4.2f, Alt = %4.2f", bmp180_temperature, bmp180_pressure, bmp180_altitude);
#else
					GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"BMP180: Temp = %2.1f, Press = %4.2f, Alt = %4.2f", bmp180_temperature, bmp180_pressure, bmp180_altitude);
#endif
				else
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[5]->Caption->setText((char *)"BMP180: error reading temp, press and altitude");
#else
					GI::IO::write((char *)CONSOLE_UART_OUT, (char *)"BMP180: error reading temp, press and altitude");
#endif
			}
#endif
#if _USE_MPL3115A2 == 1
			if(mpl3115_timer.tick())
			{
				float mpl3115a2_pressure = 0.0, mpl3115a2_altitude = 0.0, mpl3115a2_temp = 0.0;
				if(!mpl3115a2.getAltTemp(2, &mpl3115a2_pressure, &mpl3115a2_altitude, &mpl3115a2_temp))
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[6]->Caption->setTextF((char *)"MPL3115A1: T = %3.3f, P = %3.5f, Alt = %4.3f", mpl3115a2_temp, 0.0, mpl3115a2_altitude);
#else
					GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MPL3115A1: T = %3.3f, P = %3.5f, Alt = %4.3f", mpl3115a2_temp, 0.0, mpl3115a2_altitude);
#endif
				else
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[6]->Caption->setText((char *)"MPL3115A1: error reading temp, press and altitude");
#else
					GI::IO::write((char *)CONSOLE_UART_OUT, (char *)"MPL3115A1: error reading temp, press and altitude");
#endif
			}
#endif
#if _USE_MPR121 == 1
			if(mpr121_timer.tick())
			{
				mpr121_ret_t mpr121_return;
				if(!mpr121_0.idle(&mpr121_return))
				{
					if(mpr121_return.pushed)
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
						ListBox->Items[7]->Caption->setTextF((char *)"MPR121: Pushed   - K0=%u, K1=%u, K2=%u, K3=%u, K4=%u, K5=%u, K6=%u, K7=%u, K8=%u, K9=%u, K10=%u, K11=%u",     (unsigned long)mpr121_return.pushed & 0x01,   (unsigned long)(mpr121_return.pushed >> 1) & 0x01,   (unsigned long)(mpr121_return.pushed >> 2) & 0x01,   (unsigned long)(mpr121_return.pushed >> 3) & 0x01,   (unsigned long)(mpr121_return.pushed >> 4) & 0x01,   (unsigned long)(mpr121_return.pushed >> 5) & 0x01,   (unsigned long)(mpr121_return.pushed >> 6) & 0x01,   (unsigned long)(mpr121_return.pushed >> 7) & 0x01,   (unsigned long)(mpr121_return.pushed >> 8) & 0x01,   (unsigned long)(mpr121_return.pushed >> 9) & 0x01,   (unsigned long)(mpr121_return.pushed >> 10) & 0x01,   (unsigned long)(mpr121_return.pushed >> 11) & 0x01);
#else
						GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MPR121: Pushed   - K0=%u, K1=%u, K2=%u, K3=%u, K4=%u, K5=%u, K6=%u, K7=%u, K8=%u, K9=%u, K10=%u, K11=%u",     (unsigned long)mpr121_return.pushed & 0x01,   (unsigned long)(mpr121_return.pushed >> 1) & 0x01,   (unsigned long)(mpr121_return.pushed >> 2) & 0x01,   (unsigned long)(mpr121_return.pushed >> 3) & 0x01,   (unsigned long)(mpr121_return.pushed >> 4) & 0x01,   (unsigned long)(mpr121_return.pushed >> 5) & 0x01,   (unsigned long)(mpr121_return.pushed >> 6) & 0x01,   (unsigned long)(mpr121_return.pushed >> 7) & 0x01,   (unsigned long)(mpr121_return.pushed >> 8) & 0x01,   (unsigned long)(mpr121_return.pushed >> 9) & 0x01,   (unsigned long)(mpr121_return.pushed >> 10) & 0x01,   (unsigned long)(mpr121_return.pushed >> 11) & 0x01);
#endif
					if(mpr121_return.released)
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
						ListBox->Items[7]->Caption->setTextF((char *)"MPR121: Released - K0=%u, K1=%u, K2=%u, K3=%u, K4=%u, K5=%u, K6=%u, K7=%u, K8=%u, K9=%u, K10=%u, K11=%u", (unsigned long)mpr121_return.released & 0x01, (unsigned long)(mpr121_return.released >> 1) & 0x01, (unsigned long)(mpr121_return.released >> 2) & 0x01, (unsigned long)(mpr121_return.released >> 3) & 0x01, (unsigned long)(mpr121_return.released >> 4) & 0x01, (unsigned long)(mpr121_return.released >> 5) & 0x01, (unsigned long)(mpr121_return.released >> 6) & 0x01, (unsigned long)(mpr121_return.released >> 7) & 0x01, (unsigned long)(mpr121_return.released >> 8) & 0x01, (unsigned long)(mpr121_return.released >> 9) & 0x01, (unsigned long)(mpr121_return.released >> 10) & 0x01, (unsigned long)(mpr121_return.released >> 11) & 0x01);
#else
						GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"MPR121: Released - K0=%u, K1=%u, K2=%u, K3=%u, K4=%u, K5=%u, K6=%u, K7=%u, K8=%u, K9=%u, K10=%u, K11=%u", (unsigned long)mpr121_return.released & 0x01, (unsigned long)(mpr121_return.released >> 1) & 0x01, (unsigned long)(mpr121_return.released >> 2) & 0x01, (unsigned long)(mpr121_return.released >> 3) & 0x01, (unsigned long)(mpr121_return.released >> 4) & 0x01, (unsigned long)(mpr121_return.released >> 5) & 0x01, (unsigned long)(mpr121_return.released >> 6) & 0x01, (unsigned long)(mpr121_return.released >> 7) & 0x01, (unsigned long)(mpr121_return.released >> 8) & 0x01, (unsigned long)(mpr121_return.released >> 9) & 0x01, (unsigned long)(mpr121_return.released >> 10) & 0x01, (unsigned long)(mpr121_return.released >> 11) & 0x01);
#endif
				}
			}
#endif
#if _USE_L3GD20 == 1
			if(l3gd20_timer.tick())
			{
				float l3gd20_Xg = 0;
				float l3gd20_Yg = 0;
				float l3gd20_Zg = 0;
				if(!l3gd20_0.read(&l3gd20_Xg, &l3gd20_Yg, &l3gd20_Zg))
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[2]->Caption->setTextF((char *)"L3GD20: Giro:  Xg = %6.1f, Yg = %6.1f, Zg = %6.1f", l3gd20_Xg, l3gd20_Yg, l3gd20_Zg);
#else
					GI::IO::writeF((char *)CONSOLE_UART_OUT, (char *)"L3GD20: Giro:  Xg = %6.1f, Yg = %6.1f, Zg = %6.1f", l3gd20_Xg, l3gd20_Yg, l3gd20_Zg);
#endif
				else
#if (SHOW_SENSORS_RESULT_ON_SCREEN == 1)
					ListBox->Items[2]->Caption->setText((char *)"L3GD20: error reading giroscope");
#else
					GI::IO::write((char *)CONSOLE_UART_OUT, (char *)"L3GD20: error reading giroscope");
#endif
			}
#endif
	}
}

