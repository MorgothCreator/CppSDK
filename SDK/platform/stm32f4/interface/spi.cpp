/*
 * mcspi_interface.c
 *
 * Created: 3/7/2013 6:41:33 PM
 *  Author: XxXx
 */

//#include "driver/spi.h"
//extern unsigned long CoreFreq;
#include "include/stm32f4xx.h"
#include "driver/stm32f4xx_hal_conf.h"
#include "driver/stm32f4xx_hal_rcc_ex.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/stm32f4xx_hal_spi.h"
#include "driver/stm32f4xx_hal_gpio.h"
#include "driver/stm32f4xx_hal_gpio_ex.h"

#include <interface/spi.h>
#include "api/init.h"
#include <api/spi.h>

#if (USE_DRIVER_SEMAPHORE == true)
volatile bool spi_semaphore[SPI_INTERFACE_COUNT];
#endif

extern GPIO_TypeDef *GET_GPIO_PORT_BASE_ADDR[];

SPI_TypeDef* _SPI_[] =
{
#ifdef SPI1
		SPI1,
#endif
#ifdef SPI2
		SPI2,
#endif
#ifdef SPI3
		SPI3,
#endif
#ifdef SPI4
		SPI4,
#endif
#ifdef SPI5
		SPI5,
#endif
#ifdef SPI6
		SPI6
#endif
		};

/**
 * @brief SPI MSP Initialization
 *        This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 *           - Peripheral's GPIO Configuration
 *           - DMA configuration for transmission request by peripheral
 *           - NVIC configuration for DMA interrupt request enable
 * @param hspi: SPI handle pointer
 * @retval None
 */
GI::Dev::Spi::Spi(ioSettings *cfg)
{
	memset(this, 0, sizeof(*this));
	if(cfg->info.ioType != ioSettings::info_s::ioType_SPI)
		return;

	if(strncmp(cfg->info.name, (char *)"spi-", sizeof("spi-") - 1))
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	unsigned int _portNr = (unsigned int)-1;
	unsigned int _channel = (unsigned int)-1;
	char *port = strchr(cfg->info.name, '-');
	char *chan = strchr(cfg->info.name, '.');
	if(!port || !chan)
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	int _nr_of_items = sscanf(port + 1, (const char *)"%u", &_portNr);
	_nr_of_items += sscanf(chan + 1, (const char *)"%u", &_channel);
	if (_nr_of_items != 2 || _portNr >= SPI_INTERFACE_COUNT || _channel >= SPI_CHANNELS_PER_INTERFACE_COUNT)
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}

	unitNr = _portNr;
	channel = _channel;
	this->cfg = cfg;
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;

	userData = calloc(1, sizeof(SPI_HandleTypeDef));
	if (!userData)
	{
		err = SYS_ERR_OUT_OF_MEMORY;
		return;
	}
	SPI_HandleTypeDef *SpiHandle = (SPI_HandleTypeDef *) userData;
	SpiHandle->Instance = _SPI_[_portNr];
	//SPI_HandleTypeDef *hspi = McspiStruct.

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	//_gpio_init(pinSck >> 5);
	//_gpio_init(pinMiSo >> 5);
	//_gpio_init(pinMoSi >> 5);
	//SPIx_SCK_GPIO_CLK_ENABLE();
	//SPIx_MISO_GPIO_CLK_ENABLE();
	//SPIx_MOSI_GPIO_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	/* Enable SPI3 clock */
	switch ((unsigned int) SpiHandle->Instance)
	{
#ifdef SPI1_BASE
	case SPI1_BASE:
		__HAL_RCC_SPI1_CLK_ENABLE()
		;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		break;
#endif
#ifdef SPI2_BASE
	case SPI2_BASE:
		__HAL_RCC_SPI2_CLK_ENABLE()
		;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
		break;
#endif
#ifdef SPI3_BASE
	case SPI3_BASE:
		__HAL_RCC_SPI3_CLK_ENABLE()
		;
		GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
		break;
#endif
#ifdef SPI4_BASE
	case SPI4_BASE:
		__HAL_RCC_SPI4_CLK_ENABLE()
		;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
		break;
#endif
#ifdef SPI5_BASE
	case SPI5_BASE:
		__HAL_RCC_SPI5_CLK_ENABLE()
		;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
		break;
#endif
#ifdef SPI6_BASE
	case SPI6_BASE:
		__HAL_RCC_SPI6_CLK_ENABLE()
		;
		break;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI6;
		break;
#endif
#ifdef SPI7_BASE
		case SPI7_BASE:
		__HAL_RCC_SPI7_CLK_ENABLE();
		break;
#endif
#ifdef SPI8_BASE
		case SPI8_BASE:
		__HAL_RCC_SPI8_CLK_ENABLE();
		break;
#endif
	default:
		if (!userData)
			free(userData);
		return;
	}

	GPIO_InitStruct.Pin = 1 << (int_cfg->sck % 32);
	HAL_GPIO_Init((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->sck >> 5],
			&GPIO_InitStruct);

	/* SPI MISO GPIO pin configuration  */
	GPIO_InitStruct.Pin = 1 << (int_cfg->miSo % 32);
	HAL_GPIO_Init((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->miSo >> 5],
			&GPIO_InitStruct);

	/* SPI MOSI GPIO pin configuration  */
	GPIO_InitStruct.Pin = 1 << (int_cfg->moSi % 32);
	HAL_GPIO_Init((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->moSi >> 5],
			&GPIO_InitStruct);

	u32 new_speed = 0;
	if(int_cfg->speed > 25000000)
		new_speed = 0;
	else if(int_cfg->speed > 12500000)
		new_speed = 1;
	else if(int_cfg->speed > 6250000)
		new_speed = 2;
	else if(int_cfg->speed > 3125000)
		new_speed = 3;
	else if(int_cfg->speed > 1562500)
		new_speed = 4;
	else if(int_cfg->speed > 781250)
		new_speed = 5;
	else if(int_cfg->speed > 390625)
		new_speed = 6;
	else
		new_speed = 7;

	SpiHandle->Init.BaudRatePrescaler = (new_speed << 3) & SPI_BAUDRATEPRESCALER_256;
	SpiHandle->Init.Direction = SPI_DIRECTION_2LINES;
	SpiHandle->Init.CLKPhase = int_cfg->spiMode & 0x01;
	SpiHandle->Init.CLKPolarity = int_cfg->spiMode & 0x02;
	SpiHandle->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	SpiHandle->Init.CRCPolynomial = 7;
	SpiHandle->Init.DataSize = SPI_DATASIZE_8BIT;
	SpiHandle->Init.FirstBit = SPI_FIRSTBIT_MSB;
	SpiHandle->Init.NSS = SPI_NSS_SOFT;
	SpiHandle->Init.TIMode = SPI_TIMODE_DISABLE;
	SpiHandle->Init.Mode = SPI_MODE_MASTER;
	if (HAL_SPI_Init(SpiHandle) != HAL_OK)
	{
		if (!userData)
			free(userData);
		return;
	}
	//__HAL_SPI_ENABLE(SpiHandle);
	/*##-2- Configure peripheral GPIO ##########################################*/
	/* SPI SCK GPIO pin configuration  */

	//GPIO_InitStructure.Alternate = GPIO_Mode_OUT;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = 1 << (int_cfg->cs % 32);
	HAL_GPIO_Init((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->cs >> 5],
			&GPIO_InitStruct);
	HAL_GPIO_WritePin((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->cs >> 5], (unsigned short) (1 << (int_cfg->cs % 32)), GPIO_PIN_SET);	//OldCsSelect = -1;
	err = SYS_ERR_OK;
	return;
}

/**
 * @brief SPI MSP De-Initialization
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 *          - Revert GPIO, DMA and NVIC configuration to their default state
 * @param hspi: SPI handle pointer
 * @retval None
 */
GI::Dev::Spi::~Spi()
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return;
	}
	SPI_HandleTypeDef *SpiHandle = (SPI_HandleTypeDef *) userData;

	/*##-1- Reset peripherals ##################################################*/
	//SPIx_FORCE_RESET();
	//SPIx_RELEASE_RESET();
	switch ((unsigned int) SpiHandle->Instance)
	{
#ifdef SPI1_BASE
	case SPI1_BASE:
		__HAL_RCC_SPI1_FORCE_RESET();
		__HAL_RCC_SPI1_RELEASE_RESET();
		break;
#endif
#ifdef SPI2_BASE
	case SPI2_BASE:
		__HAL_RCC_SPI2_FORCE_RESET();
		__HAL_RCC_SPI2_RELEASE_RESET();
		break;
#endif
#ifdef SPI3_BASE
	case SPI3_BASE:
		__HAL_RCC_SPI3_FORCE_RESET();
		__HAL_RCC_SPI3_RELEASE_RESET();
		break;
#endif
#ifdef SPI4_BASE
	case SPI4_BASE:
		__HAL_RCC_SPI4_FORCE_RESET();
		__HAL_RCC_SPI4_RELEASE_RESET();
		break;
#endif
#ifdef SPI5_BASE
	case SPI5_BASE:
		__HAL_RCC_SPI5_FORCE_RESET();
		__HAL_RCC_SPI5_RELEASE_RESET();
		break;
#endif
#ifdef SPI6_BASE
	case SPI6_BASE:
		__HAL_RCC_SPI6_FORCE_RESET();
		__HAL_RCC_SPI6_RELEASE_RESET();
		break;
#endif
#ifdef SPI7_BASE
	case SPI7_BASE:
		__HAL_RCC_SPI7_FORCE_RESET();
		__HAL_RCC_SPI7_RELEASE_RESET();
		break;
#endif
#ifdef SPI8_BASE
	case SPI8_BASE:
		__HAL_RCC_SPI8_FORCE_RESET();
		__HAL_RCC_SPI8_RELEASE_RESET();
		break;
#endif
	}

	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	/*##-2- Disable peripherals and GPIO Clocks ################################*/
	/* Configure SPI SCK as alternate function  */
	HAL_GPIO_DeInit(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->sck >> 5],
			(unsigned long) (1 << (int_cfg->sck % 32)));
	/* Configure SPI MISO as alternate function  */
	HAL_GPIO_DeInit(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->miSo >> 5],
			(unsigned long) (1 << (int_cfg->miSo % 32)));
	/* Configure SPI MOSI as alternate function  */
	HAL_GPIO_DeInit(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->moSi >> 5],
			(unsigned long) (1 << (int_cfg->moSi % 32)));

	HAL_GPIO_DeInit(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->cs >> 5],
			(unsigned long) (1 << (int_cfg->cs % 32)));
	HAL_SPI_DeInit(SpiHandle);
	if (!userData)
		free(userData);
	err = SYS_ERR_OK;
	return;
}
/*#####################################################*/
/*#####################################################*/
int GI::Dev::Spi::assert()
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	while (spi_semaphore[unitNr])
		;
#endif
	setSpeed(speed);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = true;
#endif
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	HAL_GPIO_WritePin(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->cs >> 5],
			(unsigned short) (1 << int_cfg->cs % 32), GPIO_PIN_RESET);
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
int GI::Dev::Spi::ioctl(charDev *fp, int command, void *param)
{
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
int GI::Dev::Spi::deassert()
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	HAL_GPIO_WritePin(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->cs >> 5],
			(unsigned short) (1 << int_cfg->cs % 32), GPIO_PIN_SET);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = false;
#endif
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Spi::writeRead(unsigned char *buffWrite, unsigned int lenWrite,
		unsigned char *buffRead, unsigned int lenRead)
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	if (spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
	spi_semaphore[unitNr] = true;
#endif
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!DisableCsHandle)
		HAL_GPIO_WritePin((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->cs >> 5], (unsigned short) (1 << (int_cfg->cs % 32)), GPIO_PIN_RESET);
	SysErr status = SYS_ERR_OK;
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *) userData;
	memset(buffRead, 0xFF, lenRead);

	if (HAL_SPI_Transmit(hspi, buffWrite, lenWrite, 10) != HAL_OK)
		status = SYS_ERR_UNKNOWN;
	if (HAL_SPI_Receive(hspi, buffRead, lenRead, 10) != HAL_OK)
		status = SYS_ERR_UNKNOWN;

	if (!DisableCsHandle)
		HAL_GPIO_WritePin((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->cs >> 5], (unsigned short) (1 << (int_cfg->cs % 32)), GPIO_PIN_SET);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = false;
#endif
	return status;
}
/*#####################################################*/
int GI::Dev::Spi::readBytes(unsigned char *buff, unsigned int len)
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	if (spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
	spi_semaphore[unitNr] = true;
#endif
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!DisableCsHandle)
		HAL_GPIO_WritePin((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->cs >> 5], (unsigned short) (1 << (int_cfg->cs % 32)), GPIO_PIN_RESET);
	SysErr status = SYS_ERR_OK;
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *) userData;
	if (HAL_SPI_Receive(hspi, buff, len, 10) != HAL_OK)
		status = SYS_ERR_UNKNOWN;
	if (!DisableCsHandle)
		HAL_GPIO_WritePin((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->cs >> 5], (unsigned short) (1 << (int_cfg->cs % 32)), GPIO_PIN_SET);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = false;
#endif
	if(status == SYS_ERR_OK)
		return len;
	return status;
}
/*#####################################################*/
int GI::Dev::Spi::writeBytes(unsigned char *buff, unsigned int len)
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	if (spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
	spi_semaphore[unitNr] = true;
#endif
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!DisableCsHandle)
		HAL_GPIO_WritePin((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->cs >> 5], (unsigned short) (1 << (int_cfg->cs % 32)), GPIO_PIN_RESET);
	SysErr status = SYS_ERR_OK;
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *) userData;
	if (HAL_SPI_Transmit(hspi, buff, len, 10) != HAL_OK)
		status = SYS_ERR_UNKNOWN;

	if (!DisableCsHandle)
		HAL_GPIO_WritePin((GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[int_cfg->cs >> 5], (unsigned short) (1 << (int_cfg->cs % 32)), GPIO_PIN_SET);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = false;
#endif
	if(status == SYS_ERR_OK)
		return len;
	return status;
}
/*#####################################################*/
SysErr GI::Dev::Spi::writeReadByte(unsigned char *byte)
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *) userData;
	HAL_SPI_TransmitReceive(hspi, byte, byte, 1, 10);
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Spi::setSpeed(unsigned long baud)
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	while (spi_semaphore[unitNr]);
#endif
	unsigned int new_speed = 0;
	if(baud > 25000000)
		new_speed = 0;
	else if(baud > 12500000)
		new_speed = 1;
	else if(baud > 6250000)
		new_speed = 2;
	else if(baud > 3125000)
		new_speed = 3;
	else if(baud > 1562500)
		new_speed = 4;
	else if(baud > 781250)
		new_speed = 5;
	else if(baud > 390625)
		new_speed = 6;
	else
		new_speed = 7;

	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *) userData;
	if ((hspi->Instance->CR1 & SPI_BAUDRATEPRESCALER_256)
			!= (SPI_BAUDRATEPRESCALER_256 & (new_speed << 3)))
	{
		hspi->Instance->CR1 &= ~SPI_BAUDRATEPRESCALER_256;
		hspi->Instance->CR1 |= SPI_BAUDRATEPRESCALER_256 & (new_speed << 3);
	}
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
