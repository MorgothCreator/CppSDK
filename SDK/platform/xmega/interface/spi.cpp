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
	userData = (void *) SPI_BASE_PTRS[_portNr];
	this->cfg = cfg;
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;

	volatile PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->cs >> 5];
	BaseAddr->OUTSET = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->cs % 8]);
	BaseAddr->DIRSET = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->cs % 8]);

	unsigned char tmp = SPI_MODE_gm & (int_cfg->spiMode << SPI_MODE_gp);
	
	u32 new_speed = 0;
	if(int_cfg->speed > FCPU/2)
		new_speed = SPI_CLK2X_bm | SPI_PRESCALER_DIV4_gc;
	else if(int_cfg->speed > FCPU/4)
		new_speed = SPI_PRESCALER_DIV4_gc;
	else if(int_cfg->speed > FCPU/8)
		new_speed = SPI_CLK2X_bm | SPI_PRESCALER_DIV16_gc;
	else if(int_cfg->speed > FCPU/16)
		new_speed = SPI_PRESCALER_DIV16_gc;
	else if(int_cfg->speed > FCPU/32)
		new_speed = SPI_CLK2X_bm | SPI_PRESCALER_DIV64_gc;
	else if(int_cfg->speed > FCPU/64)
		new_speed = SPI_PRESCALER_DIV64_gc;
	else if(int_cfg->speed > FCPU/128)
		new_speed = SPI_PRESCALER_DIV128_gc;
	else
		new_speed = SPI_PRESCALER_DIV128_gc;
	tmp |= new_speed;
	if(int_cfg->lsbFirst)
		tmp |= SPI_DORD_bm;
	tmp |= SPI_ENABLE_bm | SPI_MASTER_bm;
		
	switch(unitNr)
	{
#ifdef SPIC
	case 0:
		PORTC.DIRSET = (1 << 4) | (1 << 5) | (1 << 7);
		PORTC.OUTSET = (1 << 4) | (1 << 5) | (1 << 7);
		PORTC.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTC.DIRCLR = 1 << 6;
		SPIC.CTRL = tmp;
		break;
#endif
#ifdef SPID
	case 1:
		PORTD.DIRSET = (1 << 4) | (1 << 5) | (1 << 7);
		PORTD.OUTSET = (1 << 4) | (1 << 5) | (1 << 7);
		PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTD.DIRCLR = 1 << 6;
		SPID.CTRL = tmp;
		break;
#endif
#ifdef SPIE
	case 2:
		PORTE.DIRSET = (1 << 4) | (1 << 5) | (1 << 7);
		PORTE.OUTSET = (1 << 4) | (1 << 5) | (1 << 7);
		PORTE.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTE.DIRCLR = 1 << 6;
		SPIE.CTRL = tmp;
		break;
#endif
#ifdef SPIF
	case 3:
		PORTF.DIRSET = (1 << 4) | (1 << 5) | (1 << 7);
		PORTF.OUTSET = (1 << 4) | (1 << 5) | (1 << 7);
		PORTF.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTF.DIRCLR = 1 << 6;
		SPIF.CTRL = tmp;
		break;
#endif
	}
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
	while (spi_semaphore[unitNr]);
#endif
	setSpeed(speed);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = true;
#endif
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->cs >> 5];
	BaseAddr->OUTCLR = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->cs % 8]);
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
	PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->cs >> 5];
	BaseAddr->OUTSET = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->cs % 8]);
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
	spi_semaphore[unitNr] = true;
#endif
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!disableCsHandle)
	{
		PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->cs >> 5];
		BaseAddr->OUTCLR = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->cs % 8]);
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
	if (!disableCsHandle)
	{
		PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->cs >> 5];
		BaseAddr->OUTSET = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->cs % 8]);
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
	spi_semaphore[unitNr] = true;
#endif
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!disableCsHandle)
	{
		PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->cs >> 5];
		BaseAddr->OUTCLR = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->cs % 8]);
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

	if (!disableCsHandle)
	{
		PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->cs >> 5];
		BaseAddr->OUTSET = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->cs % 8]);
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
	spi_semaphore[unitNr] = true;
#endif
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!disableCsHandle)
	{
		volatile PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->cs >> 5];
		BaseAddr->OUTCLR = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->cs % 8]);
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

	if (!disableCsHandle)
	{
		volatile PORT_t *BaseAddr = GPIO_BASE_PTRS[int_cfg->cs >> 5];
		BaseAddr->OUTSET = pgm_read_byte(&BIT_MASK_TABLE[int_cfg->cs % 8]);
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
	//SPI_SetBaudRate(hspi, FCPU/2, int_cfg->speed);

	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	u32 new_speed = 0;
	if(int_cfg->speed > FCPU/2)
		new_speed = SPI_CLK2X_bm | SPI_PRESCALER_DIV4_gc;
	else if(int_cfg->speed > FCPU/4)
		new_speed = SPI_PRESCALER_DIV4_gc;
	else if(int_cfg->speed > FCPU/8)
		new_speed = SPI_CLK2X_bm | SPI_PRESCALER_DIV16_gc;
	else if(int_cfg->speed > FCPU/16)
		new_speed = SPI_PRESCALER_DIV16_gc;
	else if(int_cfg->speed > FCPU/32)
		new_speed = SPI_CLK2X_bm | SPI_PRESCALER_DIV64_gc;
	else if(int_cfg->speed > FCPU/64)
		new_speed = SPI_PRESCALER_DIV64_gc;
	else if(int_cfg->speed > FCPU/128)
		new_speed = SPI_PRESCALER_DIV128_gc;
	else
		new_speed = SPI_PRESCALER_DIV128_gc;

	port->CTRL = (port->CTRL & ~(SPI_CLK2X_bm | SPI_PRESCALER_gm)) | new_speed;
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/

