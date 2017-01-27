/*
 *  include/global_color.cpp
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
