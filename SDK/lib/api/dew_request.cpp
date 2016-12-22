/*
 * dew_request.cpp
 *
 *  Created on: Dec 22, 2016
 *      Author: John Smith
 */

#include "dev_request.h"
#include <api/init.h>

SysErr GI::Dev::DevRequest::request(char *path, GI::Dev::Gpio **device)
{
	unsigned int dev_nr = 0;
	while(1)
	{
		if(dev.GPIO[dev_nr] == NULL || dev.GPIO[dev_nr]->cfg.name[0] == 0)
			return SYS_ERR_INVALID_PATH;
		if(!strcmp(dev.GPIO[dev_nr]->cfg.name, path))
			break;
		dev_nr++;
	}
	*device = dev.GPIO[dev_nr];
	return SYS_ERR_OK;
}

SysErr GI::Dev::DevRequest::request(char *path, GI::Dev::I2c **device)
{
	unsigned int dev_nr = 0;
	while(1)
	{
		if(dev.I2C[dev_nr] == NULL || dev.I2C[dev_nr]->cfg.name == NULL)
			return SYS_ERR_INVALID_PATH;
		if(!strcmp(dev.I2C[dev_nr]->cfg.name, path))
			break;
		dev_nr++;
	}
	*device = dev.I2C[dev_nr];
	return SYS_ERR_OK;
}

SysErr GI::Dev::DevRequest::request(char *path, GI::Dev::Spi **device)
{
	unsigned int dev_nr = 0;
	while(1)
	{
		if(dev.SPI[dev_nr] == NULL || dev.SPI[dev_nr]->cfg.name == NULL)
			return SYS_ERR_INVALID_PATH;
		if(!strcmp(dev.SPI[dev_nr]->cfg.name, path))
			break;
		dev_nr++;
	}
	*device = dev.SPI[dev_nr];
	return SYS_ERR_OK;
}

SysErr GI::Dev::DevRequest::request(char *path, GI::Dev::Uart **device)
{
	unsigned int dev_nr = 0;
	while(1)
	{
		if(dev.UART[dev_nr] == NULL || dev.UART[dev_nr]->cfg.name == NULL)
			return SYS_ERR_INVALID_PATH;
		if(!strcmp(dev.UART[dev_nr]->cfg.name, path))
			break;
		dev_nr++;
	}
	*device = dev.UART[dev_nr];
	return SYS_ERR_OK;
}
