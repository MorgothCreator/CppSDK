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

namespace GI
{
namespace Screen
{
	class Util {
	public:
		static void clipLimit(tRectangle *limited, tRectangle *limits);
		static bool insideBox(signed int x_start, signed int y_start, signed int x_len, signed int y_len, signed int x_point, signed int y_point);
	};
}
}




#endif /* LIB_LIB_GFX_UTIL_H_ */
