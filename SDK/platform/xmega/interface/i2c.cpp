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
#include <driver/i2c.h>

extern CfgI2c i2cCfg[];

TWI_t* I2C_BASE_PTRS[] =
{
#ifdef TWIC
		&TWIC,
#else
		NULL,
#endif
#ifdef TWID
		&TWID,
#else
		NULL,
#endif
#ifdef TWIE
		&TWIE,
#else
		NULL,
#endif
#ifdef TWIF
		&TWIF,
#else
		NULL,
#endif
		};

//#####################################################
/**
 * @brief  Initializes peripherals used by the I2C EEPROM driver.
 * @param  None
 * @retval None
 */
GI::Dev::I2c::I2c(const char *path)
{
	unsigned int item_nr = 0;
	while(1)
	{
		if(i2cCfg[item_nr].name == NULL)
		{
			err = SYS_ERR_INVALID_PATH;
			return;
		}
		if(!strcmp(i2cCfg[item_nr].name, path))
			break;
		item_nr++;
	}

	if(strncmp(path, (char *)"i2c-", sizeof("i2c-") - 1))
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	unsigned int dev_nr = 0;
	if(sscanf(path + (sizeof("i2c-") - 1), "%u", &dev_nr) != 1)
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	if(dev_nr >= TWI_INTERFACE_COUNT)
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}

	memset(this, 0, sizeof(*this));
	memcpy(&cfg, &i2cCfg[item_nr], sizeof(CfgI2c));

	udata = (void *) I2C_BASE_PTRS[dev_nr];
	unitNr = dev_nr;
	//I2C_ConfigType  sI2C_Config = {0};

    /* Initialize I2C module with interrupt mode */
    //sI2C_Config.u16Slt = 0;
    //sI2C_Config.u16F = 0xBC;  /* Baud rate at 100 kbit/sec, MULT = 4 , ICR=60*/
    //sI2C_Config.sSetting.bMSTEn=1;
    //sI2C_Config.sSetting.bIntEn = 0;
    //sI2C_Config.sSetting.bI2CEn = 1;

    //I2C_Init(pI2C[dev_nr], &sI2C_Config);
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
	TWI_t *pI2Cx = (TWI_t *)udata;
	uint8_t u8ErrorStatus = 0;
	if (!noSendWriteOnRead)
	{
	}
	if (!ReceiveBytes)
	{
		 return (SysErr)u8ErrorStatus;
	}
	else
	{
		return (SysErr)u8ErrorStatus;
	}
	return SYS_ERR_OK;
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

