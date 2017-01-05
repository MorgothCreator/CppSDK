/*
 *  api/std.h
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

#ifndef LIB_API_STD_H_
#define LIB_API_STD_H_

#include <stdio.h>
#include <lib/string.h>
#include <api/uart.h>
#include <lib/gfx/window.h>
#include <api/lcd_def.h>
#include <lib/fs/fat/inc/ff.h>

#ifdef HAVE_LONG_DOUBLE
#define LDOUBLE long double
#else
#define LDOUBLE double
#endif

namespace GI
{
class Std {
public:
	Std(char *devPath);
	~Std();

	void print(char Char);
	void print(char* string);
	//void print(GI::Screen::String *string);
	void print(GI::String *string);
	void printF(const char* string, ...);
	//void printF(GI::Screen::String *string, ...);
	void printF(GI::String *string, ...);
	void clear();

	enum {
		STDIO_NONE,
		STDIO_UART,
		STDIO_SCREEN,
		STDIO_FS
	}IoDevices;
private:
	void *devHandle;
	void *userData;
};
}


#endif /* LIB_API_STD_H_ */
