/*
 *  lib/api/uart_def.h
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
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

#ifndef UART_DEF_H_
#define UART_DEF_H_

#include <stdbool.h>
#include <api/gpio.h>
#include <api/lcd_def.h>
#include <interface/uart.h>
#include <include/global.h>
#include <lib/gfx/string.h>
#include <lib/string.h>
#include <api/std.h>
/*#####################################################*/
typedef struct
{
	char name[9];
	unsigned int tx;
	unsigned int rx;
	unsigned long speed;
	enum wordLen{
#if (UART_HAVE_WORD_LEN_5BITS == 1)
		WORD_LEN_7,
#endif
#if (UART_HAVE_WORD_LEN_6BITS == 1)
		WORD_LEN_6,
#endif
#if (UART_HAVE_WORD_LEN_7BITS == 1)
		WORD_LEN_7,
#endif
		WORD_LEN_8,
#if (UART_HAVE_WORD_LEN_9BITS == 1)
		WORD_LEN_9,
#endif
	}wordLen;
	enum stopBits{
		STOP_BITS_ONE,
#if (UART_HAVE_STOP_BITS_ONE_AND_HALF == 1)
		STOP_BITS_ONE_AND_HALF,
#endif
		STOP_BITS_TWO,
	}stopBits;
	enum parity{
		PAR_NONE,
		PAR_ODD,
		PAR_EVEN
	}parity;
	enum mode
	{
		MODE_ASYNC = 0,
#if (UART_HAVE_MODE_SYNCHRONOUS == 1)
		MODE_SYNC,
#endif
#if (UART_HAVE_MODE_IR == 1)
		MODE_IR,
#endif
#if (UART_HAVE_MODE_SPI == 1)
		MODE_SPI,
#endif
	}mode;
} CfgUart;
/*#####################################################*/
namespace GI
{
namespace Dev
{
class Uart
{
public:
	Uart(const char *path);
	~Uart();
	void setSpeed(unsigned long BaudRate);
	void putChar(unsigned char byteTx);
	unsigned char getChar();
	bool putCharNb(unsigned char byteTx);
	signed short getCharNb();
	void printF(const char *pcString, ...);
	void printF(GI::String *string, ...);
	//void printF(GI::Screen::String *string, ...);
	int print(const char *pcString);
	int print(GI::String *string);
	//int print(GI::Screen::String *string);

	SysErr err;
	CfgUart cfg;
private:
	int vsnprintf (char *str, size_t count, const char *fmt, va_list args);
	void dopr (char *buffer, size_t maxlen, const char *format, va_list args);
	void fmtstr (char *buffer, size_t *currlen, size_t maxlen, char *value, int flags, int min, int max);
	void fmtint (char *buffer, size_t *currlen, size_t maxlen, long value, int base, int min, int max, int flags);
	void fmtfp (char *buffer, size_t *currlen, size_t maxlen, LDOUBLE fvalue, int min, int max, int flags);
	void dopr_outch (char *buffer, size_t *currlen, size_t maxlen, char c );

	bool isVirtual;
	unsigned char unitNr;
	unsigned int BaseAddr;
	void *udata;

};
}
}
/*#####################################################*/

/*#####################################################*/
#endif /* UART_DEF_H_ */
/*#####################################################*/
