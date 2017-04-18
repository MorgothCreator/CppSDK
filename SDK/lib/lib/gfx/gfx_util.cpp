/*
 *  lib/gfx/gfx_util.cpp
 */

#include "gfx_util.h"
//#######################################################################################
/*gfx_u32 gfx_modify_color(gfx_u32 color, s8 modify)
{
	if(modify == 0) return color;
	gfx_u8 R = color;
	gfx_u8 G = color>>8;
	gfx_u8 B = color>>16;
	if (modify > 0)
	{
		R = (R<<modify);
		G = (G<<modify);
		B = (B<<modify);
	} 
	else
	{
		gfx_u8 _modify = 255-modify;
		R = R>>_modify;
		G = G>>_modify;
		B = B>>_modify;
	}
	return R | (G<<8) | (B<<16);
}*/
//#######################################################
