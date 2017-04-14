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
#include <include/derivative.h>
#include <driver/uart.h>

extern CfgUart uartCfg[];
extern unsigned long FCPU;
#if (USE_DRIVER_SEMAPHORE == true)
volatile bool uart_semaphore[UART_INTERFACE_COUNT];
#endif
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
		memset(this, 0, sizeof(*this));
		memcpy(&cfg, &uartCfg[item_nr], sizeof(CfgUart));
		unitNr = dev_nr;
	}
	else
	{
		err = SYS_ERR_INVALID_PARAM;
		return;
	}
	if(unitNr > 2)
	{
		err = SYS_ERR_OUT_OF_RANGE;
		return;
	}
	UART_Type *addr_table[] = UART_BASE_PTRS;
	udata = (void *)addr_table[unitNr];
	//SIM_SCGC |=  SIM_SCGC_UART2_MASK;		/* Enable bus clock in UART2*/
	//UART_BDH_REG(addr_table[unitNr]) = 0;							/* One stop bit*/
	//UART_BDL_REG(addr_table[unitNr]) = 128;						/* Baud rate at 9600*/
	//UART_C1_REG(addr_table[unitNr])  = 0;							/* No parity enable,8 bit format*/
	//UART_C2_REG(addr_table[unitNr]) |= UART_C2_TE_MASK;			/* Enable Transmitter*/
	//UART_C2_REG(addr_table[unitNr]) |= UART_C2_RE_MASK;			/* Enable Receiver*/
	//UART_C2_REG(addr_table[unitNr]) |= UART_C2_RIE_MASK;			/* Enable Receiver interrupts*/
	UART_ConfigType UartConfig;
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
	UART_Init(addr_table[unitNr], &UartConfig);

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
	UART_Type *UartHandle = (UART_Type *) udata;
	UART_ConfigBaudrateType Baud;
	Baud.u32SysClkHz = FCPU;
	Baud.u32Baudrate = baudRate;
	UART_SetBaudrate(UartHandle, &Baud);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setWordLen(CfgUart::wordLen_e wLen)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_Type *UartHandle = (UART_Type *) udata;

	if(cfg.wordLen == CfgUart::WORD_LEN_8)
		UART_Set8BitMode(UartHandle);
	else
		UART_Set9BitMode(UartHandle);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setStopBits(CfgUart::stopBits_e sBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_Type *UartHandle = (UART_Type *) udata;
	if(cfg.stopBits == CfgUart::STOP_BITS_TWO)
		UART_Set2StopBit(UartHandle);
	else
		UART_Set1StopBit(UartHandle);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setParBits(CfgUart::parity_e pBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_Type *UartHandle = (UART_Type *) udata;
	if(cfg.stopBits == CfgUart::STOP_BITS_ONE)
		UartHandle->BDH &= ~UART_BDH_SBNS_MASK;
	else
		UartHandle->BDH |= UART_BDH_SBNS_MASK;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getSpeed(unsigned long *baudRate)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_Type *UartHandle = (UART_Type *) udata;
	u32 tmpBaud = (UART_BDH_SBR(UartHandle->BDH) << 8) + UartHandle->BDL;
	tmpBaud = tmpBaud << 4;
	*baudRate = FCPU / tmpBaud;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getWordLen(CfgUart::wordLen_e *wLen)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_Type *UartHandle = (UART_Type *) udata;
	if(UartHandle->C1 & UART_C1_M_MASK)
		*wLen = CfgUart::WORD_LEN_9;
	else
		*wLen = CfgUart::WORD_LEN_8;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getStopBits(CfgUart::stopBits_e *sBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_Type *UartHandle = (UART_Type *) udata;
	if(UartHandle->C1 & UART_BDH_SBNS_MASK)
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
	UART_Type *UartHandle = (UART_Type *) udata;
	if(UartHandle->C1 & UART_C1_PE_MASK)
	{
		if(UartHandle->C1 & UART_C1_PT_MASK)
			*pBits = CfgUart::PAR_ODD;
		else
			*pBits = CfgUart::PAR_EVEN;
	}
	else
		*pBits = CfgUart::PAR_NONE;
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
	UART_PutChar((UART_Type*) udata, byteTx);
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
	data = UART_GetChar((UART_Type*) udata);
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
	if (!(((UART_Type*)udata)->S1 & UART_S1_TDRE_MASK))
	{
#if (USE_DRIVER_SEMAPHORE == true)
			uart_semaphore[unitNr] = false;
#endif
		return false;
	}
	UART_WriteDataReg((UART_Type*)udata, byteTx);
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
	if (!UART_CharPresent((UART_Type*)udata))
	{
#if (USE_DRIVER_SEMAPHORE == true)
			uart_semaphore[unitNr] = false;
#endif
		return -1;	 //UARTCharPutNonBlocking((USART_TypeDef*)BaseAddr, byteTx);
	}
	data = UART_ReadDataReg((UART_Type*)udata);
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
