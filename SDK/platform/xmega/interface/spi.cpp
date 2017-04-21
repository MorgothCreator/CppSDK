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
#include <interface/gpio.h>
#include <driver/spi.h>
#include <sys/core_init.h>

extern CfgSpi spiCfg[];

#if (USE_DRIVER_SEMAPHORE == true)
volatile bool spi_semaphore[SPI_INTERFACE_COUNT];
#endif

SPI_t* SPI_BASE_PTRS[] =
{
#ifdef SPIC
		&SPIC,
#else
		NULL,
#endif
#ifdef SPID
		&SPID,
#else
		NULL,
#endif
#ifdef SPIE
		&SPIE,
#else
		NULL,
#endif
#ifdef SPIF
		&SPIF,
#else
		NULL,
#endif
		};

GI::Dev::Spi::Spi(const char *path)
{
	memset(this, 0, sizeof(*this));
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
	memcpy(&cfg, &spiCfg[item_nr], sizeof(CfgSpi));
	unitNr = _portNr;
	channel = _channel;
	userData = (void *) SPI_BASE_PTRS[_portNr];


	/*SPI_Type *Addr[] = SPI_BASE_PTRS;
	SPI_Type *pSPI = Addr[_portNr];
	SPI_ConfigType pConfig;
	pConfig.u32BusClkHz = FCPU / 2;
	pConfig.u32BitRate = cfg.speed;
	pConfig.sSettings.bClkPhase1 = (cfg.spiMode >> 1) & 0x01;
	pConfig.sSettings.bClkPolarityLow = cfg.spiMode & 0x01;
	pConfig.sSettings.bModuleEn             = 1;
	pConfig.sSettings.bMasterMode           = 1;
	pConfig.sSettings.bMasterAutoDriveSS    = 0;
    SPI_Init(SPI0, &pConfig);

	GPIO_Type *GpioAddr[] = GPIO_BASE_PTRS;
	GPIO_Type *BaseAddr = GpioAddr[cfg.cs >> 5];
	CONFIG_PIN_AS_GPIO(BaseAddr, cfg.cs % 32, OUTPUT);
	DISABLE_INPUT(BaseAddr, cfg.cs % 32);
	OUTPUT_SET(BaseAddr, cfg.cs % 32);*/
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
	//SPI_Type *SpiHandle = (SPI_Type *) userData;
	//SPI_DeInit(SpiHandle);
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
	setSpeed(speed);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = true;
#endif
	PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.cs >> 5];
	BaseAddr->OUTCLR = BIT_MASK_TABLE[cfg.cs % 8];
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
	PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.cs >> 5];
	BaseAddr->OUTSET = BIT_MASK_TABLE[cfg.cs % 8];
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = false;
#endif
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
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
#endif
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = true;
#endif
	if (!DisableCsHandle)
	{
		PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.cs >> 5];
		BaseAddr->OUTCLR = BIT_MASK_TABLE[cfg.cs % 8];
	}
	SysErr status = SYS_ERR_OK;
	SPI_t *port = (SPI_t *) userData;
	memset(buffRead, 0, lenRead);

	unsigned int cnt = 0;
	unsigned char *tmp_buff_ptr = buffWrite;
	//unsigned char tmp_read;
	for(; cnt < lenWrite; cnt++)
	{
		/* Start transmission */
		port->DATA = *tmp_buff_ptr++;
		/* Wait for transmission complete */
		while(!(port->STATUS & SPI_IF_bm)) { }
		//tmp_read = port->DATA;
	}
	tmp_buff_ptr = buffRead;
	for(cnt = 0; cnt < lenRead; cnt++)
	{
		/* Start transmission */
		port->DATA = 0xFF;
		/* Wait for transmission complete */
		while(!(port->STATUS & SPI_IF_bm)) { }
		*tmp_buff_ptr++ = port->DATA;
	}
	if (!DisableCsHandle)
	{
		PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.cs >> 5];
		BaseAddr->OUTSET = BIT_MASK_TABLE[cfg.cs % 8];
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
	if (spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
#endif
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = true;
#endif
	if (!DisableCsHandle)
	{
		PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.cs >> 5];
		BaseAddr->OUTCLR = BIT_MASK_TABLE[cfg.cs % 8];
	}
	SysErr status = SYS_ERR_OK;
	SPI_t *port = (SPI_t *) userData;
	unsigned int cnt = 0;
	unsigned char *tmp_buff_ptr = buff;
	for(cnt = 0; cnt < len; cnt++)
	{
		/* Start transmission */
		port->DATA = 0xFF;
		/* Wait for transmission complete */
		while(!(port->STATUS & SPI_IF_bm)) { }
		*tmp_buff_ptr++ = port->DATA;
	}

	if (!DisableCsHandle)
	{
		PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.cs >> 5];
		BaseAddr->OUTSET = BIT_MASK_TABLE[cfg.cs % 8];
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
	if (spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
#endif
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = true;
#endif
	if (!DisableCsHandle)
	{
		PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.cs >> 5];
		BaseAddr->OUTCLR = BIT_MASK_TABLE[cfg.cs % 8];
	}
	SysErr status = SYS_ERR_OK;
	SPI_t *port = (SPI_t *) userData;
	unsigned int cnt = 0;
	unsigned char *tmp_buff_ptr = buff;
	//unsigned char tmp_read;
	for(; cnt < len; cnt++)
	{
		/* Start transmission */
		port->DATA = *tmp_buff_ptr++;
		/* Wait for transmission complete */
		while(!(port->STATUS & SPI_IF_bm)) { }
		//tmp_read = port->DATA;
	}

	if (!DisableCsHandle)
	{
		PORT_t *BaseAddr = GPIO_BASE_PTRS[cfg.cs >> 5];
		BaseAddr->OUTSET = BIT_MASK_TABLE[cfg.cs % 8];
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
	SPI_t *port = (SPI_t *) userData;
	/* Start transmission */
	port->DATA = *byte;
	/* Wait for transmission complete */
	while(!(port->STATUS & SPI_IF_bm)) { }
	*byte = port->DATA;
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
	SPI_t *port = (SPI_t *) userData;
	//SPI_SetBaudRate(hspi, FCPU/2, cfg.speed);

	u32 new_speed = 0;
	if(cfg.speed > FCPU/2)
		new_speed = SPI_CLK2X_bm | SPI_PRESCALER_DIV4_gc;
	else if(cfg.speed > FCPU/4)
		new_speed = SPI_PRESCALER_DIV4_gc;
	else if(cfg.speed > FCPU/8)
		new_speed = SPI_CLK2X_bm | SPI_PRESCALER_DIV16_gc;
	else if(cfg.speed > FCPU/16)
		new_speed = SPI_PRESCALER_DIV16_gc;
	else if(cfg.speed > FCPU/32)
		new_speed = SPI_CLK2X_bm | SPI_PRESCALER_DIV64_gc;
	else if(cfg.speed > FCPU/64)
		new_speed = SPI_PRESCALER_DIV64_gc;
	else if(cfg.speed > FCPU/128)
		new_speed = SPI_PRESCALER_DIV128_gc;
	else
		new_speed = SPI_PRESCALER_DIV128_gc;

	port->CTRL = (port->CTRL & ~(SPI_CLK2X_bm | SPI_PRESCALER_gm)) | new_speed;
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/

