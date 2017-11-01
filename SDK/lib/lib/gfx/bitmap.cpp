/*
 *  lib/gfx/bitmap.cpp
 */

#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

Bitmap::Bitmap(gfx_s32 width, gfx_s32 height)
{
	memset(this, 0, sizeof(*this));
	if(width <= 0 || height <= 0)
		return;
	buff = (Color::ARGB *)calloc(1, sizeof(Color::ARGB) * width * height);
	if(buff)
	{
		this->width = width;
		this->height = height;
	}
}
Bitmap::~Bitmap()
{
	if(buff)
		free(buff);
}

SysErr Bitmap::setPixel(gfx_s32 x, gfx_s32 y, Color::ARGB color)
{
	if(x < 0 || x >= width || y < 0 || y >= height)
		return SYS_ERR_OUT_OF_RANGE;
	if(!buff)
		return SYS_ERR_BUFF_NOT_ALLOCATED;
	if(!(void *)this)
		return SYS_ERR_INVALID_HANDLER;
	buff[(x * width) + y] = color;
	return SYS_ERR_OK;
}


