/*
 *  lib/gfx/gfx_gui_paint.cpp
 */
/*#####################################################*/
#include "stdbool.h"
#include "gfx_gui_paint.h"
#include "api/lcd_def.h"
#include "util.h"
#include "controls_definition.h"
/*#####################################################*/
Gfx_res_t sys_def_gui_res;
/*#####################################################*/
void _gui_put_simple_item(gfx_s32 x, gfx_s32 y, gfx_s32 x_size, gfx_s32 y_size, gfx_u8 fill, gfx_u32 color)
{
	gfx_s32 radius = sys_def_gui_res.corner_radius;
	if(radius < 0) radius = ~radius;
	gfx_s32 a, b, P;
	GI::Dev::Screen *pDisplay = sys_def_gui_res.pDisplay;
	gfx_s32 __x_size = x_size, __y_size = y_size;
	if(__x_size < 0) __x_size = ~__x_size;
	if(__y_size < 0) __y_size = ~__y_size;
	gfx_u32 _color = color;// << 8;

   a = 0;
   b = radius;
   P = 1 - radius;
   gfx_s32 Tmp1;
   gfx_s32 Tmp2;
   gfx_s32 Tmp3;
   gfx_s32 Tmp4;
   gfx_s32 Tmp5;
   gfx_s32 Tmp6;
   gfx_s32 Tmp7;
   gfx_s32 Tmp8;
   gfx_s32 _Tmp5 = 5, _Tmp7 = 0;

   do
   {
	      Tmp1 = ((x + __x_size + a)- radius - 1);
	      Tmp2 = ((x + radius) - a);
	      Tmp3 = ((x + __x_size + b)- radius - 1);
	      Tmp4 = ((x + radius) - b);
	      Tmp5 = ((y + __y_size + a)- radius - 1);
	      Tmp6 = ((y + radius) - a);
	      Tmp7 = ((y + __y_size + b)- radius - 1);
	      Tmp8 = ((y + radius) - b);
	  if(fill)
      {

		 if(_Tmp7 != Tmp7)
		 {
			 pDisplay->drawHLine(Tmp2, Tmp1 - Tmp2, Tmp7, 1, color);
			 pDisplay->drawHLine(Tmp2, Tmp1 - Tmp2, Tmp8, 1, color);
		 }
         if(_Tmp5 != Tmp5)
		 {
        	 pDisplay->drawHLine(Tmp4, Tmp3 - Tmp4, Tmp5, 1, color);
        	 pDisplay->drawHLine(Tmp4, Tmp3 - Tmp4, Tmp6, 1, color);
		 }
      _Tmp5 = Tmp5;
      _Tmp7 = Tmp7;
      }
      else
      {
    	  pDisplay->drawPixel(Tmp1, Tmp7, _color);
    	  pDisplay->drawPixel(Tmp3, Tmp5, _color);
    	  pDisplay->drawPixel(Tmp2, Tmp7, _color);
    	  pDisplay->drawPixel(Tmp4, Tmp5, _color);
    	  pDisplay->drawPixel(Tmp3, Tmp6, _color);
    	  pDisplay->drawPixel(Tmp1, Tmp8, _color);
    	  pDisplay->drawPixel(Tmp2, Tmp8, _color);
    	  pDisplay->drawPixel(Tmp4, Tmp6, _color);
      }

      if(P < 0)
         P += 3 + 2 * a++;
      else
         P += 5 + 2 * (a++ - b--);
    } while(a <= b);
   if(!fill)
   {
	   pDisplay->drawHLine(x + radius + 1, x_size - (radius + radius) - 2, y, 1, color);
	   pDisplay->drawHLine(x + radius + 1, x_size - (radius + radius) - 2, (y + y_size) - 1, 1, color);
	   pDisplay->drawVLine(y + radius + 1, y_size - (radius + radius) - 2, x, 1, color);
	   pDisplay->drawVLine(y + radius + 1, y_size - (radius + radius) - 2, (x + x_size) - 1, 1, color);
   } else {
	   pDisplay->drawRectangle( x, y + radius + 1, x_size, y_size - (radius + radius) - 2, true, color);
   }
}
/*#####################################################*/
void gui_put_item(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, gfx_u32 int_color, gfx_u32 border_color, CursorState cursor, GUI_PAINT_STYLE style, bool enabled)
{
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion.sXMin = x_start;
	LcdStruct->sClipRegion.sYMin = y_start;
	LcdStruct->sClipRegion.sXMax = x_start + x_len;
	LcdStruct->sClipRegion.sYMax = y_start + y_len;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	sys_def_gui_res.pDisplay = LcdStruct;
	switch(style) {
		case PAINT_STYLE_ROUNDED_CORNERS:
			sys_def_gui_res.corner_radius = 5;
			_gui_put_simple_item(x_start, y_start, x_len, y_len, true, int_color);
			_gui_put_simple_item(x_start, y_start, x_len, y_len, false, controlls_change_color(border_color, -BORDER_LINE_ONE_DARK));
			break;
		case PAINT_STYLE_DEFAULT:
		default:
			if(enabled)
			{
				switch(cursor) {
					case Cursor_Down:
					case Cursor_Move:
						LcdStruct->drawRectangle( x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, int_color);
						LcdStruct->drawHLine( x_start, x_len, y_start, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_DARK));
						LcdStruct->drawHLine( x_start + 1, x_len - 2, y_start + 1, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_DARK));

						LcdStruct->drawHLine( x_start + 1, x_len - 2, y_start + y_len - 2, 1, controlls_change_color(border_color, +BORDER_LINE_TWO_LIGHT));
						LcdStruct->drawHLine( x_start, x_len, (y_start + y_len) - 1, 1, controlls_change_color(border_color, +BORDER_LINE_ONE_LIGHT));

						LcdStruct->drawVLine( y_start, y_len, x_start, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_DARK));
						LcdStruct->drawVLine( y_start + 1, y_len - 2, x_start + 1, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_DARK));

						LcdStruct->drawVLine( y_start + 1, y_len - 2, x_start + x_len - 2, 1, controlls_change_color(border_color, +BORDER_LINE_TWO_LIGHT));
						LcdStruct->drawVLine( y_start, y_len, (x_start + x_len) - 1, 1, controlls_change_color(border_color, +BORDER_LINE_ONE_LIGHT));
						break;
					default:
						LcdStruct->drawRectangle( x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, int_color);
						LcdStruct->drawHLine( x_start, x_len, y_start, 1, controlls_change_color(border_color, +BORDER_LINE_ONE_LIGHT));
						LcdStruct->drawHLine( x_start + 1, x_len - 2, y_start + 1, 1, controlls_change_color(border_color, +BORDER_LINE_TWO_LIGHT));

						LcdStruct->drawHLine( x_start + 1, x_len - 2, y_start + y_len - 2, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_DARK));
						LcdStruct->drawHLine( x_start, x_len, (y_start + y_len) - 1, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_DARK));

						LcdStruct->drawVLine( y_start, y_len, x_start, 1, controlls_change_color(border_color, +BORDER_LINE_ONE_LIGHT));
						LcdStruct->drawVLine( y_start + 1, y_len - 2, x_start + 1, 1, controlls_change_color(border_color, +BORDER_LINE_TWO_LIGHT));

						LcdStruct->drawVLine( y_start + 1, y_len - 2, x_start + x_len - 2, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_DARK));
						LcdStruct->drawVLine( y_start, y_len, (x_start + x_len) - 1, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_DARK));
						break;
				}
			} else
			{
				LcdStruct->drawRectangle( x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, int_color);
				LcdStruct->drawHLine( x_start, x_len, y_start, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_LIGHT));
				LcdStruct->drawHLine( x_start + 1, x_len - 2, y_start + 1, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_LIGHT));

				LcdStruct->drawHLine( x_start + 1, x_len - 2, y_start + y_len - 2, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_LIGHT));
				LcdStruct->drawHLine( x_start, x_len, (y_start + y_len) - 1, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_LIGHT));

				LcdStruct->drawVLine( y_start, y_len, x_start, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_LIGHT));
				LcdStruct->drawVLine( y_start + 1, y_len - 2, x_start + 1, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_LIGHT));

				LcdStruct->drawVLine( y_start + 1, y_len - 2, x_start + x_len - 2, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_LIGHT));
				LcdStruct->drawVLine( y_start, y_len, (x_start + x_len) - 1, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_LIGHT));
		}
	}
	LcdStruct->sClipRegion = back_up_clip;
}
/*#####################################################*/
bool gui_paint_def_init(void) {
	return false;
}
/*#####################################################*/
bool gui_paint_item(Gfx_paint_res_t *resource) {
	return false;
}
/*#####################################################*/
/*#####################################################*/



