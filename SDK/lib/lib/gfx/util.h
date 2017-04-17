/*
 * util.h
 *
 *  Created on: Dec 17, 2016
 *      Author: John Smith
 */

#ifndef LIB_LIB_GFX_UTIL_H_
#define LIB_LIB_GFX_UTIL_H_


#include <api/cursor_ctl_def.h>
#include "api/lcd_def.h"
#include <lib/gfx/string.h>
#include "gfxdef.h"

namespace GI
{
namespace Screen
{
	class Util {
	public:
		static void clipLimit(tRectangle *limited, tRectangle *limits);
		static bool insideBox(gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, gfx_s32 x_point, gfx_s32 y_point);
	};
}
}




#endif /* LIB_LIB_GFX_UTIL_H_ */
