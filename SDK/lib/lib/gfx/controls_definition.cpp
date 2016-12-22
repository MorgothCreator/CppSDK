/*
 *  lib/gfx/controls_definition.c
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
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

#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#endif
#include "sys/plat_properties.h"
#include "controls_definition.h"
#include "util.h"
#include "api/lcd_def.h"
//#include "api/pmic_api.h"
#include "sys/cache.h"
#include "resource/fonts.h"
//#######################################################################################
#ifndef USE_16_BIT_COLOR_DEPTH
controlscolor controls_color =
{
	(0x39<<16)|(0x39<<8)|(0x39),
	(0x00<<16)|(0x00<<8)|(0x00),
	(0xF0<<16)|(0xF0<<8)|(0xF0),
	(0xA0<<16)|(0xA0<<8)|(0xA0),
	(0x80<<16)|(0x80<<8)|(0x80),
	(0x80<<16)|(0x80<<8)|(0x80),
	(0x37<<16)|(0x37<<8)|(0x37),
	(0xF0<<16)|(0xF0<<8)|(0xF0),
	(0x80<<16)|(0x80<<8)|(0x80),
	(0x7F<<16)|(0x7F<<8)|(0x7F),

	((0x39<<16)  )|((0x39<<8)  )|((0x39)  ),
	//((0x39<<16)/2)|((0x39<<8)/2)|((0x39)/2),
	0x989CB6,//((0x37<<16)  )|((0x37<<8)  )|((0x37)  ),
	//((0x91<<16)/2)|((0x91<<8)/2)|((0x91)/2),
	0x808080,//((0x37<<16)  )|((0x37<<8)  )|((0x37)  ),
	//((0xFF<<16)/2)|((0xFF<<8)/2)|((0xFF)/2),
	((0x37<<16)  )|((0x37<<8)  )|((0x37)  ),
	0x989CB6,//((0x37<<16)  )|((0x37<<8)  )|((0x37)  ),
	0x989CB6,//((0x37<<16)  )|((0x37<<8)  )|((0x37)  ),
	0x989CB6,
	(tFont*)&g_sFontCmss14b
};
#else
#define COLOR_24_to_16(R, G, B) ((unsigned)(((R)>>3)&0b11111) | (((G)<<3)&0b11111100000) | (((B)<<8)&0b1111100000000000))
controlscolor controls_color =
{
		.Control_Color_Enabled_Ink_Pull = COLOR_24_to_16(0x39,0x39,0x39),
		.Control_Color_Enabled_Ink_Push = COLOR_24_to_16(0x00,0x00,0x00),
		.Control_Color_Enabled_Buton_Pull = COLOR_24_to_16(0xFF,0xFF,0xFF),
		.Control_Color_Enabled_Buton_Push = COLOR_24_to_16(0x91,0x91,0x91),
		.Control_Color_Enabled_Border_Pull = COLOR_24_to_16(0xFF,0xFF,0xFF),
		.Control_Color_Enabled_Border_Push = COLOR_24_to_16(0xFF,0xFF,0xFF),
		.Control_Color_Enabled_BackGround = COLOR_24_to_16(0x37,0x37,0x37),
		.Control_Color_Enabled_WindowBorder = COLOR_24_to_16(0x55,0x55,0x55),
		.Control_Color_Enabled_WindowHeader = COLOR_24_to_16(0x7F,0x7F,0x7F),

		.Control_Color_Disabled_Ink_Pull = COLOR_24_to_16((0x00)/2,(0x00)/2,(0x00)/2),
	//COLOR_24_to_16((0x39)/2,(0x39)/2,(0x39)/2),
		.Control_Color_Disabled_Buton_Pull = COLOR_24_to_16((0xFF)/2,(0xFF)/2,(0xFF)/2),
	//COLOR_24_to_16((0x91)/2,(0x91)/2,(0x91)/2),
		.Control_Color_Disabled_Border_Pull = COLOR_24_to_16((0xFF)/2,(0xFF)/2,(0xFF)/2),
	//COLOR_24_to_16((0xFF)/2,(0xFF)/2,(0xFF)/2),
		.Control_Color_Disabled_BackGround = COLOR_24_to_16((0x37)/2,(0x37)/2,(0x37)/2),
		.Control_Color_Disabled_WindowBorder = COLOR_24_to_16(0x98, 0x9C, 0xB6),
		.Control_Color_Disabled_WindowHeader = COLOR_24_to_16(0x98, 0x9C, 0xB6),
		.Scren = COLOR_24_to_16(0x98, 0x9C, 0xB6),
#ifdef USE_FONT_CMSS14B
	.DefaultFont = (tFont*)&g_sFontCmss14b
#else
	.DefaultFont = (tFont*)NULL
#endif
};
#endif
//#######################################################################################
#ifndef USE_16_BIT_COLOR_DEPTH
unsigned int controlls_change_color(unsigned int color, double value)
{
	if(value>(double)0)
	{
		double Red   = (double)((color)    &0xFF);
		double Green = (double)((color>>8) &0xFF);
		double Blue  = (double)((color>>16)&0xFF);
		Red         *=value;
		Green       *=value;
		Blue        *=value;
		if(Red>  (double)255)Red  =(double)255;
		if(Green>(double)255)Green=(double)255;
		if(Blue> (double)255)Blue =(double)255;
		return (unsigned int)((unsigned int)Red|(((unsigned int)Green)<<8)|(((unsigned int)Blue)<<16));
	}
	else if(value<(double)0)
	{
		double Red   = (double)((color)    &0xFF);
		double Green = (double)((color>>8) &0xFF);
		double Blue  = (double)((color>>16)&0xFF);
		Red         /= 0 - value;
		Green       /= 0 - value;
		Blue        /= 0 - value;
		return (unsigned int)((unsigned int)Red|(((unsigned int)Green)<<8)|(((unsigned int)Blue)<<16));
	}
	return color;
}
#else
unsigned int controlls_change_color(unsigned int color, double value)
{
	if(value>(double)0)
	{
		double Red   = (double)((color)    &0x1F);
		double Green = (double)((color>>5) &0x3F);
		double Blue  = (double)((color>>11)&0x1F);
		Red         *=value;
		Green       *=value;
		Blue        *=value;
		if(Red>  (double)31)Red  =(double)31;
		if(Green>(double)63)Green=(double)63;
		if(Blue> (double)31)Blue =(double)31;
		return (unsigned int)((unsigned int)Red|(((unsigned int)Green)<<5)|(((unsigned int)Blue)<<11));
	}
	else if(value<(double)0)
	{
		double Red   = (double)((color)    &0x1F);
		double Green = (double)((color>>5) &0x3F);
		double Blue  = (double)((color>>11)&0x1F);
		Red         /= 0 - value;
		Green       /= 0 - value;
		Blue        /= 0 - value;
		return (unsigned int)((unsigned int)Red|(((unsigned int)Green)<<5)|(((unsigned int)Blue)<<11));
	}
	return color;
}
#endif
//#######################################################################################
//bool screen_copy(tDisplay *pDisplayTo, tDisplay *pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color)
//{
	/*if(pDisplayTo->raster_timings->X != pDisplayFrom->raster_timings->X || pDisplayTo->raster_timings->Y != pDisplayFrom->raster_timings->Y) return false;
	//memcpy((void *)pDisplayTo->DisplayData, (void *)pDisplayFrom->DisplayData, (sizeof(unsigned int) * pDisplayFrom->Height * pDisplayFrom->Width) + 32);
	//box_cache_clean(pDisplayTo, 0, 0, pDisplayFrom->Width, pDisplayFrom->Height);
	signed int LineCnt = 0;
	volatile unsigned int* ScreenBuff = pDisplayTo->DisplayData + pDisplayTo->raster_timings->palete_len;
	volatile unsigned int* _ScreenBuff = pDisplayFrom->DisplayData + pDisplayTo->raster_timings->palete_len;
	for(; LineCnt < pDisplayTo->raster_timings->Y; LineCnt ++)
	{
		memcpy((void *)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt)), (void *)(_ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt)), (sizeof(ScreenBuff[0]) * pDisplayTo->raster_timings->X));
		if(put_cursor == true && LineCnt >= Y && LineCnt <= Y + 2)
		{
			unsigned int cnt_x = X;
			for(;cnt_x < X + 2; cnt_x++) put_pixel(pDisplayTo, cnt_x, LineCnt, color);
		}
		CacheDataCleanInvalidateBuff((unsigned int)((unsigned int*)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt))), (sizeof(ScreenBuff[0]) * pDisplayTo->raster_timings->X) + 64);
	}
	return true;*/
//	return _screen_copy(pDisplayTo, pDisplayFrom, put_cursor, X, Y, color);
//}
//#######################################################################################
