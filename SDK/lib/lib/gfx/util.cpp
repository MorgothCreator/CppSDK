/*
 * util.cpp
 *
 *  Created on: Dec 17, 2016
 *      Author: John Smith
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

bool GI::Screen::Util::insideBox(signed int x_start, signed int y_start, signed int x_len, signed int y_len, signed int x_point, signed int y_point)
{
	signed int x_end = x_start + x_len ,y_end = y_start + y_len;
	if(x_point >= x_start && y_point >= y_start && x_point < x_end && y_point < y_end)
		return true;
	else
		return false;
}




