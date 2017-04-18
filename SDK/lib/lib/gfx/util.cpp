/*
 * lib/gfx/util.cpp
 */

#include "util.h"

#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#define read_data_byte(addr) pgm_read_byte(&addr)
#define read_data_word(addr) pgm_read_word(&addr)
#else
#define read_data_byte(addr) addr
#define read_data_word(addr) addr
#endif

void GI::Screen::Util::clipLimit(tRectangle *limited, tRectangle *limits)
{
	if(limited->sXMin < limits->sXMin)
		limited->sXMin = limits->sXMin;
	if(limited->sYMin < limits->sYMin)
		limited->sYMin = limits->sYMin;
	if(limited->sXMax >= limits->sXMax)
		limited->sXMax = limits->sXMax;
	if(limited->sYMax >= limits->sYMax)
		limited->sYMax = limits->sYMax;
}

bool GI::Screen::Util::insideBox(gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, gfx_s32 x_point, gfx_s32 y_point)
{
	gfx_s32 x_end = x_start + x_len ,y_end = y_start + y_len;
	if(x_point >= x_start && y_point >= y_start && x_point < x_end && y_point < y_end)
		return true;
	else
		return false;
}




