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
#include <driver/gpio.h>
#include <sys/core_init.h>

#define CONF_SPI_TIMEOUT           10000

extern CfgSpi spiCfg[];

#if (USE_DRIVER_SEMAPHORE == true)
volatile bool spi_semaphore[SPI_INTERFACE_COUNT];
#endif

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

	struct spi_config config_spi_master;

	//  [conf_defaults]
	spi_get_config_defaults(&config_spi_master);
	//! [conf_defaults]
	//  [mux_setting]
	config_spi_master.mux_setting = SPI_SIGNAL_MUX_SETTING_E;
	//! [mux_setting]
	/* Configure pad 1 as unused */
	//  [ss]
	config_spi_master.pinmux_pad1 = PINMUX_UNUSED;
	//! [ss]
	/* Configure pad 0 for data in */

	Sercom *hw;
	switch (_portNr)
	{
	case 0:
		hw = SERCOM0;
		//  [di]
		config_spi_master.pinmux_pad0 = SERCOM0_PAD0_DEFAULT;
		//! [di]
		/* Configure pad 2 for data out */
		//  [do]
		config_spi_master.pinmux_pad2 = SERCOM0_PAD2_DEFAULT;
		//! [do]
		/* Configure pad 3 for SCK */
		//  [sck]
		config_spi_master.pinmux_pad3 = SERCOM0_PAD3_DEFAULT;
		//! [sck]
		break;
	case 1:
		hw = SERCOM1;
		//  [di]
		config_spi_master.pinmux_pad0 = SERCOM1_PAD0_DEFAULT;
		//! [di]
		/* Configure pad 2 for data out */
		//  [do]
		config_spi_master.pinmux_pad2 = SERCOM1_PAD2_DEFAULT;
		//! [do]
		/* Configure pad 3 for SCK */
		//  [sck]
		config_spi_master.pinmux_pad3 = SERCOM1_PAD3_DEFAULT;
		//! [sck]
		break;
	case 2:
		hw = SERCOM2;
		//  [di]
		config_spi_master.pinmux_pad0 = SERCOM2_PAD0_DEFAULT;
		//! [di]
		/* Configure pad 2 for data out */
		//  [do]
		config_spi_master.pinmux_pad2 = SERCOM2_PAD2_DEFAULT;
		//! [do]
		/* Configure pad 3 for SCK */
		//  [sck]
		config_spi_master.pinmux_pad3 = SERCOM2_PAD3_DEFAULT;
		//! [sck]
		break;
	case 3:
		hw = SERCOM3;
		//  [di]
		config_spi_master.pinmux_pad0 = SERCOM3_PAD0_DEFAULT;
		//! [di]
		/* Configure pad 2 for data out */
		//  [do]
		config_spi_master.pinmux_pad2 = SERCOM3_PAD2_DEFAULT;
		//! [do]
		/* Configure pad 3 for SCK */
		//  [sck]
		config_spi_master.pinmux_pad3 = SERCOM3_PAD3_DEFAULT;
		//! [sck]
		break;
	case 4:
		hw = SERCOM4;
		//  [di]
		config_spi_master.pinmux_pad0 = SERCOM4_PAD0_DEFAULT;
		//! [di]
		/* Configure pad 2 for data out */
		//  [do]
		config_spi_master.pinmux_pad2 = SERCOM4_PAD2_DEFAULT;
		//! [do]
		/* Configure pad 3 for SCK */
		//  [sck]
		config_spi_master.pinmux_pad3 = SERCOM4_PAD3_DEFAULT;
		//! [sck]
		break;
	case 5:
		hw = SERCOM5;
		//  [di]
		config_spi_master.pinmux_pad0 = SERCOM5_PAD0_DEFAULT;
		//! [di]
		/* Configure pad 2 for data out */
		//  [do]
		config_spi_master.pinmux_pad2 = SERCOM5_PAD2_DEFAULT;
		//! [do]
		/* Configure pad 3 for SCK */
		//  [sck]
		config_spi_master.pinmux_pad3 = SERCOM5_PAD3_DEFAULT;
		//! [sck]
		break;
	default:
		err = SYS_ERR_DEVICE_NOT_FOUND;
		return;
	}

	struct spi_module *spi_master_instance = (struct spi_module *)calloc(1, sizeof(struct spi_module));
	if(!spi_master_instance)
	{
		err = SYS_ERR_OUT_OF_MEMORY;
		return;
	}

	unitNr = _portNr;
	channel = _channel;
	this->cfg = cfg;
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;

	struct port_config config_port_pin;
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT_WTH_READBACK;
	config_port_pin.input_pull = PORT_PIN_PULL_NONE;
	port_pin_set_config(int_cfg->cs, &config_port_pin);
	port_pin_set_output_level(int_cfg->cs, 1);
	
	userData = (struct spi_module *)spi_master_instance;
	//  [init]
	spi_init(spi_master_instance, hw, &config_spi_master);
	//! [init]

	//  [enable]
	spi_enable(spi_master_instance);
	//! [enable]
	/*SPI_Type *Addr[] = SPI_BASE_PTRS;
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
	OUTPUT_SET(BaseAddr, int_cfg->cs % 32);*/
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
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	port_pin_set_output_level(int_cfg->cs, 0);
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
	port_pin_set_output_level(int_cfg->cs, 1);
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
	if (!spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
#endif
	CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!disableCsHandle)
	{
		port_pin_set_output_level(int_cfg->cs, 0);
	}//((struct spi_module *)userData)
	SysErr status = SYS_ERR_OK;
	if (spi_write_buffer_wait(((struct spi_module *)userData), buffWrite, lenWrite) != STATUS_OK)
		status = SYS_ERR_UNKNOWN;
	if (status == SYS_ERR_OK && spi_read_buffer_wait(((struct spi_module *)userData), buffRead, lenRead, 0xFF) != STATUS_OK)
		status = SYS_ERR_UNKNOWN;

	if (!disableCsHandle)
	{
		port_pin_set_output_level(int_cfg->cs, 1);
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
		port_pin_set_output_level(int_cfg->cs, 0);
	}
	SysErr status = SYS_ERR_OK;
	if (spi_read_buffer_wait(((struct spi_module *)userData), buff, len, 0xFF) != STATUS_OK)
		status = SYS_ERR_UNKNOWN;

	if (!disableCsHandle)
	{
		port_pin_set_output_level(int_cfg->cs, 1);
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
		port_pin_set_output_level(int_cfg->cs, 0);
	}
	
	SysErr status = SYS_ERR_OK;
	if (spi_write_buffer_wait(((struct spi_module *)userData), buff, len) != STATUS_OK)
		status = SYS_ERR_UNKNOWN;

	if (!disableCsHandle)
	{
		port_pin_set_output_level(int_cfg->cs, 1);
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
	unsigned short tmp = *byte;
	if (spi_transceive_wait(((struct spi_module *)userData), tmp, &tmp) != STATUS_OK)
		return SYS_ERR_UNKNOWN;
	*byte = tmp;
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
/*	SPI_t *port = (SPI_t *) userData;
	//SPI_SetBaudRate(hspi, FCPU/2, int_cfg->speed);

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

	port->CTRL = (port->CTRL & ~(SPI_CLK2X_bm | SPI_PRESCALER_gm)) | new_speed;*/
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/

