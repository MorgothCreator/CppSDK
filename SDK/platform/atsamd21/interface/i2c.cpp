/*
 * i2c.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: John Smith
 */

#include <string.h>
#include <stdio.h>
#include <api/i2c.h>
#include "i2c.h"
#include <driver/i2c_master.h>

extern CfgI2c i2cCfg[];

#if (USE_DRIVER_SEMAPHORE == true)
volatile bool twi_semaphore[TWI_INTERFACE_COUNT];
#endif

//#####################################################
/**
 * @brief  Initializes peripherals used by the I2C EEPROM driver.
 * @param  None
 * @retval None
 */
GI::Dev::I2c::I2c(ioSettings *cfg)
{
	memset(this, 0, sizeof(*this));
	if(cfg->info.ioType != ioSettings::info_s::ioType_I2C)
		return;
	if(strncmp(cfg->info.name, (char *)"i2c-", sizeof("i2c-") - 1))
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	unsigned int dev_nr = 0;
	if(sscanf(cfg->info.name + (sizeof("i2c-") - 1), "%u", &dev_nr) != 1)
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	if(dev_nr >= TWI_INTERFACE_COUNT)
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}

	Sercom *hw;
	switch (dev_nr)
	{
	case 0:
		hw = SERCOM0;
		break;
	case 1:
		hw = SERCOM1;
		break;
	case 2:
		hw = SERCOM2;
		break;
	case 3:
		hw = SERCOM3;
		break;
	case 4:
		hw = SERCOM4;
		break;
	case 5:
		hw = SERCOM5;
		break;
	default:
		err = SYS_ERR_DEVICE_NOT_FOUND;
		return;
	}

	struct i2c_master_module *i2c_master_instance = (struct i2c_master_module *)calloc(1, sizeof(struct i2c_master_module));
	if(!i2c_master_instance)
	{
		err = SYS_ERR_OUT_OF_MEMORY;
		return;
	}
	udata = (void *) i2c_master_instance;
	unitNr = dev_nr;
	this->cfg = cfg;
	CfgI2c *int_cfg = (CfgI2c *)cfg->cfg;

	/* Initialize config structure and software module. */
	//! [init_conf]
	struct i2c_master_config config_i2c_master;
	config_i2c_master.baud_rate = int_cfg->speed;
	i2c_master_get_config_defaults(&config_i2c_master);
	//! [init_conf]

	/* Change buffer timeout to something longer. */
	//! [conf_change]
	config_i2c_master.buffer_timeout = 10000;
	//! [conf_change]
	/* Initialize and enable device with config. */
	//! [init_module]
	i2c_master_init(i2c_master_instance, hw, &config_i2c_master);
	//! [init_module]

	//! [enable_module]
	i2c_master_enable(i2c_master_instance);
	//! [enable_module]
}
/*#####################################################*/
GI::Dev::I2c::~I2c()
{
	//I2C_Deinit((I2C_Type *)udata);
}

SysErr GI::Dev::I2c::WR(unsigned char addr, unsigned char *buff_send,
		unsigned int TransmitBytes, unsigned char *buff_receive,
		unsigned int ReceiveBytes)
{
	enum status_code stat = STATUS_OK;
	if (!noSendWriteOnRead)
	{
		struct i2c_master_packet packet = {
			.address		= addr,
			.data_length	= (unsigned short)TransmitBytes,
			.data			= buff_send,
			.ten_bit_address = false,
			.high_speed      = false,
			.hs_master_code  = 0x0,
		};
		if(!ReceiveBytes)
			stat = i2c_master_write_packet_wait((struct i2c_master_module *)udata, &packet);
		else
			stat = i2c_master_write_packet_wait_no_stop((struct i2c_master_module *)udata, &packet);
	}
	if (ReceiveBytes && stat == STATUS_OK)
	{
		struct i2c_master_packet packet = {
			.address		= addr,
			.data_length	= (unsigned short)ReceiveBytes,
			.data			= buff_receive,
			.ten_bit_address = false,
			.high_speed      = false,
			.hs_master_code  = 0x0,
		};
		stat = i2c_master_read_packet_wait(((struct i2c_master_module *)udata), &packet);
	}
	switch (stat)
	{
	case STATUS_OK:
		return SYS_ERR_OK;
	case STATUS_BUSY:
		return SYS_ERR_BUSY;
	default:
		return SYS_ERR_UNKNOWN;
	}
}
/*#####################################################*/
SysErr GI::Dev::I2c::writeRead(unsigned char addr, unsigned char *buffSend,
		unsigned int lenSend, unsigned char *buffReceive,
		unsigned int lenReceive)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
#if (USE_DRIVER_SEMAPHORE == true)
	while (twi_semaphore[unitNr]);
	twi_semaphore[unitNr] = true;
#endif
	SysErr result = WR(addr << 1, buffSend, lenSend, buffReceive, lenReceive);

#if (USE_DRIVER_SEMAPHORE == true)
	twi_semaphore[unitNr] = false;
#endif
	if (result == SYS_ERR_OK)
		return SYS_ERR_OK;
	else
		return SYS_ERR_NAK;
}
/*#####################################################*/
int GI::Dev::I2c::readBytes(unsigned char addr, unsigned char *buff, unsigned int len)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
#if (USE_DRIVER_SEMAPHORE == true)
	while (twi_semaphore[unitNr])
		;
	twi_semaphore[unitNr] = true;
#endif
	SysErr result = WR(addr << 1, NULL, 0, buff, len);
#if (USE_DRIVER_SEMAPHORE == true)
	twi_semaphore[unitNr] = false;
#endif
	if (result == SYS_ERR_OK)
		return len;
	else
		return SYS_ERR_NAK;
}

/*#####################################################*/
int GI::Dev::I2c::writeBytes(unsigned char addr, unsigned char *buff, unsigned int len)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
#if (USE_DRIVER_SEMAPHORE == true)
	while (twi_semaphore[unitNr])
		;
	twi_semaphore[unitNr] = true;
#endif
	SysErr result = WR(addr << 1, buff, len, NULL, 0);
#if (USE_DRIVER_SEMAPHORE == true)
	twi_semaphore[unitNr] = false;
#endif
	if (result == SYS_ERR_OK)
		return len;
	else
		return SYS_ERR_NAK;
}

/*#####################################################*/

