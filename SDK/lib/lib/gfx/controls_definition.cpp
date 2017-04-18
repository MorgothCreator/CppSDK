/*
 *  lib/gfx/controls_definition.cpp
 */

#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#endif
#include "controls_definition.h"
#include "util.h"
#include "api/lcd_def.h"
//#include "api/pmic_api.h"
#include "sys/cache.h"
#include "resource/fonts.h"
//#######################################################################################
#if (__SIZEOF_INT__ == 4)
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
	COLOR_24_to_16(0x39,0x39,0x39),
	COLOR_24_to_16(0x00,0x00,0x00),
	COLOR_24_to_16(0xF0,0xF0,0xF0),
	COLOR_24_to_16(0xA0,0xA0,0xA0),
	COLOR_24_to_16(0x80,0x80,0x80),
	COLOR_24_to_16(0x80,0x80,0x80),
	COLOR_24_to_16(0x37,0x37,0x37),
	COLOR_24_to_16(0xF0,0xF0,0xF0),
	COLOR_24_to_16(0x80,0x80,0x80),
	COLOR_24_to_16(0x7F,0x7F,0x7F),

	COLOR_24_to_16(0x39,0x39,0x39),
	//COLOR_24_to_16((0x39)/2,(0x39)/2,(0x39)/2),
	COLOR_24_to_16(0x98, 0x9C, 0xB6),
	//COLOR_24_to_16((0x91)/2,(0x91)/2,(0x91)/2),
	COLOR_24_to_16(0x80,0x80,0x80),
	//COLOR_24_to_16((0xFF)/2,(0xFF)/2,(0xFF)/2),
	COLOR_24_to_16((0x37),(0x37),(0x37)),
	COLOR_24_to_16(0x98, 0x9C, 0xB6),
	COLOR_24_to_16(0x98, 0x9C, 0xB6),
	COLOR_24_to_16(0x98, 0x9C, 0xB6),
//#ifdef USE_FONT_CMSS14B
	(tFont*)&g_sFontCmss14b
//#else
	//(tFont*)NULL
//#endif
};
#endif
//#######################################################################################
#if (__SIZEOF_INT__ == 4)
gfx_u32 controlls_change_color(gfx_u32 color, double value)
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
		return (gfx_u32)((gfx_u32)Red|(((gfx_u32)Green)<<8)|(((gfx_u32)Blue)<<16));
	}
	else if(value<(double)0)
	{
		double Red   = (double)((color)    &0xFF);
		double Green = (double)((color>>8) &0xFF);
		double Blue  = (double)((color>>16)&0xFF);
		Red         /= 0 - value;
		Green       /= 0 - value;
		Blue        /= 0 - value;
		return (gfx_u32)((gfx_u32)Red|(((gfx_u32)Green)<<8)|(((gfx_u32)Blue)<<16));
	}
	return color;
}
#else
gfx_u32 controlls_change_color(gfx_u32 color, double value)
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
		return (gfx_u32)((gfx_u32)Red|(((gfx_u32)Green)<<5)|(((gfx_u32)Blue)<<11));
	}
	else if(value<(double)0)
	{
		double Red   = (double)((color)    &0x1F);
		double Green = (double)((color>>5) &0x3F);
		double Blue  = (double)((color>>11)&0x1F);
		Red         /= 0 - value;
		Green       /= 0 - value;
		Blue        /= 0 - value;
		return (gfx_u32)((gfx_u32)Red|(((gfx_u32)Green)<<5)|(((gfx_u32)Blue)<<11));
	}
	return color;
}
#endif
//#######################################################################################
//bool screen_copy(tDisplay *pDisplayTo, tDisplay *pDisplayFrom, bool put_cursor, gfx_s32 X, gfx_s32 Y, gfx_u32 color)
//{
	/*if(pDisplayTo->raster_timings->X != pDisplayFrom->raster_timings->X || pDisplayTo->raster_timings->Y != pDisplayFrom->raster_timings->Y) return false;
	//memcpy((void *)pDisplayTo->DisplayData, (void *)pDisplayFrom->DisplayData, (sizeof(gfx_u32) * pDisplayFrom->Height * pDisplayFrom->Width) + 32);
	//box_cache_clean(pDisplayTo, 0, 0, pDisplayFrom->Width, pDisplayFrom->Height);
	gfx_s32 LineCnt = 0;
	volatile gfx_u32* ScreenBuff = pDisplayTo->DisplayData + pDisplayTo->raster_timings->palete_len;
	volatile gfx_u32* _ScreenBuff = pDisplayFrom->DisplayData + pDisplayTo->raster_timings->palete_len;
	for(; LineCnt < pDisplayTo->raster_timings->Y; LineCnt ++)
	{
		memcpy((void *)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt)), (void *)(_ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt)), (sizeof(ScreenBuff[0]) * pDisplayTo->raster_timings->X));
		if(put_cursor == true && LineCnt >= Y && LineCnt <= Y + 2)
		{
			gfx_u32 cnt_x = X;
			for(;cnt_x < X + 2; cnt_x++) put_pixel(pDisplayTo, cnt_x, LineCnt, color);
		}
		CacheDataCleanInvalidateBuff((gfx_u32)((gfx_u32*)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt))), (sizeof(ScreenBuff[0]) * pDisplayTo->raster_timings->X) + 64);
	}
	return true;*/
//	return _screen_copy(pDisplayTo, pDisplayFrom, put_cursor, X, Y, color);
//}
//#######################################################################################
