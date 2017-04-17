/*
 *  lib/gfx/bitmap.h
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

#ifndef LIB_LIB_GFX_BITMAP_H_
#define LIB_LIB_GFX_BITMAP_H_

#include <include/global.h>
#include "gfxdef.h"

class Bitmap {
public:
	Bitmap(gfx_s32 width, gfx_s32 height);
	~Bitmap();
	SysErr setPixel(gfx_s32 x, gfx_s32 y, Color::ARGB color);

private:
	Color::ARGB *buff;
	gfx_s32 width;
	gfx_s32 height;
};

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "bitmap.cpp"
#endif
/*#####################################################*/
#endif /* LIB_LIB_GFX_BITMAP_H_ */
