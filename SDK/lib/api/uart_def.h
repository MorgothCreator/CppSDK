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
#include <api/gpio_def.h>
#include <interface/uart_def.h>
/*#####################################################*/
//-----------------------------------------------------

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

/*#####################################################*/
#endif /* UART_DEF_H_ */
/*#####################################################*/
