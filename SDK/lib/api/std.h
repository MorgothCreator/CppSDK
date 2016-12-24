/*
 * std.h
 *
 *  Created on: Dec 14, 2016
 *      Author: John Smith
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
	int vsnprintf (char *str, size_t count, const char *fmt, va_list args);
	void dopr (char *buffer, size_t maxlen, const char *format, va_list args);
	void fmtstr (char *buffer, size_t *currlen, size_t maxlen,
			    char *value, int flags, int min, int max);
	void fmtint (char *buffer, size_t *currlen, size_t maxlen,
			    long value, int base, int min, int max, int flags);
	void fmtfp (char *buffer, size_t *currlen, size_t maxlen,
			   LDOUBLE fvalue, int min, int max, int flags);
	void dopr_outch (char *buffer, size_t *currlen, size_t maxlen, char c );

	void *devHandle;
	void *userData;
};
}


#endif /* LIB_API_STD_H_ */
