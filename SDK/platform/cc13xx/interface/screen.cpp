/*
 * lcd_interface.c
 *
 * Created: 2/8/2013 10:26:43 PM
 *  Author: XxXx
 */
#include "screen.h"

void GI::Dev::IntScreen::init(LCD_TIMINGS *timings, GI::Dev::Gpio* backlight)
{
	this->backlight = backlight;
	LcdTimings = timings;
	//screen_open(LcdStruct);
}

void GI::Dev::IntScreen::deinit()
{

}
/**********************************************/
void GI::Dev::Screen::setOn()
{
	err = SYS_ERR_NOT_IMPLEMENTED;
	/* Send Display on DCS command to display */
	/*HAL_DSI_ShortWrite(&(hdsi_eval),
	 hdsivideo_handle.VirtualChannelID,
	 DSI_DCS_SHORT_PKT_WRITE_P1,
	 OTM8009A_CMD_DISPON,
	 0x00);*/
}
/**********************************************/
void GI::Dev::Screen::setOff()
{
	err = SYS_ERR_NOT_IMPLEMENTED;
	/* Send Display off DCS Command to display */
	/*HAL_DSI_ShortWrite(&(hdsi_eval),
	 hdsivideo_handle.VirtualChannelID,
	 DSI_DCS_SHORT_PKT_WRITE_P1,
	 OTM8009A_CMD_DISPOFF,
	 0x00);*/
}
//#######################################################################################
int GI::Dev::Screen::setBacklight(unsigned char value)
{
	err = SYS_ERR_OK;
	if (backlight)
	{
		if (value)
			backlight->setOut(1);
		else
			backlight->setOut(0);
		return SYS_ERR_OK;
	}
	err = SYS_ERR_NOT_INITIALIZED;
	return SYS_ERR_NOT_INITIALIZED;
}

//#######################################################################################
bool GI::Dev::Screen::copyScreen(void *_pDisplayFrom, bool put_cursor,
		signed int X, signed int Y, unsigned int color)
{
	return true;
}
//#######################################################################################
void GI::Dev::Screen::cacheClean(signed int x_start, signed int y_start,
		signed int x_len, signed int y_len)
{

}
//#######################################################################################
void GI::Dev::Screen::drawRectangle(signed int x_start, signed int y_start,
		signed int x_len, signed int y_len, bool fill, unsigned int color)
{
}
//#######################################################################################
void GI::Dev::Screen::drawPixel(signed int X, signed int Y, unsigned int color)
{

}
//#######################################################################################
void GI::Dev::Screen::copyRectangle16Bit(unsigned short *rgb_buffer,
		unsigned int x1, unsigned int y1, unsigned int width,
		unsigned int height)
{

}
//#######################################################################################
void GI::Dev::Screen::copyRectangle24Bit(unsigned char *rgb_buffer,
		unsigned long x1, unsigned long y1, unsigned long width,
		unsigned long height)
{

}
//#######################################################################################
void GI::Dev::Screen::copyRectangle32Bit(unsigned char *rgb_buffer,
		unsigned int x1, unsigned int y1, unsigned int width,
		unsigned int height)
{

}
//#######################################################################################
void GI::Dev::Screen::drawHLine(signed int X1, signed int X2, signed int Y,
		unsigned char width, unsigned int color)
{

}
//#######################################################################################
void GI::Dev::Screen::drawVLine(signed int Y1, signed int Y2, signed int X,
		unsigned char width, unsigned int color)
{

}
//#######################################################################################
void GI::Dev::Screen::clear(unsigned int color)
{

}
//#######################################################################################
void GI::Dev::Screen::drawTouchPoint(signed int X, signed int Y,
		unsigned int color)
{

}
//#######################################################################################
//void put_pixel(void *pDisplay, signed int X, signed int Y, unsigned int color);
void GI::Dev::Screen::drawCircle(signed int x, signed int y, signed int _radius,
		unsigned char fill, unsigned int color)
{

}
//#######################################################################################
void GI::Dev::Screen::drawLine(signed int X1, signed int Y1, signed int X2,
		signed int Y2, unsigned char width, unsigned int color)
{

}
//#######################################################################################
void GI::Dev::Screen::drawElipse(signed int xc, signed int yc, signed int _rx,
		signed int _ry, unsigned char Fill, unsigned int color)
{

}
//#######################################################################################

void GI::Dev::Screen::drawTriangle(signed int Ax, signed int Ay, signed int Bx,
		signed int By, signed int Cx, signed int Cy, unsigned char Fill,
		unsigned int color)
{

}
//#######################################################################################
