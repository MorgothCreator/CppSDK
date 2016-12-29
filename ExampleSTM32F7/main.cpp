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


#include <api/init.h>
#include <stdio.h>

#include "lib/gfx/window.h"

#include "lib/gfx/PasswordWindowNumeric.h"

#include "device/lepton_flir.h"

#include <lib/gfx/libdrawtext/drawtext.h>

GI::Sensor::LeptonFLIR *leptonFLIR = NULL;

GI::Screen::Gfx::Window *MainWindow = NULL;
GI::Screen::Gfx::TextBox *SensorResultTextboxGlobal;

#if _USE_LEPTON_FLIR == 1
GI::Screen::Gfx::Picturebox *FlirPictureBox_Global;
unsigned short flir_buff[LEPTON_FLIR_LINE_SIZE * LEPTON_FLIR_LINES_NR] __attribute__ ((aligned (2)));
Color::ARGB flir_buff_translated[LEPTON_FLIR_LINE_SIZE * LEPTON_FLIR_LINES_NR] __attribute__ ((aligned (4)));
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
	GI::Sys::Timer timer_touch = GI::Sys::Timer();
	timer_touch.interval(20);
	GI::Sys::Timer read_flir = GI::Sys::Timer();
	read_flir.interval(1);

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
    u32 cnt = 0;
    char listbox_buff[32];
	char buff_tmp[10];
	/*
	 * Populate list box with 256 items.
	 */
    for(; cnt < 256; cnt++)
    {
		strcpy((char *)listbox_buff, "Device ID ");
		utoa(cnt + 1, buff_tmp, 10);
		strcat(listbox_buff, buff_tmp);
		ListBox->add(listbox_buff);
    }

    leptonFLIR = new GI::Sensor::LeptonFLIR((s8 *)"spi-1.0", (s8 *)"", GI::Sensor::LeptonFLIR::MIDLE_HEAT);

    newWindowPasswordNumeric(MainWindow, pass, 2, 2);

	dtx_target_raster((unsigned char *)MainWindow->Internals.pDisplay->DisplayData, MainWindow->Internals.pDisplay->LcdTimings->X, MainWindow->Internals.pDisplay->LcdTimings->Y);
	dtx_open_font("serif.ttf", 32);
	//dtx_use_font(font, 32);

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
				dev.UART[1]->printF("XY%u %u\n", dev.CAPTOUCH[0]->TouchResponse.x1, dev.CAPTOUCH[0]->TouchResponse.y1);
			}
			control_comand.Cursor = (CursorState)dev.CAPTOUCH[0]->TouchResponse.touch_event1;
			control_comand.X = dev.CAPTOUCH[0]->TouchResponse.x1;
			control_comand.Y = dev.CAPTOUCH[0]->TouchResponse.y1;
			MainWindow->idle(&control_comand);
			if(pass->idle())
				pass->internals.windowHandler->Visible = false;
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
		dtx_color(0, 0, 0, 255);
		dtx_position(0, 0);
		dtx_printf("hello world!");
	}
}

