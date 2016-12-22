/*
 * uart_interface.h
 *
 * Created: 2/8/2013 2:07:01 AM
 *  Author: XxXx
 */
/*#####################################################*/
#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_
/*#####################################################*/
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include "include/stm32f7xx.h"
#include "driver/stm32f7xx_hal_conf.h"
#include "driver/stm32f7xx_hal_uart.h"
#include "driver/stm32f7xx_hal_rcc.h"
#include <interface/gpio.h>
#include <interface/uart_def.h>
#include <include/global.h>
#include <lib/string.h>
#include <lib/gfx/string.h>
/*#####################################################*/
#ifdef HAVE_LONG_DOUBLE
#define LDOUBLE long double
#else
#define LDOUBLE double
#endif
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
	unsigned char getChar();bool putCharNb(unsigned char byteTx);
	signed short getCharNb();
	void printF(const char *pcString, ...);
	void printF(GI::String *string, ...);
	void printF(GI::Screen::String *string, ...);
	int print(const char *pcString);
	int print(GI::String *string);
	int print(GI::Screen::String *string);

	int err;
	CfgUart cfg;
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

	bool is_virtual;
	unsigned char unitNr;
	unsigned int BaseAddr;
	void *udata;

};
}
}
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "uart_interface.c"
#endif
/*#####################################################*/
#endif /* UART_INTERFACE_H_ */
/*#####################################################*/
