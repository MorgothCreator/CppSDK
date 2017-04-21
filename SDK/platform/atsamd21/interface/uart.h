/*
 * uart.h
 *
 *  Created on: Jan 3, 2017
 *      Author: John Smith
 */

#ifndef NXP_KEAZ_INTERFACE_UART_H_
#define NXP_KEAZ_INTERFACE_UART_H_

#include <include/global.h>
#include "sam.h"

#define UART_HAVE_MODE_SYNCHRONOUS			0
#define UART_HAVE_MODE_SPI					0
#define UART_HAVE_MODE_IR					0

#define UART_HAVE_STOP_BITS_ONE_AND_HALF	0

#define UART_HAVE_WORD_LEN_5BITS			1
#define UART_HAVE_WORD_LEN_6BITS			1
#define UART_HAVE_WORD_LEN_7BITS			1
#define UART_HAVE_WORD_LEN_9BITS			1

#define UART_INTERFACE_COUNT				6

#endif /* NXP_KEAZ_INTERFACE_UART_H_ */
