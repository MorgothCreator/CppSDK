/*
 *  main.cpp
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


#define _USE_PASSWORD_PROTECTION	0

#define _USE_HIH613x				0
#define _USE_MPU60x0_9150			0
#define _USE_AK8975					0
#define _USE_BMP180					0
#define _USE_MPL3115A2				0
#define _USE_MPR121					0

#include <api/init.h>
#include <stdio.h>

#include "lib/gfx/window.h"

#include "lib/gfx/PasswordWindowNumeric.h"

#include "device/lepton_flir.h"

#include <lib/gfx/libdrawtext/drawtext.h>

#include <device/hih6130.h>
#include <device/mpu60x0_9x50.h>
#include <device/ak8975.h>
#include <device/bmp180.h>
#include <device/mpl3115a2.h>
#include <device/mpr121.h>

GI::Sensor::LeptonFLIR *leptonFLIR = NULL;

GI::Screen::Gfx::Window *MainWindow = NULL;
GI::Screen::Gfx::TextBox *SensorResultTextboxGlobal;

#if _USE_LEPTON_FLIR == 1
GI::Screen::Gfx::Picturebox *FlirPictureBox_Global;
unsigned short flir_buff[LEPTON_FLIR_LINE_SIZE * LEPTON_FLIR_LINES_NR] __attribute__ ((aligned (2)));
Color::ARGB flir_buff_translated[LEPTON_FLIR_LINE_SIZE * LEPTON_FLIR_LINES_NR] __attribute__ ((aligned (4)));

/* This will be call when a refresh of picture box will occur*/
void *FlirPictureBox_Rfsh_Callback(GI::Screen::Gfx::Picturebox *structure, tControlCommandData *cursor_ctrl)
{
	tRectangle _dest_rectangle;
	_dest_rectangle.sXMin = 0;
	_dest_rectangle.sXMax = LEPTON_FLIR_LINE_SIZE;
	_dest_rectangle.sYMin = 0;
	_dest_rectangle.sYMax = LEPTON_FLIR_LINES_NR;
	tRectangle _src_rectangle;
	_src_rectangle.sXMin = 0;
	_src_rectangle.sXMax = LEPTON_FLIR_LINE_SIZE;
	_src_rectangle.sYMin = 0;
	_src_rectangle.sYMax = LEPTON_FLIR_LINES_NR;
	structure->copyRectangle((gfx_u32 *)flir_buff_translated, 0, &_dest_rectangle, &_src_rectangle, LEPTON_FLIR_LINE_SIZE, LEPTON_FLIR_LINES_NR);
	return NULL;
}
#endif


int main(void)
{
	GI::Sys::Timer timer_touch = GI::Sys::Timer(20);
	GI::Sys::Timer read_flir = GI::Sys::Timer(1);
	GI::Sys::Timer blink_timer = GI::Sys::Timer(500);
	GI::Sys::Timer hih613timer = GI::Sys::Timer(200);
	GI::Sys::Timer mpu60x0_9x50_timer = GI::Sys::Timer(100);
	GI::Sys::Timer bmp180_timer = GI::Sys::Timer(200);
	GI::Sys::Timer mpl3115_timer = GI::Sys::Timer(200);
	GI::Sys::Timer mpr121_timer = GI::Sys::Timer(50);

	/* Get control of "led-0" pin.*/
	GI::IO led_pin = GI::IO((char *)"led-0");
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
#if _USE_LEPTON_FLIR == 1
	FlirPictureBox->Events.Refresh.CallBack = FlirPictureBox_Rfsh_Callback;
#endif

	/*
	 * Create a text box.
	 */
	newTextBox(MainWindow, SensorResultTextbox);
	SensorResultTextboxGlobal = SensorResultTextbox;
	SensorResultTextbox->Position.X = 10;
	SensorResultTextbox->Position.Y = FlirPictureBox->Position.Y + FlirPictureBox->Size.Y + 10;
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
	ProgressBar1->Position.X = 10;
	ProgressBar1->Position.Y = Buton1->Position.Y + Buton1->Size.Y + 10;
	ProgressBar1->Size.X = 460;
	ProgressBar1->Size.Y = 30;
	ProgressBar1->MinimValue  = 0;
	ProgressBar1->MaximValue = 100;
	ProgressBar1->Value = 50;

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
    ListBox->Caption->textAlign = alignCenter;

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
    ListBox->Items[0]->Caption->setText((char *)"aaaa");

    leptonFLIR = new GI::Sensor::LeptonFLIR((s8 *)"spi-1.0", (s8 *)"", GI::Sensor::LeptonFLIR::MIDLE_HEAT);

#if(_USE_PASSWORD_PROTECTION == 1)
    newWindowPasswordNumeric(MainWindow, pass, 2, 2);
#endif

	/*dtx_target_raster((unsigned char *)MainWindow->Internals.pDisplay->DisplayData, MainWindow->Internals.pDisplay->LcdTimings->X, MainWindow->Internals.pDisplay->LcdTimings->Y);
	dtx_open_font("serif.ttf", 32);*/
	//dtx_use_font(font, 32);

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
    /*
     * Put on parent window caption the IP of ETH interface.
     */
    GI::String ip_str_buff = GI::String();
    dev.LWIP[0]->getIp(&ip_str_buff);
	MainWindow->Caption->setTextF("IP:%s\n\r", ip_str_buff.buff);
	tControlCommandData control_comand;
	bool old_state_ip_assigned = false;
	dev._stdout->printF("IP:%s\n\r", ip_str_buff.buff);
	while(1)
	{
		dev.idle();
		if(old_state_ip_assigned == false && dev.LWIP[0]->ipAssigned == true)
		{
			old_state_ip_assigned = true;
		    /*
		     * Update on parent window caption the IP of ETH interface if is assigned by DHCP.
		     */
		    GI::String ip_str_buff = GI::String();
		    dev.LWIP[0]->getIp(&ip_str_buff);
			MainWindow->Caption->setTextF("IP:%s\n\r", ip_str_buff.buff);
			dev._stdout->printF("IP:%s\n\r", ip_str_buff.buff);
		}
		/*
		 * Call every 20ms the idle function of touch screen interface
		 * 	and idle function of parent window.
		 */
		if(timer_touch.tick())
		{
			memset(&control_comand, 0, sizeof(tControlCommandData));
			dev.CAPTOUCH[0]->idle();
			if(dev.CAPTOUCH[0]->TouchResponse.touch_event1)
			{
				dev.UART[1]->printF("X :%u Y:%u\n", dev.CAPTOUCH[0]->TouchResponse.x1, dev.CAPTOUCH[0]->TouchResponse.y1);
			}
			control_comand.Cursor = (CursorState)dev.CAPTOUCH[0]->TouchResponse.touch_event1;
			control_comand.X = dev.CAPTOUCH[0]->TouchResponse.x1;
			control_comand.Y = dev.CAPTOUCH[0]->TouchResponse.y1;
			MainWindow->idle(&control_comand);
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

#if _USE_LEPTON_FLIR == 1
		if(read_flir.tick())
		{
			memset(&flir_buff, 0, (LEPTON_FLIR_LINE_SIZE * LEPTON_FLIR_LINES_NR));
			bool lepton_new_data = leptonFLIR->getImageARGB(flir_buff_translated, 0xFF);
			if(lepton_new_data)
			{
				tRectangle _dest_rectangle;
				_dest_rectangle.sXMin = 0;
				_dest_rectangle.sXMax = LEPTON_FLIR_LINE_SIZE;
				_dest_rectangle.sYMin = 0;
				_dest_rectangle.sYMax = LEPTON_FLIR_LINES_NR;
				tRectangle _src_rectangle;
				_src_rectangle.sXMin = 0;
				_src_rectangle.sXMax = LEPTON_FLIR_LINE_SIZE;
				_src_rectangle.sYMin = 0;
				_src_rectangle.sYMax = LEPTON_FLIR_LINES_NR;
				FlirPictureBox->copyRectangle((gfx_u32 *)flir_buff_translated, 0, &_dest_rectangle, &_src_rectangle, LEPTON_FLIR_LINE_SIZE , LEPTON_FLIR_LINES_NR);
			}
		}
#endif
		if(blink_timer.tick())
		{
			bool state;
			led_pin.read(&state);
			if(state)
				led_pin.write(false);
			else
				led_pin.write(true);
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
						ListBox->Items[0]->Caption->setTextF("HIH613x: T = %2.2f, H = %2.1f", hih613x_temp, hih613x_hum);
						//SensorResultTextbox->text->appendF("HIH613x: T = %2.2f, H = %2.1f\n\r", hih613x_temp, hih613x_hum);
						break;
					case 1:
						ListBox->Items[0]->Caption->setText((char *)"HIH613x: stale data");
						//SensorResultTextbox->text->appendF("HIH613x: stale data\n\r");
						break;
					case 2:
						ListBox->Items[0]->Caption->setText((char *)"HIH613x: in command mode");
						//SensorResultTextbox->text->appendF("HIH613x: in command mode\n\r");
						break;
					case 3:
						ListBox->Items[0]->Caption->setText((char *)"HIH613x: diagnostic");
						//SensorResultTextbox->text->appendF("HIH613x: diagnostic\n\r");
						break;
					}
				}
				else
					SensorResultTextbox->text->append((char *)"HIH613x:  error reading temperature and humidity\n\r");
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
					ListBox->Items[1]->Caption->setTextF("MPU60x0: Temp:  %2.2f Gr Celsius\n\r", mpu60x0_9150_temp);
				else
					ListBox->Items[1]->Caption->setText((char *)"MPU60x0: error reading temperature\n\r");

				if(!mpu60x0_9x50.getGyroData(&mpu60x0_9150_gyro_Xg, &mpu60x0_9150_gyro_Yg, &mpu60x0_9150_gyro_Zg))
					ListBox->Items[2]->Caption->setTextF("MPU60x0: Giro:  Xg = %6d, Yg = %6d, Zg = %6d", mpu60x0_9150_gyro_Xg, mpu60x0_9150_gyro_Yg, mpu60x0_9150_gyro_Zg);
				else
					ListBox->Items[2]->Caption->setText((char *)"MPU60x0: error reading giroscope");

				if(!mpu60x0_9x50.getAccelData(&mpu60x0_9150_accel_Xa, &mpu60x0_9150_accel_Ya, &mpu60x0_9150_accel_Za))
					ListBox->Items[3]->Caption->setTextF("MPU60x0: Accel: Xa = %6d, Ya = %6d, Za = %6d", mpu60x0_9150_accel_Xa, mpu60x0_9150_accel_Ya, mpu60x0_9150_accel_Za);
				else
					ListBox->Items[3]->Caption->setText((char *)"MPU60x0: error reading accelerometer");
			}
#endif
#if (_USE_AK8975 == 1)
			if(mpu60x0_9x50_timer_ticked)
			{
				signed short AK8975_X_Axis = 0, AK8975_Y_Axis = 0, AK8975_Z_Axis = 0;
				if(!ak8975_0.getMag(&AK8975_X_Axis, &AK8975_Y_Axis, &AK8975_Z_Axis))
					ListBox->Items[4]->Caption->setTextF("AK8975: Magne: Xm = %6d, Ym = %6d, Zm = %6d", AK8975_X_Axis, AK8975_Y_Axis, AK8975_Z_Axis);
				else
					ListBox->Items[4]->Caption->setText((char *)"AK8975: error reading magnetometer");
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
					ListBox->Items[5]->Caption->setTextF("BMP180: Temp = %2.1f, Press = %4.2f, Alt = %4.2f", bmp180_temperature, bmp180_pressure, bmp180_altitude);
				else
					ListBox->Items[5]->Caption->setText((char *)"BMP180: error reading temp, press and altitude");
			}
#endif
#if _USE_MPL3115A2 == 1
			if(mpl3115_timer.tick())
			{
				float mpl3115a2_pressure = 0.0, mpl3115a2_altitude = 0.0, mpl3115a2_temp = 0.0;
				if(!mpl3115a2.getAltTemp(2, &mpl3115a2_pressure, &mpl3115a2_altitude, &mpl3115a2_temp))
					ListBox->Items[6]->Caption->setTextF("MPL3115A1: T = %3.3f, P = %3.5f, Alt = %4.3f", mpl3115a2_temp, 0.0, mpl3115a2_altitude);
				else
					ListBox->Items[6]->Caption->setText((char *)"MPL3115A1: error reading temp, press and altitude");
			}
#endif
#if (_USE_MPR121 == 1)
			if(mpr121_timer.tick())
			{
				mpr121_ret_t mpr121_return;
				if(!mpr121_0.idle(&mpr121_return))
				{
					if(mpr121_return.pushed)
						ListBox->Items[7]->Caption->setTextF("MPR121: Pushed   - K0=%u, K1=%u, K2=%u, K3=%u, K4=%u, K5=%u, K6=%u, K7=%u, K8=%u, K9=%u, K10=%u, K11=%u",     (unsigned long)mpr121_return.pushed & 0x01,   (unsigned long)(mpr121_return.pushed >> 1) & 0x01,   (unsigned long)(mpr121_return.pushed >> 2) & 0x01,   (unsigned long)(mpr121_return.pushed >> 3) & 0x01,   (unsigned long)(mpr121_return.pushed >> 4) & 0x01,   (unsigned long)(mpr121_return.pushed >> 5) & 0x01,   (unsigned long)(mpr121_return.pushed >> 6) & 0x01,   (unsigned long)(mpr121_return.pushed >> 7) & 0x01,   (unsigned long)(mpr121_return.pushed >> 8) & 0x01,   (unsigned long)(mpr121_return.pushed >> 9) & 0x01,   (unsigned long)(mpr121_return.pushed >> 10) & 0x01,   (unsigned long)(mpr121_return.pushed >> 11) & 0x01);
					if(mpr121_return.released)
						ListBox->Items[7]->Caption->setTextF("MPR121: Released - K0=%u, K1=%u, K2=%u, K3=%u, K4=%u, K5=%u, K6=%u, K7=%u, K8=%u, K9=%u, K10=%u, K11=%u", (unsigned long)mpr121_return.released & 0x01, (unsigned long)(mpr121_return.released >> 1) & 0x01, (unsigned long)(mpr121_return.released >> 2) & 0x01, (unsigned long)(mpr121_return.released >> 3) & 0x01, (unsigned long)(mpr121_return.released >> 4) & 0x01, (unsigned long)(mpr121_return.released >> 5) & 0x01, (unsigned long)(mpr121_return.released >> 6) & 0x01, (unsigned long)(mpr121_return.released >> 7) & 0x01, (unsigned long)(mpr121_return.released >> 8) & 0x01, (unsigned long)(mpr121_return.released >> 9) & 0x01, (unsigned long)(mpr121_return.released >> 10) & 0x01, (unsigned long)(mpr121_return.released >> 11) & 0x01);
				}
			}
#endif
		}
		/*dtx_color(0, 0, 0, 255);
		dtx_position(0, 0);
		dtx_printf("hello world!");*/
	}
}

