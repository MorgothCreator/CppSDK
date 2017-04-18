/*
 *  api/std.h
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

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "std.cpp"
#endif
/*#####################################################*/

#endif /* LIB_API_STD_H_ */
