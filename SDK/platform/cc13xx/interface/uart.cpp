/*
 * uart_interface.c
 *
 * Created: 2/8/2013 2:06:44 AM
 *  Author: XxXx
 */
/*#####################################################*/
#include <stdio.h>
#include "driver/uart.h"
#include "sys/core_init.h"
#include "driver/ioc.h"

#include <interface/uart.h>
#include "api/init.h"
#include "usb_def.h"

#if (USE_DRIVER_SEMAPHORE == true)
volatile bool uart_semaphore[UART_INTERFACE_COUNT];
#endif

extern CfgUart uartCfg[];

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

    this->cfg = cfg;
    CfgUart *int_cfg = (CfgUart *)cfg->cfg;

    IOCPortConfigureSet((int_cfg->tx % 32), IOC_PORT_MCU_UART0_TX, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
	HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (int_cfg->tx % 32));
	IOCPortConfigureSet((int_cfg->rx % 32), IOC_PORT_MCU_UART0_RX, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE);
	HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->rx % 32));

	unsigned long config = 0;
	switch((unsigned char)int_cfg->wordLen)
	{
	case CfgUart::WORD_LEN_5:
		config = UART_CONFIG_WLEN_5;
		break;
	case CfgUart::WORD_LEN_6:
		config = UART_CONFIG_WLEN_6;
		break;
	case CfgUart::WORD_LEN_7:
		config = UART_CONFIG_WLEN_7;
		break;
	case CfgUart::WORD_LEN_8:
		config = UART_CONFIG_WLEN_8;
		break;
	}
	if(int_cfg->stopBits == CfgUart::STOP_BITS_TWO)
		config |= UART_CONFIG_STOP_TWO;

	switch((unsigned char)int_cfg->parity)
	{
	case CfgUart::PAR_ODD:
		config |= UART_CONFIG_PAR_ODD;
		break;
	case CfgUart::PAR_EVEN:
		config |= UART_CONFIG_PAR_EVEN;
		break;
	case CfgUart::PAR_MASK:
		config |= UART_CONFIG_PAR_MASK;
		break;
	case CfgUart::PAR_ZERO:
		config |= UART_CONFIG_PAR_ZERO;
		break;
	case CfgUart::PAR_ONE:
		config |= UART_CONFIG_PAR_ONE;
		break;
	}


	UARTConfigSetExpClk(UART0_BASE, CoreFreq, int_cfg->speed, config);
	//UARTParityModeSet(UART0_BASE, UART_CONFIG_PAR_NONE);
	UARTEnable(UART0_BASE);
	udata = (void *)UART0_BASE;
}
/*#####################################################*/
GI::Dev::Uart::~Uart()
{
	UARTDisable(UART0_BASE);
	return;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setSpeed(unsigned long baudRate)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
    CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	int_cfg->speed = baudRate;
	unsigned long config = 0;
	switch((unsigned char)int_cfg->wordLen)
	{
	case CfgUart::WORD_LEN_5:
		config = UART_CONFIG_WLEN_5;
		break;
	case CfgUart::WORD_LEN_6:
		config = UART_CONFIG_WLEN_6;
		break;
	case CfgUart::WORD_LEN_7:
		config = UART_CONFIG_WLEN_7;
		break;
	case CfgUart::WORD_LEN_8:
		config = UART_CONFIG_WLEN_8;
		break;
	}
	if(int_cfg->stopBits == CfgUart::STOP_BITS_TWO)
		config |= UART_CONFIG_STOP_TWO;

	switch((unsigned char)int_cfg->parity)
	{
	case CfgUart::PAR_ODD:
		config |= UART_CONFIG_PAR_ODD;
		break;
	case CfgUart::PAR_EVEN:
		config |= UART_CONFIG_PAR_EVEN;
		break;
	case CfgUart::PAR_MASK:
		config |= UART_CONFIG_PAR_MASK;
		break;
	case CfgUart::PAR_ZERO:
		config |= UART_CONFIG_PAR_ZERO;
		break;
	case CfgUart::PAR_ONE:
		config |= UART_CONFIG_PAR_ONE;
		break;
	}

	UARTConfigSetExpClk(UART0_BASE, CoreFreq, int_cfg->speed, config);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setWordLen(CfgUart::wordLen_e wLen)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
    CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	int_cfg->wordLen = wLen;
	unsigned long config = 0;
	switch((unsigned char)int_cfg->wordLen)
	{
	case CfgUart::WORD_LEN_5:
		config = UART_CONFIG_WLEN_5;
		break;
	case CfgUart::WORD_LEN_6:
		config = UART_CONFIG_WLEN_6;
		break;
	case CfgUart::WORD_LEN_7:
		config = UART_CONFIG_WLEN_7;
		break;
	case CfgUart::WORD_LEN_8:
		config = UART_CONFIG_WLEN_8;
		break;
	}
	if(int_cfg->stopBits == CfgUart::STOP_BITS_TWO)
		config |= UART_CONFIG_STOP_TWO;

	switch((unsigned char)int_cfg->parity)
	{
	case CfgUart::PAR_ODD:
		config |= UART_CONFIG_PAR_ODD;
		break;
	case CfgUart::PAR_EVEN:
		config |= UART_CONFIG_PAR_EVEN;
		break;
	case CfgUart::PAR_MASK:
		config |= UART_CONFIG_PAR_MASK;
		break;
	case CfgUart::PAR_ZERO:
		config |= UART_CONFIG_PAR_ZERO;
		break;
	case CfgUart::PAR_ONE:
		config |= UART_CONFIG_PAR_ONE;
		break;
	}

	UARTConfigSetExpClk(UART0_BASE, CoreFreq, int_cfg->speed, config);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setStopBits(CfgUart::stopBits_e sBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
    CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	int_cfg->stopBits = sBits;
	unsigned long config = 0;
	switch((unsigned char)int_cfg->wordLen)
	{
	case CfgUart::WORD_LEN_5:
		config = UART_CONFIG_WLEN_5;
		break;
	case CfgUart::WORD_LEN_6:
		config = UART_CONFIG_WLEN_6;
		break;
	case CfgUart::WORD_LEN_7:
		config = UART_CONFIG_WLEN_7;
		break;
	case CfgUart::WORD_LEN_8:
		config = UART_CONFIG_WLEN_8;
		break;
	}
	if(int_cfg->stopBits == CfgUart::STOP_BITS_TWO)
		config |= UART_CONFIG_STOP_TWO;

	switch((unsigned char)int_cfg->parity)
	{
	case CfgUart::PAR_ODD:
		config |= UART_CONFIG_PAR_ODD;
		break;
	case CfgUart::PAR_EVEN:
		config |= UART_CONFIG_PAR_EVEN;
		break;
	case CfgUart::PAR_MASK:
		config |= UART_CONFIG_PAR_MASK;
		break;
	case CfgUart::PAR_ZERO:
		config |= UART_CONFIG_PAR_ZERO;
		break;
	case CfgUart::PAR_ONE:
		config |= UART_CONFIG_PAR_ONE;
		break;
	}

	UARTConfigSetExpClk(UART0_BASE, CoreFreq, int_cfg->speed, config);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setParBits(CfgUart::parity_e pBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
    CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	int_cfg->parity = pBits;
	unsigned long config = 0;
	switch((unsigned char)int_cfg->wordLen)
	{
	case CfgUart::WORD_LEN_5:
		config = UART_CONFIG_WLEN_5;
		break;
	case CfgUart::WORD_LEN_6:
		config = UART_CONFIG_WLEN_6;
		break;
	case CfgUart::WORD_LEN_7:
		config = UART_CONFIG_WLEN_7;
		break;
	case CfgUart::WORD_LEN_8:
		config = UART_CONFIG_WLEN_8;
		break;
	}
	if(int_cfg->stopBits == CfgUart::STOP_BITS_TWO)
		config |= UART_CONFIG_STOP_TWO;

	switch((unsigned char)int_cfg->parity)
	{
	case CfgUart::PAR_ODD:
		config |= UART_CONFIG_PAR_ODD;
		break;
	case CfgUart::PAR_EVEN:
		config |= UART_CONFIG_PAR_EVEN;
		break;
	case CfgUart::PAR_MASK:
		config |= UART_CONFIG_PAR_MASK;
		break;
	case CfgUart::PAR_ZERO:
		config |= UART_CONFIG_PAR_ZERO;
		break;
	case CfgUart::PAR_ONE:
		config |= UART_CONFIG_PAR_ONE;
		break;
	}

	UARTConfigSetExpClk(UART0_BASE, CoreFreq, int_cfg->speed, config);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getSpeed(unsigned long *baudRate)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
    CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	*baudRate = int_cfg->speed;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getWordLen(CfgUart::wordLen_e *wLen)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
    CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	*wLen = int_cfg->wordLen;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getStopBits(CfgUart::stopBits_e *sBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
    CfgUart *int_cfg = (CfgUart *)cfg->cfg;
	*sBits = int_cfg->stopBits;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getParBits(CfgUart::parity_e *pBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
    CfgUart *int_cfg = (CfgUart *)cfg->cfg;
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
	if(isVirtual)
	{
		while(((GI::Dev::UsbDCdc *)udata)->tx(&byteTx, 1) != 1);
	}
	else
		UARTCharPut(UART0_BASE, byteTx);
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
	if(isVirtual)
		while(((GI::Dev::UsbDCdc *)udata)->rx(&data) == 0);
	else
		data = UARTCharGet(UART0_BASE);
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
	if(isVirtual)
	{
		if(((GI::Dev::UsbDCdc *)udata)->tx(&byteTx, 1) == 1)
		{
#if (USE_DRIVER_SEMAPHORE == true)
			uart_semaphore[unitNr] = false;
#endif
			return false;
		}
	}
	else
	{
		if (!UARTCharPutNonBlocking(UART0_BASE, byteTx))
		{
#if (USE_DRIVER_SEMAPHORE == true)
			uart_semaphore[unitNr] = false;
#endif
			return false;
		}
	}
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
	if(isVirtual)
	{
		unsigned char tmp = 0;
		if(((GI::Dev::UsbDCdc *)udata)->rx(&tmp) == 0)
		{
#if (USE_DRIVER_SEMAPHORE == true)
			uart_semaphore[unitNr] = false;
#endif
			return -1;	 //UARTCharPutNonBlocking((USART_TypeDef*)BaseAddr, byteTx);
		}
		else
		{
			data = tmp;
			return data;
		}
	}
	else
	{
		if ((data = UARTCharGetNonBlocking(UART0_BASE)) < 0)
		{
#if (USE_DRIVER_SEMAPHORE == true)
			uart_semaphore[unitNr] = false;
#endif
			return -1;	 //UARTCharPutNonBlocking((USART_TypeDef*)BaseAddr, byteTx);
		}
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
			vsnprintf(tmp_str, len + 1, pcString, args);
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
			vsnprintf(tmp_str, len + 1, pcString, args);
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
	unsigned int len_cnt = 0;
	for(;len_cnt < len; len_cnt++ )
	{
		putChar(*data++);
	}
	return len_cnt;
}
/*#####################################################*/
