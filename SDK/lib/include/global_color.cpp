/*
 *  include/global_color.cpp
 */

#include "global.h"

Color::ARGB Color::fromArgb(u8 red, u8 green, u8 blue, u8 alpha)
{
	Color::ARGB color;
	color.R = red;
	color.G = green;
	color.B = blue;
	color.A = alpha;
	return color;
}

Color::ARGB Color::fromArgb(u8 red, u8 green, u8 blue)
{
	Color::ARGB color;
	color.R = red;
	color.G = green;
	color.B = blue;
	color.A = 0xFF;
	return color;
}
