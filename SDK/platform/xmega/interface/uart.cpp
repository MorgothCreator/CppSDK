/*
 * uart.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: John Smith
 */

#include <string.h>
#include <api/uart.h>
#include <api/init.h>
#include "uart.h"
#include <driver/uart.h>

extern CfgUart uartCfg[];
#if (USE_DRIVER_SEMAPHORE == true)
volatile bool uart_semaphore[UART_INTERFACE_COUNT];
#endif
extern unsigned long FCPU;

USART_t* UART_BASE_PTRS[] =
{
#ifdef USARTC0
		&USARTC0,
#else
		NULL,
#endif
#ifdef USARTC1
		&USARTC1,
#else
		NULL,
#endif
#ifdef USARTD0
		&USARTD0,
#else
		NULL,
#endif
#ifdef USARTD1
		&USARTD1,
#else
		NULL,
#endif
#ifdef USARTE0
		&USARTE0,
#else
		NULL,
#endif
#ifdef USARTE1
		&USARTE1,
#else
		NULL,
#endif
#ifdef USARTF0
		&USARTF0,
#else
		NULL,
#endif
#ifdef USARTF1
		&USARTF1
#else
		NULL,
#endif
		};

/*#####################################################*/
GI::Dev::Uart::Uart(ioSettings *cfg)
{
	memset(this, 0, sizeof(*this));
	if(cfg->info.ioType != ioSettings::info_s::ioType_UART)
	return;

	if(strncmp(cfg->info.name, (char *)"uart-", sizeof("uart-") - 1))
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	if(!strncmp(cfg->info.name, (char *)"uart-", sizeof("uart-") - 1))
	{
		unsigned char dev_nr = cfg->info.name[sizeof("uart-") - 1] - '0';
		if(dev_nr >= UART_INTERFACE_COUNT)
		{
			err = SYS_ERR_INVALID_PATH;
			return;
		}
		memset(this, 0, sizeof(*this));
		unitNr = dev_nr;
	}
	else
	{
		err = SYS_ERR_INVALID_PARAM;
		return;
	}

	this->cfg = cfg;
	CfgUart *int_cfg = (CfgUart *)cfg->cfg;

#if defined(__AVR_ATxmega8E5__) || defined(__AVR_ATxmega16E5__) || defined(__AVR_ATxmega32E5__)
	switch(unitNr)
	{
	case 0:
		PORTC.REMAP &= ~PORT_USART0_bm;
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
		{
			PORTC.DIRSET = 1<<1;
			PORTC.OUTSET = 1<<1;
		}
#endif
		PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc;
		PORTC.DIRCLR = 1<<2;
		PORTC.DIRSET = 1<<3;
		PORTC.OUTSET = 1<<3;
		udata = (void *)UART_BASE_PTRS[0];
		break;
	case 1:
		PORTC.REMAP |= PORT_USART0_bm;
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
		{
			PORTC.DIRSET = 1<<5;
			PORTC.OUTSET = 1<<5;
		}
#endif
		PORTC.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTC.DIRCLR = 1<<6;
		PORTC.DIRSET = 1<<7;
		PORTC.OUTSET = 1<<7;
		udata = (void *)UART_BASE_PTRS[0];
		break;
	case 2:
		PORTD.REMAP &= ~PORT_USART0_bm;
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
		{
			PORTD.DIRSET = 1<<1;
			PORTD.OUTSET = 1<<1;
		}
#endif
		PORTD.PIN2CTRL = PORT_OPC_PULLUP_gc;
		PORTD.DIRCLR = 1<<2;
		PORTD.DIRSET = 1<<3;
		PORTD.OUTSET = 1<<3;
		udata = (void *)UART_BASE_PTRS[2];
		break;
	case 3:
		PORTD.REMAP |= PORT_USART0_bm;
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
		{
			PORTD.DIRSET = 1<<5;
			PORTD.OUTSET = 1<<5;
		}
#endif
		PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTD.DIRCLR = 1<<6;
		PORTD.DIRSET = 1<<7;
		PORTD.OUTSET = 1<<7;
		udata = (void *)UART_BASE_PTRS[2];
		break;
	default:
		return;
	}
#else
	udata = (void *)UART_BASE_PTRS[unitNr];
	switch(unitNr)
	{
#ifdef USARTC0
	case 0:
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		{
			PORTC.DIRSET = 1<<1;
			PORTC.OUTSET = 1<<1;
		}
#endif
		PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc;
		PORTC.DIRCLR = 1<<2;
		PORTC.DIRSET = 1<<3;
		PORTC.OUTSET = 1<<3;
		break;
#endif
#ifdef USARTC1
	case 1:
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		{
			PORTC.DIRSET = 1<<5;
			PORTC.OUTSET = 1<<5;
		}
#endif
		PORTC.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTC.DIRCLR = 1<<6;
		PORTC.DIRSET = 1<<7;
		PORTC.OUTSET = 1<<7;
		break;
#endif
#ifdef USARTD0
	case 2:
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		{
			PORTD.DIRSET = 1<<1;
			PORTD.OUTSET = 1<<1;
		}
#endif
		PORTD.PIN2CTRL = PORT_OPC_PULLUP_gc;
		PORTD.DIRCLR = 1<<2;
		PORTD.DIRSET = 1<<3;
		PORTD.OUTSET = 1<<3;
		break;
#endif
#ifdef USARTD1
	case 3:
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		{
			PORTD.DIRSET = 1<<5;
			PORTD.OUTSET = 1<<5;
		}
#endif
		PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTD.DIRCLR = 1<<6;
		PORTD.DIRSET = 1<<7;
		PORTD.OUTSET = 1<<7;
		break;
#endif
#ifdef USARTE0
	case 0:
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		{
			PORTD.DIRSET = 1<<1;
			PORTD.OUTSET = 1<<1;
		}
#endif
		PORTD.PIN2CTRL = PORT_OPC_PULLUP_gc;
		PORTD.DIRCLR = 1<<2;
		PORTD.DIRSET = 1<<3;
		PORTD.OUTSET = 1<<3;
		break;
#endif
#ifdef USARTE1
	case 1:
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		{
			PORTD.DIRSET = 1<<5;
			PORTD.OUTSET = 1<<5;
		}
#endif
		PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTD.DIRCLR = 1<<6;
		PORTD.DIRSET = 1<<7;
		PORTD.OUTSET = 1<<7;
		break;
#endif
#ifdef USARTF0
	case 2:
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		{
			PORTF.DIRSET = 1<<1;
			PORTF.OUTSET = 1<<1;
		}
#endif
		PORTF.PIN2CTRL = PORT_OPC_PULLUP_gc;
		PORTF.DIRCLR = 1<<2;
		PORTF.DIRSET = 1<<3;
		PORTF.OUTSET = 1<<3;
		break;
#endif
#ifdef USARTF1
	case 3:
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC || int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(int_cfg->uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		{
			PORTF.DIRSET = 1<<5;
			PORTF.OUTSET = 1<<5;
		}
#endif
		PORTF.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTF.DIRCLR = 1<<6;
		PORTF.DIRSET = 1<<7;
		PORTF.OUTSET = 1<<7;
		break;
#endif
	default:
		return;
	}
#endif
	unsigned int ubrr = ((FCPU / 8) / int_cfg->speed) -1;
	((USART_t*)udata)->BAUDCTRLA = (unsigned char)ubrr;
	((USART_t*)udata)->BAUDCTRLB = (unsigned char)((ubrr>>8) & 0x0F);
	unsigned char tmp = 0;
	if(int_cfg->wordLen == CfgUart::WORD_LEN_5)
		tmp = USART_CHSIZE_5BIT_gc;
	else if(int_cfg->wordLen == CfgUart::WORD_LEN_6)
		tmp = USART_CHSIZE_6BIT_gc;
	else if(int_cfg->wordLen == CfgUart::WORD_LEN_7)
		tmp = USART_CHSIZE_7BIT_gc;
	else if(int_cfg->wordLen == CfgUart::WORD_LEN_8)
		tmp = USART_CHSIZE_8BIT_gc;
	else if(int_cfg->wordLen == CfgUart::WORD_LEN_9)
		tmp = USART_CHSIZE_9BIT_gc;
	else
		tmp = USART_CHSIZE_8BIT_gc;
		
	if(int_cfg->parity == CfgUart::PAR_EVEN)
		tmp |= USART_PMODE_EVEN_gc;
	else if(int_cfg->parity == CfgUart::PAR_ODD)
		tmp |= USART_PMODE_ODD_gc;
		
#if UART_HAVE_MODE_SYNCHRONOUS == 1
	if(int_cfg->uartMode == CfgUart::MODE_SYNC)
	{
		tmp |= USART_CMODE_SYNCHRONOUS_gc;
	}
#endif
#if UART_HAVE_MODE_SPI == 1
	if(int_cfg->uartMode == CfgUart::MODE_SPI)
	{
		tmp |= USART_CMODE_MSPI_gc;
	}
#endif
#if UART_HAVE_MODE_IR == 1
	if(int_cfg->uartMode == CfgUart::MODE_IR)
	{
		tmp |= USART_CMODE_IRDA_gc;
	}
#endif

	if(int_cfg->stopBits == CfgUart::STOP_BITS_TWO)
		tmp |= USART_SBMODE_bm;
	((USART_t*)udata)->CTRLC = tmp;
	tmp = USART_CLK2X_bm;
	if(int_cfg->rx)
		tmp |= USART_RXEN_bm;
	if(int_cfg->tx)
		tmp |= USART_TXEN_bm;
	((USART_t*)udata)->CTRLB = tmp;
}
/*#####################################################*/
GI::Dev::Uart::~Uart()
{

}
/*#####################################################*/
/*#####################################################*/
SysErr GI::Dev::Uart::setSpeed(unsigned long baudRate)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	unsigned int ubrr = ((FCPU / 8) / int_cfg->speed) -1;
	((USART_t*)udata)->BAUDCTRLA = (unsigned char)ubrr;
	((USART_t*)udata)->BAUDCTRLB = (unsigned char)((ubrr>>8) & 0x0F);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setWordLen(CfgUart::wordLen_e wLen)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	unsigned char tmp = ((USART_t*)udata)->CTRLC & ~USART_CHSIZE_gm;
	CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	if(int_cfg->wordLen == CfgUart::WORD_LEN_5)
		tmp = USART_CHSIZE_5BIT_gc;
	else if(int_cfg->wordLen == CfgUart::WORD_LEN_6)
		tmp = USART_CHSIZE_6BIT_gc;
	else if(int_cfg->wordLen == CfgUart::WORD_LEN_7)
		tmp = USART_CHSIZE_7BIT_gc;
	else if(int_cfg->wordLen == CfgUart::WORD_LEN_8)
		tmp = USART_CHSIZE_8BIT_gc;
	else if(int_cfg->wordLen == CfgUart::WORD_LEN_9)
		tmp = USART_CHSIZE_9BIT_gc;
	else
		tmp = USART_CHSIZE_8BIT_gc;
	((USART_t*)udata)->CTRLC = tmp;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setStopBits(CfgUart::stopBits_e sBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	unsigned char tmp = ((USART_t*)udata)->CTRLC & ~USART_SBMODE_bm;
	CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	if(int_cfg->stopBits == CfgUart::STOP_BITS_TWO)
		tmp |= USART_SBMODE_bm;
	((USART_t*)udata)->CTRLC = tmp;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setParBits(CfgUart::parity_e pBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	unsigned char tmp = ((USART_t*)udata)->CTRLC & ~USART_PMODE_gm;
	CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	if(int_cfg->parity == CfgUart::PAR_EVEN)
		tmp |= USART_PMODE_EVEN_gc;
	else if(int_cfg->parity == CfgUart::PAR_ODD)
		tmp |= USART_PMODE_ODD_gc;
	((USART_t*)udata)->CTRLC = tmp;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getSpeed(unsigned long *baudRate)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	*baudRate = (FCPU / 8) / ((((USART_t*)udata)->BAUDCTRLB << 8) | ((USART_t*)udata)->BAUDCTRLA);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getWordLen(CfgUart::wordLen_e *wLen)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	//unsigned char tmp = ((USART_t*)udata)->CTRLC & USART_CHSIZE_gm;
	CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	/*if(int_cfg->wordLen == USART_CHSIZE_5BIT_gc)
		*wLen = CfgUart::WORD_LEN_5;
	else if(int_cfg->wordLen == USART_CHSIZE_6BIT_gc)
		*wLen = CfgUart::WORD_LEN_6;
	else if(int_cfg->wordLen == USART_CHSIZE_7BIT_gc)
		*wLen = CfgUart::WORD_LEN_7;
	else if(int_cfg->wordLen == USART_CHSIZE_8BIT_gc)
		*wLen = CfgUart::WORD_LEN_8;
	else if(int_cfg->wordLen == USART_CHSIZE_9BIT_gc)
		*wLen = CfgUart::WORD_LEN_9;
	else
		*wLen = CfgUart::WORD_LEN_8;*/
	*wLen = int_cfg->wordLen;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getStopBits(CfgUart::stopBits_e *sBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	//unsigned char tmp = ((USART_t*)udata)->CTRLC & USART_SBMODE_bm;
	CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	if(int_cfg->stopBits == USART_SBMODE_bm)
		*sBits = CfgUart::STOP_BITS_TWO;
	else
		*sBits = CfgUart::STOP_BITS_ONE;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getParBits(CfgUart::parity_e *pBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	//unsigned char tmp = ((USART_t*)udata)->CTRLC & ~USART_PMODE_gm;
	CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	/*if(int_cfg->parity == USART_PMODE_EVEN_gc)
		*pBits = CfgUart::PAR_EVEN;
	else if(int_cfg->parity == USART_PMODE_ODD_gc)
		*pBits = CfgUart::PAR_ODD;
	else if(int_cfg->parity == USART_PMODE_DISABLED_gc)
		*pBits = CfgUart::PAR_NONE;
	else
		*pBits = CfgUart::PAR_NONE;*/
	*pBits = int_cfg->parity;
	return SYS_ERR_OK;
}
/*#####################################################*/
void GI::Dev::Uart::putChar(unsigned char byteTx)
{
#if (USE_DRIVER_SEMAPHORE == true)
	while (uart_semaphore[unitNr]);
	uart_semaphore[unitNr] = true;
#endif
	/*if(isVirtual)
	{
		while(((GI::Dev::UsbDCdc *)udata)->tx(&byteTx, 1) != 1);
	}
	else*/
	{
		while (!(((USART_t*)udata)->STATUS & (USART_DREIF_bm)));
		/* Put data into buffer, sends the data */
		((USART_t*)udata)->DATA = byteTx;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	uart_semaphore[unitNr] = false;
#endif
}
/*#####################################################*/
unsigned char GI::Dev::Uart::getChar()
{
	unsigned char data = 0;
#if (USE_DRIVER_SEMAPHORE == true)
	while (uart_semaphore[unitNr]);
	uart_semaphore[unitNr] = true;
#endif
	/*if(isVirtual)
		while(((GI::Dev::UsbDCdc *)udata)->rx(&data) == 0);
	else*/
	{
		while ((((USART_t*)udata)->STATUS & (USART_RXCIF_bm)) == 0)
		data = ((USART_t*)udata)->DATA;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	uart_semaphore[unitNr] = false;
#endif
	return data;
}
/*#####################################################*/
bool GI::Dev::Uart::putCharNb(unsigned char byteTx)
{
#if (USE_DRIVER_SEMAPHORE == true)
	while (uart_semaphore[unitNr])
		;
	uart_semaphore[unitNr] = true;
#endif
	if (!(((USART_t*)udata)->STATUS & USART_DREIF_bm))
	{
#if (USE_DRIVER_SEMAPHORE == true)
			uart_semaphore[unitNr] = false;
#endif
		return false;
	}
	((USART_t*)udata)->DATA = byteTx;
#if (USE_DRIVER_SEMAPHORE == true)
	uart_semaphore[unitNr] = false;
#endif
	return true;
}
/*#####################################################*/
signed short GI::Dev::Uart::getCharNb()
{
	signed short data = 0;
#if (USE_DRIVER_SEMAPHORE == true)
	while (uart_semaphore[unitNr])
		;
	uart_semaphore[unitNr] = true;
#endif
	if ((((USART_t*)udata)->STATUS & (USART_RXCIF_bm)) == 0)
	{
#if (USE_DRIVER_SEMAPHORE == true)
			uart_semaphore[unitNr] = false;
#endif
		return -1;	 //UARTCharPutNonBlocking((USART_TypeDef*)BaseAddr, byteTx);
	}
	data = ((USART_t*)udata)->DATA;
#if (USE_DRIVER_SEMAPHORE == true)
	uart_semaphore[unitNr] = false;
#endif
	return data;
}
/*#####################################################*/
void GI::Dev::Uart::printF(const char *pcString, ...)
{
	va_list args;
	va_start (args, pcString);
	char buff[1];
	s32 len = vsnprintf(buff, 0, pcString, args);
	va_end (args);
	if(len > 0)
	{
		char *tmp_str = (char *)malloc(len + 1);
		if(tmp_str)
		{
			va_start (args, pcString);
			vsnprintf(tmp_str, len, pcString, args);
			va_end (args);
			print(tmp_str);
			free(tmp_str);
		}
	}
}
/*#####################################################*/
void GI::Dev::Uart::printF(GI::String *string, ...)
{
	va_list args;
	char *pcString = string->buff;
	va_start (args, pcString);
	char buff[1];
	s32 len = vsnprintf(buff, 0, pcString, args);
	va_end (args);
	if(len > 0)
	{
		char *tmp_str = (char *)malloc(len + 1);
		if(tmp_str)
		{
			va_start (args, pcString);
			vsnprintf(tmp_str, len, pcString, args);
			va_end (args);
			print(tmp_str);
			free(tmp_str);
		}
	}
}
/*#####################################################*/
int GI::Dev::Uart::print(const char *pcString)
{
	if(!this)
		return 0;
	unsigned int count = 0;

	while('\0' != *pcString)
	{
		putChar((char)*pcString);
		pcString++;
		count++;

	}
	return count;
}
/*#####################################################*/
int GI::Dev::Uart::print(GI::String *string)
{
	if(!this)
		return 0;
	unsigned int count = 0;
	char *pcString = string->buff;
	while('\0' != *pcString)
	{
		putChar((char)*pcString);
		pcString++;
		count++;

	}
	return count;
}
/*#####################################################*/
/*int GI::Dev::Uart::print(GI::Screen::String *string)
{
	if(!this)
		return 0;
	unsigned int count = 0;
	char *pcString = string->buff;
	while('\0' != *pcString)
	{
		putChar((char)*pcString);
		pcString++;
		count++;

	}
	return count;
}*/
/*#####################################################*/
int GI::Dev::Uart::write(char *data, unsigned int len)
{
	unsigned int len_cnt = len;
	while(!len_cnt--)
	{
		putChar(*data++);
	}
	return len - len_cnt;
}
/*#####################################################*/
