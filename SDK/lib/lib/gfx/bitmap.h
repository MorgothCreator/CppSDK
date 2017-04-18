/*
 *  lib/gfx/bitmap.h
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
