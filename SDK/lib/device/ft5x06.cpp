/*
 *  device/ft5x06.cpp
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

#include <device/ft5x06.h>
#include <api/dev_request.h>
bool GI::Screen::Cursor::dataReady()
{
	if(!this)
		return false;
	if(!irqHeandle->in())
		return false;
	else
		return true;
}

GI::Screen::Cursor::~Cursor()
{

}

GI::Screen::Cursor::Cursor(GI::Dev::Screen *pDisplay, char *i2cpath, char *irqPinPath)
{
	if(!pDisplay)
		return;
	memset(this, 0, sizeof(*this));
	this->pDisplay = pDisplay;
	GI::Dev::DevRequest::request(i2cpath, &twiStruct);
	GI::Dev::DevRequest::request(irqPinPath, &irqHeandle);
    if(dataReady())
    {
		unsigned char reg = 0xF9;
		unsigned char result[38];
		if(twiStruct->writeRead(FT5X0X_TWI_ADDR2, &reg, 1, result, 38) == SYS_ERR_OK)
		{
			twi_addr = FT5X0X_TWI_ADDR2;
			touch_max_x = 1024;
			touch_max_y = 600;
			return;
		}
		else if(twiStruct->writeRead(FT5X0X_TWI_ADDR1, &reg, 1, result, 38) == SYS_ERR_OK)
		{
			twi_addr = FT5X0X_TWI_ADDR1;
			touch_max_x = 480;
			touch_max_y = 800;
			return;
		}
		else if(twiStruct->writeRead(0x54, &reg, 1, result, 38) == SYS_ERR_OK)
		{
			twi_addr = 0x54;
			touch_max_x = 480;
			touch_max_y = 800;
			return;
		}
		else if(twiStruct->writeRead(0x00, &reg, 1, result, 38) == SYS_ERR_OK)
		{
			twi_addr = 0x00;
			touch_max_x = 480;
			touch_max_y = 800;
			return;
		}
    }
    //ft5x06_free(structure);
    return;
}

#define ft5x06_TouchIdle_offset 8

tControlCommandData *GI::Screen::Cursor::idle()
{
	if(!this || !irqHeandle->in())
	{
		memset(&cursor_ctl, 0, sizeof(tControlCommandData));
		return &cursor_ctl;
	}
	unsigned char reg = 0xF9;
	unsigned char result[38];
	if(twiStruct->writeRead(twi_addr, &reg, 1, result, 38) != SYS_ERR_OK)
	{
		memset(&cursor_ctl, 0, sizeof(tControlCommandData));
		return &cursor_ctl;
	}
	//if(Response[0] != 0xAA || Response[1] != 0xAA) return 0;
	//TouchScreen_Data_t* TouchResponse = &TouchResponse;
	TouchResponse.touch_point = result[ft5x06_TouchIdle_offset + 1] & 0x0F;
	unsigned char State1 = Gfx_IntTouch_MouseNoAction;
	unsigned char State2 = Gfx_IntTouch_MouseNoAction;
	unsigned char State3 = Gfx_IntTouch_MouseNoAction;
	unsigned char State4 = Gfx_IntTouch_MouseNoAction;
	unsigned char State5 = Gfx_IntTouch_MouseNoAction;
	double X =(double)pDisplay->LcdTimings->X;
	double Y =(double)pDisplay->LcdTimings->Y;
	if(pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE || pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
	{
		X =(double)pDisplay->LcdTimings->Y;
		Y =(double)pDisplay->LcdTimings->X;
	}
	switch(TouchResponse.touch_point)
	{
	case 5:
		TouchResponse.x5 = (X - 1) - (u32)GUtil::Converters::toPercent((u32)0,(u32)touch_max_x,(u32)X,(u32)((result[ft5x06_TouchIdle_offset + 26]<<8) | result[ft5x06_TouchIdle_offset + 27]) & (u32)0x0FFF);
		TouchResponse.y5 = (u32)GUtil::Converters::toPercent((u32)0,(u32)touch_max_y,(u32)Y,(u32)((result[ft5x06_TouchIdle_offset + 28]<<8) | result[ft5x06_TouchIdle_offset + 29]) & (u32)0x0FFF);
		TouchResponse.touch_ID5=(unsigned short)(result[ft5x06_TouchIdle_offset + 29] & 0xF0)>>4;
		//TouchResponse->touch_event5 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 27] & 0xc0) >> 6);
		if(pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE || pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			signed int	x5_Back = TouchResponse.x5;
			TouchResponse.x5 = TouchResponse.y5;
			TouchResponse.y5 = x5_Back;
		}
		if(pDisplay->LcdTimings->orientation == LCD_ORIENTATION_PORTRAIT_FLIP || pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			TouchResponse.x5 = pDisplay->LcdTimings->X - TouchResponse.x5;
			TouchResponse.y5 = pDisplay->LcdTimings->Y - TouchResponse.y5;
		}
		if(!flip_x)
			TouchResponse.x5 = pDisplay->LcdTimings->X - TouchResponse.x5;
		if(flip_y)
			TouchResponse.y5 = pDisplay->LcdTimings->Y - TouchResponse.y5;
		State5 = Gfx_IntTouch_MouseMove;
	case 4:
		TouchResponse.x4 = (X - 1) - (u32)GUtil::Converters::toPercent((u32)0,(u32)touch_max_x,(u32)X,(u32)((result[ft5x06_TouchIdle_offset + 20]<<8) + result[ft5x06_TouchIdle_offset + 21]) & (u32)0x0FFF);
		TouchResponse.y4 = (u32)GUtil::Converters::toPercent((u32)0,(u32)touch_max_y,(u32)Y,(u32)((result[ft5x06_TouchIdle_offset + 22]<<8) + (u32)result[ft5x06_TouchIdle_offset + 23]) & (u32)0x0FFF);
		TouchResponse.touch_ID4=(unsigned short)(result[ft5x06_TouchIdle_offset + 23] & 0xF0)>>4;
		//TouchResponse->touch_event4 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 21] & 0xc0) >> 6);
		if(pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE || pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			signed int	x4_Back = TouchResponse.x4;
			TouchResponse.x4 = TouchResponse.y4;
			TouchResponse.y4 = x4_Back;
		}
		if(pDisplay->LcdTimings->orientation == LCD_ORIENTATION_PORTRAIT_FLIP || pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			TouchResponse.x4 = pDisplay->LcdTimings->X - TouchResponse.x4;
			TouchResponse.y4 = pDisplay->LcdTimings->Y - TouchResponse.y4;
		}
		if(!flip_x)
			TouchResponse.x4 = pDisplay->LcdTimings->X - TouchResponse.x4;
		if(flip_y)
			TouchResponse.y4 = pDisplay->LcdTimings->Y - TouchResponse.y4;
		State4 = Gfx_IntTouch_MouseMove;
	case 3:
		TouchResponse.x3 = (X - 1) - (u32)GUtil::Converters::toPercent((u32)0,(u32)touch_max_x,(u32)X,(u32)((result[ft5x06_TouchIdle_offset + 14]<<8) | result[ft5x06_TouchIdle_offset + 15]) & (u32)0x0FFF);
		TouchResponse.y3 = (u32)GUtil::Converters::toPercent((u32)0,(u32)touch_max_y,(u32)Y,(u32)((result[ft5x06_TouchIdle_offset + 16]<<8) | result[ft5x06_TouchIdle_offset + 17]) & (u32)0x0FFF);
		TouchResponse.touch_ID3=(unsigned short)(result[ft5x06_TouchIdle_offset + 17] & 0xF0)>>4;
		//TouchResponse->touch_event3 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 15] & 0xc0) >> 6);
		if(pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE || pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			signed int	x3_Back = TouchResponse.x3;
			TouchResponse.x3 = TouchResponse.y3;
			TouchResponse.y3 = x3_Back;
		}
		if(pDisplay->LcdTimings->orientation == LCD_ORIENTATION_PORTRAIT_FLIP || pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			TouchResponse.x3 = pDisplay->LcdTimings->X - TouchResponse.x3;
			TouchResponse.y3 = pDisplay->LcdTimings->Y - TouchResponse.y3;
		}
		if(!flip_x)
			TouchResponse.x3 = pDisplay->LcdTimings->X - TouchResponse.x3;
		if(flip_y)
			TouchResponse.y3 = pDisplay->LcdTimings->Y - TouchResponse.y3;
		State3 = Gfx_IntTouch_MouseMove;
	case 2:
		TouchResponse.x2 = (X - 1) - (u32)GUtil::Converters::toPercent((u32)0,(u32)touch_max_x,(u32)X,(u32)((result[ft5x06_TouchIdle_offset + 8]<<8) | result[ft5x06_TouchIdle_offset + 9]) & (u32)0x0FFF);
		TouchResponse.y2 = (u32)GUtil::Converters::toPercent((u32)0,(u32)touch_max_y,(u32)Y,(u32)((result[ft5x06_TouchIdle_offset + 10]<<8) | result[ft5x06_TouchIdle_offset + 11]) & (u32)0x0FFF);
		TouchResponse.touch_ID2=(unsigned short)(result[ft5x06_TouchIdle_offset + 11] & 0xF0)>>4;
		//TouchResponse->touch_event2 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 9] & 0xc0) >> 6);
		if(pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE || pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			signed int	x2_Back = TouchResponse.x2;
			TouchResponse.x2 = TouchResponse.y2;
			TouchResponse.y2 = x2_Back;
		}
		if(pDisplay->LcdTimings->orientation == LCD_ORIENTATION_PORTRAIT_FLIP || pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			TouchResponse.x2 = pDisplay->LcdTimings->X - TouchResponse.x2;
			TouchResponse.y2 = pDisplay->LcdTimings->Y - TouchResponse.y2;
		}
		if(!flip_x)
			TouchResponse.x2 = pDisplay->LcdTimings->X - TouchResponse.x2;
		if(flip_y)
			TouchResponse.y2 = pDisplay->LcdTimings->Y - TouchResponse.y2;
		State2 = Gfx_IntTouch_MouseMove;
	case 1:
		TouchResponse.x1 = (X - 1) - (u32)GUtil::Converters::toPercent((u32)0,(u32)touch_max_x,(u32)X,(u32)((result[ft5x06_TouchIdle_offset + 2]<<8) | result[ft5x06_TouchIdle_offset + 3]) & (u32)0x0FFF);
		TouchResponse.y1 = (u32)GUtil::Converters::toPercent((u32)0,(u32)touch_max_y,(u32)Y,(u32)((result[ft5x06_TouchIdle_offset + 4]<<8) | result[ft5x06_TouchIdle_offset + 5]) & (u32)0x0FFF);
		TouchResponse.touch_ID1=(unsigned short)(result[ft5x06_TouchIdle_offset + 5] & 0xF0)>>4;
		//TouchResponse->touch_event1 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 3] & 0xc0) >> 6);
		if(pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE || pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			TouchResponse.y1 = (double)pDisplay->LcdTimings->X - TouchResponse.y1;
			signed int	x1_Back = TouchResponse.x1;
			TouchResponse.x1 = TouchResponse.y1;
			TouchResponse.y1 = x1_Back;
		}
		if(pDisplay->LcdTimings->orientation == LCD_ORIENTATION_PORTRAIT_FLIP || pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			TouchResponse.x1 = pDisplay->LcdTimings->X - TouchResponse.x1;
			TouchResponse.y1 = pDisplay->LcdTimings->Y - TouchResponse.y1;
		}
		if(!flip_x)
			TouchResponse.x1 = pDisplay->LcdTimings->X - TouchResponse.x1;
		if(flip_y)
			TouchResponse.y1 = pDisplay->LcdTimings->Y - TouchResponse.y1;
		State1 = Gfx_IntTouch_MouseMove;
	}

	//if(timer_tick(&timer1) && LastState1 == Gfx_MouseMove || LastState1 == Gfx_MouseDn) TouchResponse->touch_event1 = Gfx_MouseUp;

	if(LastState1 == Gfx_IntTouch_MouseNoAction && State1 == Gfx_IntTouch_MouseMove)
	{
		LastState1 = Gfx_IntTouch_MouseDn;
		TouchResponse.touch_event1 = Gfx_IntTouch_MouseDn;
	}
	else if((LastState1 == Gfx_IntTouch_MouseDn || LastState1 == Gfx_IntTouch_MouseMove) && State1 == Gfx_IntTouch_MouseMove)
	{
		LastState1 = Gfx_IntTouch_MouseMove;
		TouchResponse.touch_event1 = Gfx_IntTouch_MouseMove;
	}
	else if((LastState1 == Gfx_IntTouch_MouseDn || LastState1 == Gfx_IntTouch_MouseMove) && State1 == Gfx_IntTouch_MouseNoAction)
	{
		LastState1 = Gfx_IntTouch_MouseUp;
		TouchResponse.touch_event1 = Gfx_IntTouch_MouseUp;
	}
	else if(LastState1 == Gfx_IntTouch_MouseUp && State1 == Gfx_IntTouch_MouseNoAction)
	{
		LastState1 = Gfx_IntTouch_MouseNoAction;
		TouchResponse.touch_event1 = Gfx_IntTouch_MouseNoAction;
		TouchResponse.x1 = -1;
		TouchResponse.y1 = -1;
	}

	if(LastState2 == Gfx_IntTouch_MouseNoAction && State2 == Gfx_IntTouch_MouseMove)
	{
		LastState2 = Gfx_IntTouch_MouseDn;
		TouchResponse.touch_event2 = Gfx_IntTouch_MouseDn;
	}
	else if((LastState2 == Gfx_IntTouch_MouseDn || LastState2 == Gfx_IntTouch_MouseMove) && State2 == Gfx_IntTouch_MouseMove)
	{
		LastState2 = Gfx_IntTouch_MouseMove;
		TouchResponse.touch_event2 = Gfx_IntTouch_MouseMove;
	}
	else if((LastState2 == Gfx_IntTouch_MouseDn || LastState2 == Gfx_IntTouch_MouseMove) && State2 == Gfx_IntTouch_MouseNoAction)
	{
		LastState2 = Gfx_IntTouch_MouseUp;
		TouchResponse.touch_event2 = Gfx_IntTouch_MouseUp;
	}
	else if(LastState2 == Gfx_IntTouch_MouseUp && State2 == Gfx_IntTouch_MouseNoAction)
	{
		LastState2 = Gfx_IntTouch_MouseNoAction;
		TouchResponse.touch_event2 = Gfx_IntTouch_MouseNoAction;
		TouchResponse.x2 = -1;
		TouchResponse.y2 = -1;
	}

	if(LastState3 == Gfx_IntTouch_MouseNoAction && State3 == Gfx_IntTouch_MouseMove)
	{
		LastState3 = Gfx_IntTouch_MouseDn;
		TouchResponse.touch_event3 = Gfx_IntTouch_MouseDn;
	}
	else if((LastState3 == Gfx_IntTouch_MouseDn || LastState3 == Gfx_IntTouch_MouseMove) && State3 == Gfx_IntTouch_MouseMove)
	{
		LastState3 = Gfx_IntTouch_MouseMove;
		TouchResponse.touch_event3 = Gfx_IntTouch_MouseMove;
	}
	else if((LastState3 == Gfx_IntTouch_MouseDn || LastState3 == Gfx_IntTouch_MouseMove) && State3 == Gfx_IntTouch_MouseNoAction)
	{
		LastState3 = Gfx_IntTouch_MouseUp;
		TouchResponse.touch_event3 = Gfx_IntTouch_MouseUp;
	}
	else if(LastState3 == Gfx_IntTouch_MouseUp && State3 == Gfx_IntTouch_MouseNoAction)
	{
		LastState3 = Gfx_IntTouch_MouseNoAction;
		TouchResponse.touch_event3 = Gfx_IntTouch_MouseNoAction;
		TouchResponse.x3 = -1;
		TouchResponse.y3 = -1;
	}

	if(LastState4 == Gfx_IntTouch_MouseNoAction && State4 == Gfx_IntTouch_MouseMove)
	{
		LastState4 = Gfx_IntTouch_MouseDn;
		TouchResponse.touch_event4 = Gfx_IntTouch_MouseDn;
	}
	else if((LastState4 == Gfx_IntTouch_MouseDn || LastState4 == Gfx_IntTouch_MouseMove) && State4 == Gfx_IntTouch_MouseMove)
	{
		LastState4 = Gfx_IntTouch_MouseMove;
		TouchResponse.touch_event4 = Gfx_IntTouch_MouseMove;
	}
	else if((LastState4 == Gfx_IntTouch_MouseDn || LastState4 == Gfx_IntTouch_MouseMove) && State4 == Gfx_IntTouch_MouseNoAction)
	{
		LastState4 = Gfx_IntTouch_MouseUp;
		TouchResponse.touch_event4 = Gfx_IntTouch_MouseUp;
	}
	else if(LastState4 == Gfx_IntTouch_MouseUp && State4 == Gfx_IntTouch_MouseNoAction)
	{
		LastState4 = Gfx_IntTouch_MouseNoAction;
		TouchResponse.touch_event4 = Gfx_IntTouch_MouseNoAction;
		TouchResponse.x4 = -1;
		TouchResponse.y4 = -1;
	}

	if(LastState5 == Gfx_IntTouch_MouseNoAction && State5 == Gfx_IntTouch_MouseMove)
	{
		LastState5 = Gfx_IntTouch_MouseDn;
		TouchResponse.touch_event5 = Gfx_IntTouch_MouseDn;
	}
	else if((LastState5 == Gfx_IntTouch_MouseDn || LastState5 == Gfx_IntTouch_MouseMove) && State5 == Gfx_IntTouch_MouseMove)
	{
		LastState5 = Gfx_IntTouch_MouseMove;
		TouchResponse.touch_event5 = Gfx_IntTouch_MouseMove;
	}
	else if((LastState5 == Gfx_IntTouch_MouseDn || LastState5 == Gfx_IntTouch_MouseMove) && State5 == Gfx_IntTouch_MouseNoAction)
	{
		LastState5 = Gfx_IntTouch_MouseUp;
		TouchResponse.touch_event5 = Gfx_IntTouch_MouseUp;
	}
	else if(LastState5 == Gfx_IntTouch_MouseUp && State5 == Gfx_IntTouch_MouseNoAction)
	{
		LastState5 = Gfx_IntTouch_MouseNoAction;
		TouchResponse.touch_event5 = Gfx_IntTouch_MouseNoAction;
		TouchResponse.x5 = -1;
		TouchResponse.y5 = -1;
	}

	memset(&cursor_ctl, 0, sizeof(tControlCommandData));
	cursor_ctl.Cursor = (CursorState)TouchResponse.touch_event1;
	cursor_ctl.X = TouchResponse.x1;
	cursor_ctl.Y = TouchResponse.y1;
	cursor_ctl.Cursor2 = (CursorState)TouchResponse.touch_event2;
	cursor_ctl.X2 = TouchResponse.x2;
	cursor_ctl.Y2 = TouchResponse.y2;
	cursor_ctl.Cursor3 = (CursorState)TouchResponse.touch_event3;
	cursor_ctl.X3 = TouchResponse.x3;
	cursor_ctl.Y3 = TouchResponse.y3;
	cursor_ctl.Cursor4 = (CursorState)TouchResponse.touch_event4;
	cursor_ctl.X4 = TouchResponse.x4;
	cursor_ctl.Y4 = TouchResponse.y4;
	cursor_ctl.Cursor5 = (CursorState)TouchResponse.touch_event5;
	cursor_ctl.X5 = TouchResponse.x5;
	cursor_ctl.Y5 = TouchResponse.y5;
	return &cursor_ctl;
}
