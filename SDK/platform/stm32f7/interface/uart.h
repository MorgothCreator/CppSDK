/*
 * interface/uart.h
 */
/*#####################################################*/
#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_
/*#####################################################*/
/*#####################################################*/
#ifdef HAVE_LONG_DOUBLE
#define LDOUBLE long double
#else
#define LDOUBLE double
#endif
/*#####################################################*/
#define UART_HAVE_MODE_SYNCHRONOUS			0
#define UART_HAVE_MODE_SPI					0
#define UART_HAVE_MODE_IR					0

#define UART_HAVE_STOP_BITS_ONE_AND_HALF	0

#define UART_HAVE_WORD_LEN_5BITS			0
#define UART_HAVE_WORD_LEN_6BITS			0
#define UART_HAVE_WORD_LEN_7BITS			1
#define UART_HAVE_WORD_LEN_9BITS			1

#define UART_INTERFACE_COUNT				8
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "uart_interface.c"
#endif
/*#####################################################*/
#endif /* UART_INTERFACE_H_ */
/*#####################################################*/
