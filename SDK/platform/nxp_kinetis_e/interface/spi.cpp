/*
 * spi.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: John Smith
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <api/spi.h>
#include <driver/spi.h>
#include <driver/gpio.h>
#include <sys/core_init.h>
#include "spi.h"

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

	SPI_Type *Addr[] = SPI_BASE_PTRS;
	SPI_Type *pSPI = Addr[_portNr];
	SPI_ConfigType pConfig;
	pConfig.u32BusClkHz = FCPU / 2;
	pConfig.u32BitRate = int_cfg->speed;
	pConfig.sSettings.bClkPhase1 = (int_cfg->spiMode >> 1) & 0x01;
	pConfig.sSettings.bClkPolarityLow = int_cfg->spiMode & 0x01;
	pConfig.sSettings.bModuleEn             = 1;
	pConfig.sSettings.bMasterMode           = 1;
	pConfig.sSettings.bMasterAutoDriveSS    = 0;
    SPI_Init(SPI0, &pConfig);

	GPIO_Type *GpioAddr[] = GPIO_BASE_PTRS;
	GPIO_Type *BaseAddr = GpioAddr[int_cfg->cs >> 5];
	CONFIG_PIN_AS_GPIO(BaseAddr, int_cfg->cs % 32, OUTPUT);
	DISABLE_INPUT(BaseAddr, int_cfg->cs % 32);
	OUTPUT_SET(BaseAddr, int_cfg->cs % 32);
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
	SPI_Type *SpiHandle = (SPI_Type *) userData;
	SPI_DeInit(SpiHandle);
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
	while (spi_semaphore[property.unitNr])
		;
#endif
	setSpeed(speed);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[property.unitNr] = true;
#endif
	GPIO_Type *GpioAddr[] = GPIO_BASE_PTRS;
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	GPIO_Type *BaseAddr = GpioAddr[int_cfg->cs >> 5];
	OUTPUT_CLEAR(BaseAddr, int_cfg->cs % 32);
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
	GPIO_Type *GpioAddr[] = GPIO_BASE_PTRS;
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	GPIO_Type *BaseAddr = GpioAddr[int_cfg->cs >> 5];
	OUTPUT_SET(BaseAddr, int_cfg->cs % 32);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[property.unitNr] = false;
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
	if (!spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
#endif
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!disableCsHandle)
	{
		GPIO_Type *GpioAddr[] = GPIO_BASE_PTRS;
		GPIO_Type *BaseAddr = GpioAddr[int_cfg->cs >> 5];
		OUTPUT_CLEAR(BaseAddr, int_cfg->cs % 32);
	}
	SysErr status = SYS_ERR_OK;
	SPI_Type *hspi = (SPI_Type *) userData;
	memset(buffRead, 0, lenRead);
	if (SPI_TransferWait(hspi, buffWrite, buffWrite, lenWrite) != SPI_ERR_SUCCESS)
		status = SYS_ERR_UNKNOWN;
	if (SPI_TransferWait(hspi, buffRead, buffRead, lenRead) != SPI_ERR_SUCCESS)
		status = SYS_ERR_UNKNOWN;
	if (!disableCsHandle)
	{
		GPIO_Type *GpioAddr[] = GPIO_BASE_PTRS;
		GPIO_Type *BaseAddr = GpioAddr[int_cfg->cs >> 5];
		OUTPUT_SET(BaseAddr, int_cfg->cs % 32);
	}
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
		return SYS_ERR_BUSY;
#endif
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!disableCsHandle)
	{
		GPIO_Type *GpioAddr[] = GPIO_BASE_PTRS;
		GPIO_Type *BaseAddr = GpioAddr[int_cfg->cs >> 5];
		OUTPUT_CLEAR(BaseAddr, int_cfg->cs % 32);
	}
	SysErr status = SYS_ERR_OK;
	SPI_Type *hspi = (SPI_Type *) userData;
	if (SPI_TransferWait(hspi, buff, buff, len) != SPI_ERR_SUCCESS)
		status = SYS_ERR_UNKNOWN;
	if (!disableCsHandle)
	{
		GPIO_Type *GpioAddr[] = GPIO_BASE_PTRS;
		GPIO_Type *BaseAddr = GpioAddr[int_cfg->cs >> 5];
		OUTPUT_SET(BaseAddr, int_cfg->cs % 32);
	}
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
	if (!spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
#endif
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!disableCsHandle)
	{
		GPIO_Type *GpioAddr[] = GPIO_BASE_PTRS;
		GPIO_Type *BaseAddr = GpioAddr[int_cfg->cs >> 5];
		OUTPUT_CLEAR(BaseAddr, int_cfg->cs % 32);
	}
	SysErr status = SYS_ERR_OK;
	SPI_Type *hspi = (SPI_Type *) userData;
	if (SPI_TransferWait(hspi, buff, buff, len) != SPI_ERR_SUCCESS)
		status = SYS_ERR_UNKNOWN;

	if (!disableCsHandle)
	{
		GPIO_Type *GpioAddr[] = GPIO_BASE_PTRS;
		GPIO_Type *BaseAddr = GpioAddr[int_cfg->cs >> 5];
		OUTPUT_SET(BaseAddr, int_cfg->cs % 32);
	}
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
	SPI_Type *hspi = (SPI_Type *) userData;
	SPI_TransferWait(hspi, byte, byte, 1);
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
	SPI_Type *hspi = (SPI_Type *) userData;
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	SPI_SetBaudRate(hspi, FCPU/2, int_cfg->speed);
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/

