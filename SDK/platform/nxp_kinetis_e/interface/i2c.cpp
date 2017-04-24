/*
 * i2c.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: John Smith
 */

#include <string.h>
#include <stdio.h>
#include <api/i2c.h>
#include <driver/i2c.h>
#include "i2c.h"

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

	this->cfg = cfg;
	//CfgI2c *int_cfg = (CfgI2c *)cfg->cfg;

	I2C_Type *pI2C[]= I2C_BASE_PTRS;

	udata = (void *) pI2C[dev_nr];
	unitNr = dev_nr;
	I2C_ConfigType  sI2C_Config = {0};

    /* Initialize I2C module with interrupt mode */
    sI2C_Config.u16Slt = 0;
    sI2C_Config.u16F = 0xBC;  /* Baud rate at 100 kbit/sec, MULT = 4 , ICR=60*/
    sI2C_Config.sSetting.bMSTEn=1;
    sI2C_Config.sSetting.bIntEn = 0;
    sI2C_Config.sSetting.bI2CEn = 1;

    I2C_Init(pI2C[dev_nr], &sI2C_Config);
}
/*#####################################################*/
GI::Dev::I2c::~I2c()
{
	I2C_Deinit((I2C_Type *)udata);
}

SysErr GI::Dev::I2c::WR(unsigned char addr, unsigned char *buff_send,
		unsigned int TransmitBytes, unsigned char *buff_receive,
		unsigned int ReceiveBytes)
{
	I2C_Type *pI2Cx = (I2C_Type *)udata;
	uint8_t u8ErrorStatus;
	if (!noSendWriteOnRead)
	{

		 uint32_t i;


		/* send start signals to bus */
		u8ErrorStatus = I2C_Start(pI2Cx);

		/* send device address to slave */
		u8ErrorStatus = I2C_WriteOneByte(pI2Cx, addr| I2C_WRITE);

		/* if no error occur, received the correct ack from slave
				continue to send data to slave
			*/
		if( u8ErrorStatus == I2C_ERROR_NULL )
		{
			for(i=0;i<TransmitBytes;i++)
			{
				u8ErrorStatus = I2C_WriteOneByte(pI2Cx,buff_send[i]);
				if( u8ErrorStatus != I2C_ERROR_NULL )
				{
					return (SysErr)u8ErrorStatus;
				}
			}
		 }
	}
	if (!ReceiveBytes)
	{
		 /* send stop signals to bus */
		 u8ErrorStatus = I2C_Stop(pI2Cx);

		 return (SysErr)u8ErrorStatus;
	}
	else
	{
		uint32_t i;
		uint8_t u8ErrorStatus;

		/* send start signals to bus */
		u8ErrorStatus = I2C_Start(pI2Cx);

		/* send device address to slave */
		u8ErrorStatus = I2C_WriteOneByte(pI2Cx,addr | I2C_READ);

		/* if no error occur, received the correct ack from slave
		            continue to send data to slave
		        */
		/* dummy read one byte to switch to Rx mode */
		I2C_ReadOneByte(pI2Cx,&buff_receive[0],I2C_SEND_ACK);

		if( u8ErrorStatus == I2C_ERROR_NULL )
		{
			for(i=0;i<ReceiveBytes-1;i++)
			{
				u8ErrorStatus = I2C_ReadOneByte(pI2Cx,&buff_receive[i],I2C_SEND_ACK);
				if( u8ErrorStatus != I2C_ERROR_NULL )
				{
					return (SysErr)u8ErrorStatus;
				}
			}
			u8ErrorStatus = I2C_ReadOneByte(pI2Cx,&buff_receive[i],I2C_SEND_NACK);
		}
		/* send stop signals to bus */
		u8ErrorStatus = I2C_Stop(pI2Cx);

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

