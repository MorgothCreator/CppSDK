/*
 * mcspi_interface.c
 *
 * Created: 3/7/2013 6:41:33 PM
 *  Author: XxXx
 */

//#include "driver/spi.h"
//extern unsigned long CoreFreq;
#include <string.h>
#include <interface/spi.h>
#include <api/init.h>

#if (USE_DRIVER_SEMAPHORE == true)
volatile bool spi_semaphore[SPI_INTERFACE_COUNT];
#endif

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
GI::Dev::Spi::Spi(const char *path)
{
	unsigned int item_nr = 0;
	while(1)
	{
		if(spiCfg[item_nr].name == NULL)
		{
			err = SYS_ERR_INVALID_PATH;
			return;
		}
		if(!strcmp(spiCfg[item_nr].name, path))
			break;
		item_nr++;
	}

	if(strncmp(path, (char *)"spi-", sizeof("spi-") - 1))
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	unsigned int _portNr = (unsigned int)-1;
	unsigned int _channel = (unsigned int)-1;
	char *port = strchr(path, '-');
	char *chan = strchr(path, '.');
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

	memset(this, 0, sizeof(*this));
	unitNr = _portNr;
	channel = _channel;
	memcpy(&cfg, &spiCfg[item_nr], sizeof(CfgSpi));

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
#ifdef __HAL_RCC_SPI1_CLK_ENABLE
	case SPI1_BASE:
		__HAL_RCC_SPI1_CLK_ENABLE()
		;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		break;
#endif
#ifdef __HAL_RCC_SPI2_CLK_ENABLE
	case SPI2_BASE:
		__HAL_RCC_SPI2_CLK_ENABLE()
		;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
		break;
#endif
#ifdef __HAL_RCC_SPI3_CLK_ENABLE
	case SPI3_BASE:
		__HAL_RCC_SPI3_CLK_ENABLE()
		;
		GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
		break;
#endif
#ifdef __HAL_RCC_SPI4_CLK_ENABLE
	case SPI4_BASE:
		__HAL_RCC_SPI4_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_SPI5_CLK_ENABLE
	case SPI5_BASE:
		__HAL_RCC_SPI5_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_SPI6_CLK_ENABLE
	case SPI6_BASE:
		__HAL_RCC_SPI6_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_SPI7_CLK_ENABLE
		case SPI7_BASE:
		__HAL_RCC_SPI7_CLK_ENABLE();
		break;
#endif
#ifdef __HAL_RCC_SPI8_CLK_ENABLE
		case SPI1_BASE:
		__HAL_RCC_SPI8_CLK_ENABLE();
		break;
#endif
	default:
		if (!userData)
			free(userData);
		return;
	}

	GPIO_InitStruct.Pin = 1 << (cfg.sck % 32);
	HAL_GPIO_Init(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.sck >> 5],
			&GPIO_InitStruct);

	/* SPI MISO GPIO pin configuration  */
	GPIO_InitStruct.Pin = 1 << (cfg.miSo % 32);
	HAL_GPIO_Init(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.miSo >> 5],
			&GPIO_InitStruct);

	/* SPI MOSI GPIO pin configuration  */
	GPIO_InitStruct.Pin = 1 << (cfg.moSi % 32);
	HAL_GPIO_Init(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.moSi >> 5],
			&GPIO_InitStruct);

	SpiHandle->Init.BaudRatePrescaler = (cfg.speed << 3)
			& SPI_BAUDRATEPRESCALER_256;
	SpiHandle->Init.Direction = SPI_DIRECTION_2LINES;
	SpiHandle->Init.CLKPhase = cPha & 0x01;
	SpiHandle->Init.CLKPolarity = (cPol & 0x01) << 1;
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
	GPIO_InitStruct.Pin = 1 << (cfg.cs % 32);
	HAL_GPIO_Init(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.cs >> 5],
			&GPIO_InitStruct);
	//OldCsSelect = -1;
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
#if defined(__HAL_RCC_SPI1_FORCE_RESET)
	case SPI1_BASE:
		__HAL_RCC_SPI1_FORCE_RESET();
		__HAL_RCC_SPI1_RELEASE_RESET();
		break;
#endif
#if defined(__HAL_RCC_SPI2_FORCE_RESET)
	case SPI2_BASE:
		__HAL_RCC_SPI2_FORCE_RESET();
		__HAL_RCC_SPI2_RELEASE_RESET();
		break;
#endif
#if defined(__HAL_RCC_SPI3_FORCE_RESET)
	case SPI3_BASE:
		__HAL_RCC_SPI3_FORCE_RESET();
		__HAL_RCC_SPI3_RELEASE_RESET();
		break;
#endif
#if defined(__HAL_RCC_SPI4_FORCE_RESET)
	case SPI4_BASE:
		__HAL_RCC_SPI4_FORCE_RESET();
		__HAL_RCC_SPI4_RELEASE_RESET();
		break;
#endif
#ifdef __HAL_RCC_SPI5_FORCE_RESET
	case SPI5_BASE:
		__HAL_RCC_SPI5_FORCE_RESET();
		__HAL_RCC_SPI5_RELEASE_RESET();
		break;
#endif
#ifdef __HAL_RCC_SPI6_FORCE_RESET
	case SPI6_BASE:
		__HAL_RCC_SPI6_FORCE_RESET();
		__HAL_RCC_SPI6_RELEASE_RESET();
		break;
#endif
	}

	/*##-2- Disable peripherals and GPIO Clocks ################################*/
	/* Configure SPI SCK as alternate function  */
	HAL_GPIO_DeInit(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.sck >> 5],
			(unsigned long) (1 << (cfg.sck % 32)));
	/* Configure SPI MISO as alternate function  */
	HAL_GPIO_DeInit(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.miSo >> 5],
			(unsigned long) (1 << (cfg.miSo % 32)));
	/* Configure SPI MOSI as alternate function  */
	HAL_GPIO_DeInit(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.moSi >> 5],
			(unsigned long) (1 << (cfg.moSi % 32)));

	HAL_GPIO_DeInit(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.cs >> 5],
			(unsigned long) (1 << (cfg.cs % 32)));
	HAL_SPI_DeInit(SpiHandle);
	if (!userData)
		free(userData);
	err = SYS_ERR_OK;
	return;
}
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
	_mcspi_set_baud(speed);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = true;
#endif
	HAL_GPIO_WritePin(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.cs >> 5],
			(unsigned short) (1 << cfg.cs % 32), GPIO_PIN_RESET);
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
	HAL_GPIO_WritePin(
			(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.cs >> 5],
			(unsigned short) (1 << cfg.cs % 32), GPIO_PIN_SET);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = false;
#endif
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
int GI::Dev::Spi::writeRead(unsigned char *buffWrite,
		unsigned char *buffRead, unsigned int len)
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	if (!spi_semaphore[unitNr])
		return false;
#endif
	bool status = SYS_ERR_OK;
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *) userData;
	if (HAL_SPI_TransmitReceive(hspi, buffWrite, buffRead, len, 10) != HAL_OK)
		status = SYS_ERR_UNKNOWN;
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
	if (!spi_semaphore[unitNr])
		return false;
#endif
	bool status = SYS_ERR_OK;
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *) userData;
	if (HAL_SPI_Receive(hspi, buff, len, 10) != HAL_OK)
		status = SYS_ERR_UNKNOWN;
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = false;
#endif
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
	if (!spi_semaphore[unitNr])
		return false;
#endif
	bool status = SYS_ERR_OK;
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *) userData;
	if (HAL_SPI_Transmit(hspi, buff, len, 10) != HAL_OK)
		status = SYS_ERR_UNKNOWN;

	if (!DisableCsHandle)
		HAL_GPIO_WritePin(
				(GPIO_TypeDef *) GET_GPIO_PORT_BASE_ADDR[cfg.cs >> 5],
				(unsigned short) (1 << (cfg.cs % 32)), GPIO_PIN_SET);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = false;
#endif
	return status;
}
/*#####################################################*/
int GI::Dev::Spi::_mcspi_set_baud(unsigned long baud)
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
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *) userData;
	if ((hspi->Instance->CR1 & SPI_BAUDRATEPRESCALER_256)
			!= (SPI_BAUDRATEPRESCALER_256 & (baud << 3)))
	{
		hspi->Instance->CR1 &= ~SPI_BAUDRATEPRESCALER_256;
		hspi->Instance->CR1 |= SPI_BAUDRATEPRESCALER_256 & (baud << 3);
	}
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
int GI::Dev::Spi::writeReadByte(unsigned char *byte)
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

