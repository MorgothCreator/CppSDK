/*
 * uart_interface.c
 *
 * Created: 2/8/2013 2:06:44 AM
 *  Author: XxXx
 */
/*#####################################################*/
#include <stdio.h>
#include "include/stm32f4xx.h"
#include "driver/stm32f4xx_hal_conf.h"
#include "driver/stm32f4xx_hal_uart.h"
#include "driver/stm32f4xx_hal_rcc.h"

#include <interface/uart.h>
#include "api/init.h"
#include "usb_def.h"

#if (USE_DRIVER_SEMAPHORE == true)
volatile bool uart_semaphore[UART_INTERFACE_COUNT];
#endif

extern GPIO_TypeDef *GET_GPIO_PORT_BASE_ADDR[];

extern CfgUart uartCfg[];

USART_TypeDef* COM_USART[] =
{
#ifdef USART1
		USART1,
#endif
#ifdef USART2
		USART2,
#endif
#ifdef USART3
		USART3,
#endif
#ifdef UART4
		UART4,
#endif
#ifdef UART5
		UART5,
#endif
#ifdef USART6
		USART6,
#endif
#ifdef UART7
		UART7,
#endif
#ifdef UART8
		UART8
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
		memset(this, 0, sizeof(*this));
		memcpy(&cfg, &uartCfg[item_nr], sizeof(CfgUart));
		unitNr = dev_nr;
	}
	else 	if(!strncmp(path, (char *)"usbcdc-", sizeof("usbcdc-") - 1))
	{
		unsigned char dev_nr = path[sizeof("usbcdc-") - 1] - '0';
		if(dev_nr >= USB_INTERFACE_COUNT)
		{
			err = SYS_ERR_INVALID_PATH;
			return;
		}
		memset(this, 0, sizeof(*this));
		unitNr = dev_nr;
		udata = (void *) new GI::Dev::UsbDCdc(unitNr);
		isVirtual = true;
		return;
	}
	else
	{
		err = SYS_ERR_INVALID_PARAM;
		return;
	}
	GPIO_InitTypeDef GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	//_gpio_init(UartSettings->TxPort);
	//_gpio_init(UartSettings->RxPort);
	//USARTx_TX_GPIO_CLK_ENABLE();
	//USARTx_RX_GPIO_CLK_ENABLE();
	/* Enable USART1 clock */
	switch (unitNr)
	{
#ifdef __HAL_RCC_USART1_CLK_ENABLE
	case 0:
		__HAL_RCC_USART1_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_USART2_CLK_ENABLE
	case 1:
		__HAL_RCC_USART2_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_USART3_CLK_ENABLE
	case 2:
		__HAL_RCC_USART3_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_UART4_CLK_ENABLE
	case 3:
		__HAL_RCC_UART4_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_UART5_CLK_ENABLE
	case 4:
		__HAL_RCC_UART5_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_USART6_CLK_ENABLE
	case 5:
		__HAL_RCC_USART6_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_UART7_CLK_ENABLE
	case 6:
		__HAL_RCC_UART7_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_UART8_CLK_ENABLE
	case 7:
		__HAL_RCC_UART8_CLK_ENABLE()
		;
		break;
#endif
	}
	//USARTx_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin = 1 << (cfg.tx % 32);
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	switch (unitNr)
	{
	case 0:
	case 1:
	case 2:
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
		break;
	default:
		return;
	}
	HAL_GPIO_Init(GET_GPIO_PORT_BASE_ADDR[cfg.tx >> 5],
			&GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin = 1 << (cfg.rx % 32);
	HAL_GPIO_Init(GET_GPIO_PORT_BASE_ADDR[cfg.rx >> 5],
			&GPIO_InitStruct);

	udata = calloc(1, sizeof(UART_HandleTypeDef));
	if (!udata)
		return;
	UART_HandleTypeDef *UartHandle = (UART_HandleTypeDef *) udata;
	UartHandle->Instance = COM_USART[unitNr];
	UartHandle->Init.BaudRate = cfg.speed;
	UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
	switch((unsigned char)cfg.wordLen)
	{
	case CfgUart::WORD_LEN_9:
		UartHandle->Init.WordLength = UART_WORDLENGTH_9B;
		break;
	}
	UartHandle->Init.StopBits = UART_STOPBITS_1;
	if(cfg.stopBits == CfgUart::STOP_BITS_TWO)
		UartHandle->Init.StopBits = UART_STOPBITS_2;
	UartHandle->Init.Parity = UART_PARITY_NONE;
	switch((unsigned char)cfg.parity)
	{
	case CfgUart::PAR_ODD:
		UartHandle->Init.Parity = UART_PARITY_ODD;
		break;
	case CfgUart::PAR_EVEN:
		UartHandle->Init.Parity = UART_PARITY_EVEN;
		break;
	}
	UartHandle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle->Init.Mode = UART_MODE_TX_RX;
	UartHandle->Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(UartHandle);
}
/*#####################################################*/
GI::Dev::Uart::~Uart()
{
	HAL_UART_DeInit((UART_HandleTypeDef*) udata);
	if (udata)
		free(udata);
	return;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setSpeed(unsigned long baudRate)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_HandleTypeDef *UartHandle = (UART_HandleTypeDef *) udata;
	UartHandle->Init.BaudRate = baudRate;
	UART_SetConfig(UartHandle);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setWordLen(CfgUart::wordLen_e wLen)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_HandleTypeDef *UartHandle = (UART_HandleTypeDef *) udata;
	UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
	switch((unsigned char)cfg.wordLen)
	{
#ifdef UART_WORDLENGTH_7B
	case CfgUart::WORD_LEN_7:
		UartHandle->Init.WordLength = UART_WORDLENGTH_7B;
		break;
#endif
	case CfgUart::WORD_LEN_9:
		UartHandle->Init.WordLength = UART_WORDLENGTH_9B;
		break;
	}
	UART_SetConfig(UartHandle);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setStopBits(CfgUart::stopBits_e sBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_HandleTypeDef *UartHandle = (UART_HandleTypeDef *) udata;
	UartHandle->Init.StopBits = UART_STOPBITS_1;
	if(cfg.stopBits == CfgUart::STOP_BITS_TWO)
		UartHandle->Init.StopBits = UART_STOPBITS_2;
	UART_SetConfig(UartHandle);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::setParBits(CfgUart::parity_e pBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_HandleTypeDef *UartHandle = (UART_HandleTypeDef *) udata;
	UartHandle->Init.Parity = UART_PARITY_NONE;
	switch((unsigned char)cfg.parity)
	{
	case CfgUart::PAR_ODD:
		UartHandle->Init.Parity = UART_PARITY_ODD;
		break;
	case CfgUart::PAR_EVEN:
		UartHandle->Init.Parity = UART_PARITY_EVEN;
		break;
	}
	UART_SetConfig(UartHandle);
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getSpeed(unsigned long *baudRate)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_HandleTypeDef *UartHandle = (UART_HandleTypeDef *) udata;
	*baudRate = UartHandle->Init.BaudRate;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getWordLen(CfgUart::wordLen_e *wLen)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_HandleTypeDef *UartHandle = (UART_HandleTypeDef *) udata;
	switch((int)UartHandle->Init.WordLength)
	{
#ifdef UART_WORDLENGTH_7B
	case UART_WORDLENGTH_7B:
		*wLen = CfgUart::WORD_LEN_7;
		break;
#endif
	case UART_WORDLENGTH_8B:
		*wLen = CfgUart::WORD_LEN_8;
		break;
	case UART_WORDLENGTH_9B:
		*wLen = CfgUart::WORD_LEN_9;
		break;
	}
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getStopBits(CfgUart::stopBits_e *sBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_HandleTypeDef *UartHandle = (UART_HandleTypeDef *) udata;
	if(UartHandle->Init.StopBits == UART_STOPBITS_1)
		*sBits = CfgUart::STOP_BITS_ONE;
	else
		*sBits = CfgUart::STOP_BITS_TWO;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Uart::getParBits(CfgUart::parity_e *pBits)
{
	if(!this || !udata)
		return SYS_ERR_INVALID_HANDLER;
	UART_HandleTypeDef *UartHandle = (UART_HandleTypeDef *) udata;
	switch((int)UartHandle->Init.Parity)
	{
	case UART_PARITY_NONE:
		*pBits = CfgUart::PAR_NONE;
		break;
	case UART_PARITY_ODD:
		*pBits = CfgUart::PAR_ODD;
		break;
	case UART_PARITY_EVEN:
		*pBits = CfgUart::PAR_EVEN;
		break;
	}
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
		HAL_UART_Transmit((UART_HandleTypeDef*) udata, &byteTx, 1, 10);
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
		HAL_UART_Receive((UART_HandleTypeDef*) udata, &data, 1, 10);
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
		HAL_StatusTypeDef status = HAL_UART_Transmit((UART_HandleTypeDef*) udata, &byteTx, 1, 2);
		if (status == HAL_TIMEOUT || status == HAL_BUSY || status == HAL_ERROR)
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
		HAL_StatusTypeDef status = HAL_UART_Receive((UART_HandleTypeDef *) udata, (unsigned char *) &data, 1, 2);
		if (status == HAL_TIMEOUT || status == HAL_BUSY || status == HAL_ERROR)
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
	unsigned int len_cnt = len;
	while(!len_cnt--)
	{
		putChar(*data++);
	}
	return len - len_cnt;
}
/*#####################################################*/
