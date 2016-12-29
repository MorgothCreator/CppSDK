/*
 *  api/io_handler.cpp
 *
 *  Copyright (C) 2016  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of CppSDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "io_handle.h"

#include <stdio.h>
#include <string.h>

#include <api/gpio.h>
#include <api/i2c.h>
#include <api/spi.h>
#include <api/uart.h>

#include <api/dev_request.h>

/**********************************************************************************/
GI::IO::IO(char *path)
{
	memset(this, 0, sizeof(*this));
	GI::Dev::Gpio *gpioDev;
	GI::Dev::I2c *i2cdev;
	GI::Dev::Spi *spiDev;
	GI::Dev::Uart *uartDev;
	if(GI::Dev::DevRequest::request(path, &gpioDev) == SYS_ERR_OK)
	{
		devHandler = (void *)gpioDev;
		ioDevType = IO_DEV_GPIO;
	}
	else if(GI::Dev::DevRequest::request(path, &i2cdev) == SYS_ERR_OK)
	{
		devHandler = (void *)i2cdev;
		ioDevType = IO_DEV_I2C;
	}
	else if(GI::Dev::DevRequest::request(path, &spiDev) == SYS_ERR_OK)
	{
		devHandler = (void *)spiDev;
		ioDevType = IO_DEV_SPI;
	}
	else if(GI::Dev::DevRequest::request(path, &uartDev) == SYS_ERR_OK)
	{
		devHandler = (void *)uartDev;
		ioDevType = IO_DEV_UART;
	}
	else
	{
		ioDevType = IO_DEV_NULL;
		devHandler = NULL;
	}
}

GI::IO::~IO()
{
	ioDevType = IO_DEV_NULL;
	devHandler = NULL;
}
/**********************************************************************************/
int GI::IO::write(unsigned char *buff, unsigned int len)
{
	if(!devHandler || !ioDevType)
		return SYS_ERR_NO_REGISTERED_DEVICE;
	switch((int)ioDevType)
	{
	case IO_DEV_I2C:
		return ((GI::Dev::I2c *)devHandler)->writeBytes(slaveAddr, buff, len);
	case IO_DEV_SPI:
		return ((GI::Dev::Spi *)devHandler)->writeBytes(buff, len);
	case IO_DEV_UART:
		return ((GI::Dev::Uart *)devHandler)->write((char *)buff, len);
	}
	return SYS_ERR_NOT_IMPLEMENTED;
}

SysErr GI::IO::write(bool state)
{
	if(!devHandler || !ioDevType)
		return SYS_ERR_NO_REGISTERED_DEVICE;
	switch((int)ioDevType)
	{
	case IO_DEV_GPIO:
		return ((GI::Dev::Gpio *)devHandler)->setOut(state);
	}
	return SYS_ERR_NOT_IMPLEMENTED;
}

SysErr GI::IO::write(u32 data)
{
	if(!devHandler || !ioDevType)
		return SYS_ERR_NO_REGISTERED_DEVICE;
	switch((int)ioDevType)
	{
	case IO_DEV_GPIO:
		return ((GI::Dev::Gpio *)devHandler)->setOut(data);
	}
	return SYS_ERR_NOT_IMPLEMENTED;
}
/**********************************************************************************/
int GI::IO::read(unsigned char *buff, unsigned int len)
{
	if(!devHandler || !ioDevType)
		return SYS_ERR_NO_REGISTERED_DEVICE;
	switch((int)ioDevType)
	{
	case IO_DEV_I2C:
		return ((GI::Dev::I2c *)devHandler)->readBytes(slaveAddr, buff, len);
	case IO_DEV_SPI:
		return ((GI::Dev::Spi *)devHandler)->readBytes(buff, len);
	//case IO_DEV_UART:
		//return ((GI::Dev::Uart *)devHandler)->print((char *)buff);
	}
	return SYS_ERR_NOT_IMPLEMENTED;
}

SysErr GI::IO::read(bool *state)
{
	if(!devHandler || !ioDevType)
		return SYS_ERR_NO_REGISTERED_DEVICE;
	switch((int)ioDevType)
	{
	case IO_DEV_GPIO:
		u32 data;
		SysErr res = ((GI::Dev::Gpio *)devHandler)->getIn(&data);
		if(data)
			*state = true;
		else
			*state = false;
		return res;
	}
	return SYS_ERR_NOT_IMPLEMENTED;
}

SysErr GI::IO::read(u32 *data)
{
	if(!devHandler || !ioDevType)
		return SYS_ERR_NO_REGISTERED_DEVICE;
	switch((int)ioDevType)
	{
	case IO_DEV_GPIO:
		return ((GI::Dev::Gpio *)devHandler)->getIn(data);
	}
	return SYS_ERR_NOT_IMPLEMENTED;
}
/**********************************************************************************/
SysErr GI::IO::ctl(GI::IO::ioCtl_e cmd, unsigned long *data)
{
	if(!devHandler || !ioDevType)
		return SYS_ERR_NO_REGISTERED_DEVICE;
	ioCtlRwMsgs_t *msgs = (ioCtlRwMsgs_t *)data;
	ioCtlMsg_t *msg = msgs->msgs;
	switch((int)cmd)
	{
	case IO_CTL_NONE:
		switch((int)ioDevType)
		{
		case IO_DEV_GPIO:
			((GI::Dev::Gpio *)devHandler)->idle();
			return SYS_ERR_OK;
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_WR:
		if(msgs->nMsgs != 2)
			return SYS_ERR_INVALID_COMMAND;
		switch((int)ioDevType)
		{
		case IO_DEV_I2C:
			return ((GI::Dev::I2c *)devHandler)->writeRead(msg[0].slaveAddr, msg[0].buff, msg[0].len, msg[1].buff, msg[1].len);
		case IO_DEV_SPI:
			if(msg[0].len == msg[1].len)
				return ((GI::Dev::Spi *)devHandler)->writeRead(msg[0].buff, msg[1].buff, msg[0].len);
			else
			{
				if(((GI::Dev::Spi *)devHandler)->writeBytes(msg[0].buff, msg[0].len) != SYS_ERR_OK)
					return SYS_ERR_WRITE;
				if(((GI::Dev::Spi *)devHandler)->readBytes(msg[1].buff, msg[1].len) != SYS_ERR_OK)
					return SYS_ERR_READ;
			}
			break;
		case IO_DEV_UART:
			((GI::Dev::Uart *)devHandler)->write((char *)msg[0].buff, msg[0].len);
			return SYS_ERR_OK;
		}
		break;
	case IO_CTL_R:
		if(msgs->nMsgs != 1)
			return SYS_ERR_INVALID_COMMAND;
		switch((int)ioDevType)
		{
		case IO_DEV_I2C:
			if(((GI::Dev::I2c *)devHandler)->readBytes(msg[0].slaveAddr, msg[0].buff, msg[0].len) != (int)msg[0].len)
				return SYS_ERR_UNKNOWN;
			else
				return SYS_ERR_OK;
		case IO_DEV_SPI:
			if(((GI::Dev::Spi *)devHandler)->readBytes(msg[0].buff, msg[0].len) != (int)msg[0].len)
			return SYS_ERR_UNKNOWN;
		else
			return SYS_ERR_OK;
		//case IO_DEV_UART:
			//return ((GI::Dev::Uart *)devHandler)->print((char *)buff);
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_W:
		switch((int)ioDevType)
		{
		case IO_DEV_I2C:
			if(((GI::Dev::I2c *)devHandler)->writeBytes(msg[0].slaveAddr, msg[0].buff, msg[0].len) != (int)msg[0].len)
				return SYS_ERR_UNKNOWN;
			else
				return SYS_ERR_OK;
		case IO_DEV_SPI:
			if(((GI::Dev::Spi *)devHandler)->writeBytes(msg[0].buff, msg[0].len) != (int)msg[0].len)
				return SYS_ERR_UNKNOWN;
			else
				return SYS_ERR_OK;
		case IO_DEV_UART:
			((GI::Dev::Uart *)devHandler)->write((char *)msg[0].buff, msg[0].len);
			return SYS_ERR_OK;
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_SET_SLAVE_ADDR:
		slaveAddr = *data;
		break;
	case IO_CTL_GET_SLAVE_ADDR:
		*data = slaveAddr;
		break;
	case IO_CTL_SET_MODE:
		switch((int)ioDevType)
		{
		case IO_DEV_GPIO:
			return ((GI::Dev::Gpio *)devHandler)->setMode((CfgGpio::gpioMode_e)*data);
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_GET_MODE:
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_SET_SPEED:
		switch((int)ioDevType)
		{
		case IO_DEV_UART:
			return ((GI::Dev::Uart *)devHandler)->setSpeed(*data);
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_GET_SPEED:
		switch((int)ioDevType)
		{
		case IO_DEV_UART:
			return ((GI::Dev::Uart *)devHandler)->getSpeed(data);
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_SET_WLEN:
		switch((int)ioDevType)
		{
		case IO_DEV_UART:
			((GI::Dev::Uart *)devHandler)->setWordLen((CfgUart::wordLen_e)*data);
			return SYS_ERR_OK;
		return SYS_ERR_NOT_IMPLEMENTED;
		}
		break;
	case IO_CTL_GET_WLEN:
		switch((int)ioDevType)
		{
		case IO_DEV_UART:
			return ((GI::Dev::Uart *)devHandler)->getWordLen((CfgUart::wordLen_e *)data);
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_SET_SBITS:
		switch((int)ioDevType)
		{
		case IO_DEV_UART:
			((GI::Dev::Uart *)devHandler)->setStopBits((CfgUart::stopBits_e)*data);
			return SYS_ERR_OK;
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_GET_SBITS:
		switch((int)ioDevType)
		{
		case IO_DEV_UART:
			return ((GI::Dev::Uart *)devHandler)->getStopBits((CfgUart::stopBits_e *)data);
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_SET_PBITS:
		switch((int)ioDevType)
		{
		case IO_DEV_UART:
			((GI::Dev::Uart *)devHandler)->setParBits((CfgUart::parity_e)*data);
			return SYS_ERR_OK;
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_GET_PBITS:
		switch((int)ioDevType)
		{
		case IO_DEV_UART:
			return ((GI::Dev::Uart *)devHandler)->getParBits((CfgUart::parity_e *)data);
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_SET:
		switch((int)ioDevType)
		{
		case IO_DEV_GPIO:
			return ((GI::Dev::Gpio *)devHandler)->setOut(*data);
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_GET:
		switch((int)ioDevType)
		{
		case IO_DEV_GPIO:
			return ((GI::Dev::Gpio *)devHandler)->getIn(data);
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	}
	return SYS_ERR_OK;
}
/**********************************************************************************/
