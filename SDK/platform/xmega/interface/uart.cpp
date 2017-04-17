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
#endif
#ifdef USARTC1
		&USARTC1,
#endif
#ifdef USARTD0
		&USARTD0,
#endif
#ifdef USARTD1
		&USARTD1,
#endif
#ifdef USARTE0
		&USARTE0,
#endif
#ifdef USARTE1
		&USARTE1,
#endif
#ifdef USARTF0
		&USARTF0,
#endif
#ifdef USARTF1
		&USARTF1
#endif
		};

/*#####################################################*/
GI::Dev::Uart::Uart(const char *path)
{
	memset(this, 0, sizeof(*this));
	unsigned int item_nr = 0;
	while(1)
	{
		if(uartCfg[item_nr].name == NULL)
		{
			err = SYS_ERR_INVALID_PATH;
			return;
		}
		if(!strcmp(uartCfg[item_nr].name, path))
			break;
		item_nr++;
	}

	if(strncmp(path, (char *)"uart-", sizeof("uart-") - 1) && strncmp(path, (char *)"usbcdc-", sizeof("usbcdc-") - 1))
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	if(!strncmp(path, (char *)"uart-", sizeof("uart-") - 1))
	{
		unsigned char dev_nr = path[sizeof("uart-") - 1] - '0';
		if(dev_nr >= UART_INTERFACE_COUNT)
		{
			err = SYS_ERR_INVALID_PATH;
			return;
		}
		unitNr = dev_nr;
	}
	else
	{
		err = SYS_ERR_INVALID_PARAM;
		return;
	}
	memset(this, 0, sizeof(*this));
	memcpy(&cfg, &uartCfg[item_nr], sizeof(CfgUart));

	udata = (void *)UART_BASE_PTRS[unitNr];
#if defined(__AVR_ATxmega8E5__) || defined(__AVR_ATxmega16E5__) || defined(__AVR_ATxmega32E5__)
	switch(unitNr)
	{
	case 0:
		PORTC.REMAP &= ~PORT_USART0_bm;
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
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
	case 1:
		PORTC.REMAP |= PORT_USART0_bm;
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
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
	case 2:
		PORTD.REMAP &= ~PORT_USART0_bm;
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
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
	case 3:
		PORTD.REMAP |= PORT_USART0_bm;
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#endif
#if UART_HAVE_MODE_SYNCHRONOUS == 1 || UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
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
	default:
		return;
	}
#else
	switch(unitNr)
	{
#ifdef USARTC0
	case 0:
#if UART_HAVE_MODE_SYNCHRONOUS == 1 && UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
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
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
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
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
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
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
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
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
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
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
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
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
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
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC || cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
#elif UART_HAVE_MODE_SYNCHRONOUS == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SYNC)
#elif UART_HAVE_MODE_SPI == 1
		if(cfg.uartMode == CfgUart::uartMode_e::MODE_SPI)
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
/*	UART_ConfigType UartConfig;
	UartConfig.u32SysClkHz = FCPU;
	UartConfig.u32Baudrate = cfg.speed;
	if(cfg.parity != CfgUart::PAR_NONE)
	{
		UartConfig.sctrl1settings.bits.bPe = true;
		if( cfg.parity == CfgUart::PAR_EVEN)
			UartConfig.sctrl1settings.bits.bPt = false;
		else
			UartConfig.sctrl1settings.bits.bPt = true;

	}
	if(cfg.wordLen == CfgUart::WORD_LEN_8)
		UartConfig.sctrl1settings.bits.bM = false;
	else
		UartConfig.sctrl1settings.bits.bM = true;
	if(cfg.stopBits == CfgUart::STOP_BITS_ONE)
		UartConfig.bSbns = false;
	else
		UartConfig.bSbns = true;
	UART_Init(addr_table[unitNr], &UartConfig);*/

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
	/*USART_t *UartHandle = (UART_Type *) udata;
	UART_ConfigBaudrateType Baud;
	Baud.u32SysClkHz = FCPU;
	Baud.u32Baudrate = baudRate;
	UART_SetBaudrate(UartHandle, &Baud);*/
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setWordLen(CfgUart::wordLen_e wLen)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	/*USART_t *UartHandle = (UART_Type *) udata;

	if(cfg.wordLen == CfgUart::WORD_LEN_8)
		UART_Set8BitMode(UartHandle);
	else
		UART_Set9BitMode(UartHandle);*/
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setStopBits(CfgUart::stopBits_e sBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	/*USART_t *UartHandle = (UART_Type *) udata;
	if(cfg.stopBits == CfgUart::STOP_BITS_TWO)
		UART_Set2StopBit(UartHandle);
	else
		UART_Set1StopBit(UartHandle);*/
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setParBits(CfgUart::parity_e pBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	/*USART_t *UartHandle = (UART_Type *) udata;
	if(cfg.stopBits == CfgUart::STOP_BITS_ONE)
		UartHandle->BDH &= ~UART_BDH_SBNS_MASK;
	else
		UartHandle->BDH |= UART_BDH_SBNS_MASK;*/
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getSpeed(unsigned long *baudRate)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	/*USART_t *UartHandle = (UART_Type *) udata;
	u32 tmpBaud = (UART_BDH_SBR(UartHandle->BDH) << 8) + UartHandle->BDL;
	tmpBaud = tmpBaud << 4;
	*baudRate = FCPU / tmpBaud;*/
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getWordLen(CfgUart::wordLen_e *wLen)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	/*USART_t *UartHandle = (UART_Type *) udata;
	if(UartHandle->C1 & UART_C1_M_MASK)
		*wLen = CfgUart::WORD_LEN_9;
	else
		*wLen = CfgUart::WORD_LEN_8;*/
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getStopBits(CfgUart::stopBits_e *sBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	/*USART_t *UartHandle = (UART_Type *) udata;
	if(UartHandle->C1 & UART_BDH_SBNS_MASK)
		*sBits = CfgUart::STOP_BITS_TWO;
	else
		*sBits = CfgUart::STOP_BITS_ONE;*/
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getParBits(CfgUart::parity_e *pBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	/*USART_t *UartHandle = (UART_Type *) udata;
	if(UartHandle->C1 & UART_C1_PE_MASK)
	{
		if(UartHandle->C1 & UART_C1_PT_MASK)
			*pBits = CfgUart::PAR_ODD;
		else
			*pBits = CfgUart::PAR_EVEN;
	}
	else
		*pBits = CfgUart::PAR_NONE;*/
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
