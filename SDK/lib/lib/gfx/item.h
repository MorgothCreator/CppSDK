/*
 * item.h
 *
 *  Created on: Dec 15, 2016
 *      Author: John Smith
 */

#ifndef LIB_LIB_GFX_ITEM_H_
#define LIB_LIB_GFX_ITEM_H_

#include <lib/gfx/string.h>
#include "controls_definition.h"

namespace GI
{
namespace Screen
{
namespace Gfx
{
class Item {
public:
	Item(s8* text, gfx_u32 location);

	struct
	{
		gfx_s32 X;
		gfx_s32 Y;
	}Size;
	GI::Screen::String *Caption;
	controls_color_struct_t Color;
	bool CursorDownInsideBox;

};
}
}
}




#endif /* LIB_LIB_GFX_ITEM_H_ */
