/*
 *  lib/gfx/bitmap.cpp
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
	if(!this)
		return SYS_ERR_INVALID_HANDLER;
	buff[(x * width) + y] = color;
	return SYS_ERR_OK;
}


