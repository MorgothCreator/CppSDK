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

/*#####################################################*/
GI::Dev::Uart::Uart(ioSettings *cfg)
{
	memset(this, 0, sizeof(*this));
	if(cfg->info.ioType != ioSettings::info_s::ioType_UART)
		return;

	if(strncmp(cfg->info.name, (char *)"uart-", sizeof("uart-") - 1) && strncmp(cfg->info.name, (char *)"usbcdc-", sizeof("usbcdc-") - 1))
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
		unitNr = dev_nr;
	}
	else
	{
		err = SYS_ERR_INVALID_PARAM;
		return;
	}

	struct usart_config usart_conf;
	/* Configure USART for unit test output */
	usart_get_config_defaults(&usart_conf);
	Sercom *hw;
	switch (unitNr)
	{
	case 0:
		hw = SERCOM0;
		usart_conf.pinmux_pad0 = SERCOM0_PAD0_DEFAULT;
		usart_conf.pinmux_pad1 = SERCOM0_PAD1_DEFAULT;
		break;
	case 1:
		hw = SERCOM1;
		usart_conf.pinmux_pad0 = SERCOM1_PAD0_DEFAULT;
		usart_conf.pinmux_pad1 = SERCOM1_PAD1_DEFAULT;
		break;
	case 2:
		hw = SERCOM2;
		usart_conf.pinmux_pad0 = SERCOM2_PAD0_DEFAULT;
		usart_conf.pinmux_pad1 = SERCOM2_PAD1_DEFAULT;
		break;
	case 3:
		hw = SERCOM3;
		usart_conf.pinmux_pad0 = SERCOM3_PAD0_DEFAULT;
		usart_conf.pinmux_pad1 = SERCOM3_PAD1_DEFAULT;
		break;
	case 4:
		hw = SERCOM4;
		usart_conf.pinmux_pad0 = SERCOM4_PAD0_DEFAULT;
		usart_conf.pinmux_pad1 = SERCOM4_PAD1_DEFAULT;
		break;
	case 5:
		hw = SERCOM5;
		usart_conf.pinmux_pad0 = SERCOM5_PAD0_DEFAULT;
		usart_conf.pinmux_pad1 = SERCOM5_PAD1_DEFAULT;
		break;
	default:
		err = SYS_ERR_DEVICE_NOT_FOUND;
		return;
	}

	struct usart_module *uart_module = (struct usart_module *)calloc(1, sizeof(struct usart_module));
	if(!uart_module)
	{
		err = SYS_ERR_OUT_OF_MEMORY;
		return;
	}
	
	this->cfg = cfg;
	CfgUart *int_cfg = (CfgUart *)cfg->cfg;

	udata = (void *)uart_module;
	usart_conf.mux_setting = USART_RX_1_TX_0_XCK_1;
	usart_conf.pinmux_pad2 = PINMUX_UNUSED;
	usart_conf.pinmux_pad3 = PINMUX_UNUSED;
	usart_conf.baudrate    = int_cfg->speed;

	if( int_cfg->parity == CfgUart::PAR_EVEN)
		usart_conf.parity = USART_PARITY_EVEN;
	else if( int_cfg->parity == CfgUart::PAR_ODD)
		usart_conf.parity = USART_PARITY_ODD;

	if(int_cfg->wordLen == CfgUart::WORD_LEN_5)
		usart_conf.character_size = USART_CHARACTER_SIZE_5BIT;
	else if(int_cfg->wordLen == CfgUart::WORD_LEN_6)
		usart_conf.character_size = USART_CHARACTER_SIZE_6BIT;
	else if(int_cfg->wordLen == CfgUart::WORD_LEN_7)
		usart_conf.character_size = USART_CHARACTER_SIZE_7BIT;
	else if(int_cfg->wordLen == CfgUart::WORD_LEN_9)
		usart_conf.character_size = USART_CHARACTER_SIZE_9BIT;

	if(int_cfg->stopBits == CfgUart::STOP_BITS_TWO)
		usart_conf.stopbits = USART_STOPBITS_2;

	usart_init(uart_module, hw, &usart_conf);
	usart_enable(uart_module);
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

	if(int_cfg->wordLen == CfgUart::WORD_LEN_8)
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
	if(int_cfg->stopBits == CfgUart::STOP_BITS_TWO)
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
	if(int_cfg->stopBits == CfgUart::STOP_BITS_ONE)
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
	//{
		while(STATUS_OK !=usart_write_wait((struct usart_module *)udata, byteTx));
	//}
#if (USE_DRIVER_SEMAPHORE == true)
	uart_semaphore[unitNr] = false;
#endif
}
/*#####################################################*/
unsigned char GI::Dev::Uart::getChar()
{
	unsigned short data = 0;
#if (USE_DRIVER_SEMAPHORE == true)
	while (uart_semaphore[unitNr]);
	uart_semaphore[unitNr] = true;
#endif
	/*if(isVirtual)
		while(((GI::Dev::UsbDCdc *)udata)->rx(&data) == 0);
	else*/
	//{
		while(STATUS_OK != usart_read_wait((struct usart_module *)udata, &data));
	//}
#if (USE_DRIVER_SEMAPHORE == true)
	uart_semaphore[unitNr] = false;
#endif
	return data;
}
/*#####################################################*/
bool GI::Dev::Uart::putCharNb(unsigned char byteTx)
{
#if (USE_DRIVER_SEMAPHORE == true)
	while (uart_semaphore[unitNr]);
	uart_semaphore[unitNr] = true;
#endif
	if (STATUS_OK !=usart_write_wait((struct usart_module *)udata, byteTx))
	{
#if (USE_DRIVER_SEMAPHORE == true)
			uart_semaphore[unitNr] = false;
#endif
		return false;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	uart_semaphore[unitNr] = false;
#endif
	return true;
}
/*#####################################################*/
signed short GI::Dev::Uart::getCharNb()
{
	unsigned short data = 0;
#if (USE_DRIVER_SEMAPHORE == true)
	while (uart_semaphore[unitNr]);
	uart_semaphore[unitNr] = true;
#endif
	if (STATUS_OK != usart_read_wait((struct usart_module *)udata, &data))
	{
#if (USE_DRIVER_SEMAPHORE == true)
			uart_semaphore[unitNr] = false;
#endif
		return -1;	 //UARTCharPutNonBlocking((USART_TypeDef*)BaseAddr, byteTx);
	}
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
