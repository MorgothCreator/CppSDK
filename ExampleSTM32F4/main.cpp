/*
 * main.cpp
 *
 *  Created on: Dec 9, 2016
 *      Author: John Smith
 */

#include <api/init.h>
#include <stdio.h>

#include "lib/gfx/window.h"
#include <api/io_handle.h>

#include "lib/gfx/PasswordWindowNumeric.h"

GI::Screen::Gfx::Window *MainWindow = NULL;
GI::Screen::Gfx::TextBox *SensorResultTextboxGlobal;;

int main(void)
{
	GI::Sys::Timer timer_touch = GI::Sys::Timer();
	timer_touch.interval(20);

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
    unsigned int cnt = 0;
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



    newWindowPasswordNumeric(MainWindow, pass, 2, 2);

    /*
     * Put on parent window caption the IP of ETH interface.
     */
#if (USE_LWIP == 1)
    GI::String ip_str_buff = GI::String();
    dev.LWIP[0]->getIp(&ip_str_buff);
	MainWindow->Caption->setTextF("IP:%s\n\r", ip_str_buff.buff);
	bool old_state_ip_assigned = false;
	dev._stdout->printF("IP:%s\n\r", ip_str_buff.buff);
#endif
	tControlCommandData control_comand;

	GI::Sys::Timer blink_timer = GI::Sys::Timer(500);
	GI::IO led_pin = GI::IO((char *)"led-0");

	while(1)
	{
		dev.idle();
#if (USE_LWIP == 1)
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
#endif
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
		if(blink_timer.tick())
		{
			bool state;
			led_pin.read(&state);
			if(state)
				led_pin.write(false);
			else
				led_pin.write(true);
		}
	}

}

