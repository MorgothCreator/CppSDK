/*
 *  api/io_handler.cpp
 */

#include "io_handle.h"

#include <stdio.h>

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
int GI::IO::write(char *path, unsigned char *buff, unsigned int len)
{
	GI::IO tmp_io = GI::IO((char *)path);
	return tmp_io.write(buff, len);
}

int GI::IO::write(char *path, unsigned char *buff)
{
	GI::IO tmp_io = GI::IO((char *)path);
	return tmp_io.write(buff);
}

int GI::IO::write(char *path, char *buff)
{
	GI::IO tmp_io = GI::IO((char *)path);
	return tmp_io.write(buff);
}

int GI::IO::write(char *path, GI::String *string)
{
	GI::IO tmp_io = GI::IO((char *)path);
	return tmp_io.write(string);
}

SysErr GI::IO::write(char *path, bool state)/* Bit device */
{
	GI::IO tmp_io = GI::IO((char *)path);
	return tmp_io.write(state);
}

SysErr GI::IO::write(char *path, u32 data)
{
	GI::IO tmp_io = GI::IO((char *)path);
	return tmp_io.write(data);
}

SysErr GI::IO::write(char *path, char data)
{
	GI::IO tmp_io = GI::IO((char *)path);
	return tmp_io.write(data);
}

int GI::IO::writeF(char *path, char *buff, ...)
{
	GI::IO tmp_io = GI::IO((char *)path);
    va_list args;
    //char *pcString = (char *)buff;
    va_start (args, buff);
    char tmp_buff[1];
    s32 len = vsnprintf(tmp_buff, 0, (char *)buff, args);
    va_end (args);
    int lend_send = 0;
    if(len > 0)
    {
        char *tmp_str = (char *)malloc(len + 1);
        if(tmp_str)
        {
            va_start (args, buff);
            vsnprintf(tmp_str, len + 1, (char *)buff, args);
            va_end (args);
            lend_send = tmp_io.write(tmp_str);
            free(tmp_str);
        }
    }
    return lend_send;
}

int GI::IO::writeF(char *path, unsigned char *buff, ...)
{
	GI::IO tmp_io = GI::IO((char *)path);
    va_list args;
    //char *pcString = (char *)buff;
    va_start (args, buff);
    char tmp_buff[1];
    s32 len = vsnprintf(tmp_buff, 0, (char *)buff, args);
    va_end (args);
    int lend_send = 0;
    if(len > 0)
    {
        char *tmp_str = (char *)malloc(len + 1);
        if(tmp_str)
        {
            va_start (args, buff);
            vsnprintf(tmp_str, len + 1, (char *)buff, args);
            va_end (args);
            lend_send = tmp_io.write(tmp_str);
            free(tmp_str);
        }
    }
    return lend_send;
}

int GI::IO::writeF(char *path, GI::String *string, ...)
{
	GI::IO tmp_io = GI::IO((char *)path);
    va_list args;
    va_start (args, string->buff);
    char buff[1];
    s32 len = vsnprintf(buff, 0, string->buff, args);
    va_end (args);
    int lend_send = 0;
    if(len > 0)
    {
        char *tmp_str = (char *)malloc(len + 1);
        if(tmp_str)
        {
            va_start (args, string->buff);
            vsnprintf(tmp_str, len + 1, string->buff, args);
            va_end (args);
            lend_send = tmp_io.write(tmp_str);
            free(tmp_str);
        }
    }
    return lend_send;
}
/**********************************************************************************/
int GI::IO::write(unsigned char *buff, unsigned int len)
{
	if(!devHandler || !ioDevType || !buff)
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

int GI::IO::write(unsigned char *buff)
{
    if(!devHandler || !ioDevType || !buff)
        return SYS_ERR_NO_REGISTERED_DEVICE;
    switch((int)ioDevType)
    {
    case IO_DEV_UART:
        return ((GI::Dev::Uart *)devHandler)->print((char *)buff);
    }
    return SYS_ERR_NOT_IMPLEMENTED;
}

int GI::IO::write(char *buff)
{
    if(!devHandler || !ioDevType || !buff)
        return SYS_ERR_NO_REGISTERED_DEVICE;
    switch((int)ioDevType)
    {
    case IO_DEV_UART:
        return ((GI::Dev::Uart *)devHandler)->print((char *)buff);
    }
    return SYS_ERR_NOT_IMPLEMENTED;
}

int GI::IO::write(GI::String *string)
{
    if(!devHandler || !ioDevType || !string)
        return SYS_ERR_NO_REGISTERED_DEVICE;
    switch((int)ioDevType)
    {
    case IO_DEV_UART:
        return ((GI::Dev::Uart *)devHandler)->print((char *)string->buff);
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

SysErr GI::IO::write(char data)
{
	if(!devHandler || !ioDevType)
		return SYS_ERR_NO_REGISTERED_DEVICE;
	switch((int)ioDevType)
	{
	case IO_DEV_UART:
		if(((GI::Dev::Uart *)devHandler)->write((char *)&data, 1) != 1)
			return SYS_ERR_BUSY;
		else
			return SYS_ERR_OK;
	}
	return SYS_ERR_NOT_IMPLEMENTED;
}

int GI::IO::writeF(char *buff, ...)
{
    va_list args;
    //char *pcString = buff;
    va_start (args, buff);
    char tmp_buff[1];
    s32 len = vsnprintf(tmp_buff, 0, buff, args);
    va_end (args);
    int lend_send = 0;
    if(len > 0)
    {
        char *tmp_str = (char *)malloc(len + 1);
        if(tmp_str)
        {
            va_start (args, buff);
            vsnprintf(tmp_str, len + 1, buff, args);
            va_end (args);
            lend_send = write(tmp_str);
            free(tmp_str);
        }
    }
    return lend_send;
}

int GI::IO::writeF(unsigned char *buff, ...)
{
    va_list args;
    //char *pcString = (char *)buff;
    va_start (args, buff);
    char tmp_buff[1];
    s32 len = vsnprintf(tmp_buff, 0, (char *)buff, args);
    va_end (args);
    int lend_send = 0;
    if(len > 0)
    {
        char *tmp_str = (char *)malloc(len + 1);
        if(tmp_str)
        {
            va_start (args, buff);
            vsnprintf(tmp_str, len + 1, (char *)buff, args);
            va_end (args);
            lend_send = write(tmp_str);
            free(tmp_str);
        }
    }
    return lend_send;
}

int GI::IO::writeF(GI::String *string, ...)
{
    va_list args;
    va_start (args, string->buff);
    char buff[1];
    s32 len = vsnprintf(buff, 0, string->buff, args);
    va_end (args);
    int lend_send = 0;
    if(len > 0)
    {
        char *tmp_str = (char *)malloc(len + 1);
        if(tmp_str)
        {
            va_start (args, string->buff);
            vsnprintf(tmp_str, len + 1, string->buff, args);
            va_end (args);
            lend_send = write(tmp_str);
            free(tmp_str);
        }
    }
    return lend_send;
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
	signed short tmp = -1;
	switch((int)ioDevType)
	{
	case IO_DEV_GPIO:
		return ((GI::Dev::Gpio *)devHandler)->getIn(data);
	case IO_DEV_UART:
		tmp = ((GI::Dev::Uart *)devHandler)->getCharNb();
		if(tmp == -1)
			return SYS_ERR_NOT_RECEIVED;
		else
		{
			*data = tmp;
			return SYS_ERR_OK;
		}
	}
	return SYS_ERR_NOT_IMPLEMENTED;
}
/**********************************************************************************/
int GI::IO::read(char *path, unsigned char *buff, unsigned int len)/*Char device*/
{
	GI::IO tmp_io = GI::IO((char *)path);
	return tmp_io.read(buff, len);
}

SysErr GI::IO::read(char *path, bool *state)/* Bit device */
{
	GI::IO tmp_io = GI::IO((char *)path);
	return tmp_io.read(state);
}

SysErr GI::IO::read(char *path, u32 *data)
{
	GI::IO tmp_io = GI::IO((char *)path);
	return tmp_io.read(data);
}

/**********************************************************************************/
SysErr GI::IO::ctl(GI::IO::ioCtl_e cmd, unsigned long *data)
{
	if(!devHandler || !ioDevType)
		return SYS_ERR_NO_REGISTERED_DEVICE;
	ioCtlRwMsgs_t *msgs = (ioCtlRwMsgs_t *)data;
	ioCtlMsg_t *msg = msgs->msgs;
	//unsigned char *tmp_buff;
	//SysErr error;
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
			//if(msg[0].len == msg[1].len)
			//	return ((GI::Dev::Spi *)devHandler)->writeRead(msg[0].buff, msg[1].buff, msg[0].len);
			//else
			//{
				/*if(((GI::Dev::Spi *)devHandler)->writeBytes(msg[0].buff, msg[0].len) != SYS_ERR_OK)
					return SYS_ERR_WRITE;
				if(((GI::Dev::Spi *)devHandler)->readBytes(msg[1].buff, msg[1].len) != SYS_ERR_OK)
					return SYS_ERR_READ;*/
				return ((GI::Dev::Spi *)devHandler)->writeRead(msg[0].buff, msg[0].len, msg[1].buff, msg[1].len);
			//}
			//break;
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
	case IO_CTL_ASSERT:
		switch((int)ioDevType)
		{
		case IO_DEV_SPI:
			return (SysErr)((GI::Dev::Spi *)devHandler)->assert();
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	case IO_CTL_DEASSERT:
		switch((int)ioDevType)
		{
		case IO_DEV_SPI:
			return (SysErr)((GI::Dev::Spi *)devHandler)->deassert();
		}
		return SYS_ERR_NOT_IMPLEMENTED;
		break;
	}
	return SYS_ERR_OK;
}
/**********************************************************************************/
