/*
 *  api/dev_request.cpp
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

#include <api/init.h>
#include "dev_request.h"

SysErr GI::Dev::DevRequest::request(char *path, GI::Dev::Gpio **device)
{
	unsigned int dev_nr = 0;
	while(1)
	{
		if(dev.GPIO == NULL || dev.GPIO[dev_nr] == NULL || dev.GPIO[dev_nr]->cfg.name[0] == 0 || dev.GPIO[dev_nr]->cfg.name[0] == '\0')
		{
			*device = NULL;
			return SYS_ERR_INVALID_PATH;
		}
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
		if(dev.I2C == NULL || dev.I2C[dev_nr] == NULL || dev.I2C[dev_nr]->cfg.name == NULL || dev.I2C[dev_nr]->cfg.name[0] == '\0')
		{
			*device = NULL;
			return SYS_ERR_INVALID_PATH;
		}
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
		if(dev.SPI == NULL || dev.SPI[dev_nr] == NULL || dev.SPI[dev_nr]->cfg.name == NULL || dev.SPI[dev_nr]->cfg.name[0] == '\0')
		{
			*device = NULL;
			return SYS_ERR_INVALID_PATH;
		}
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
		if(dev.UART == NULL || dev.UART[dev_nr] == NULL || dev.UART[dev_nr]->cfg.name == NULL || dev.UART[dev_nr]->cfg.name[0] == '\0')
		{
			*device = NULL;
			return SYS_ERR_INVALID_PATH;
		}
		if(!strcmp(dev.UART[dev_nr]->cfg.name, path))
			break;
		dev_nr++;
	}
	*device = dev.UART[dev_nr];
	return SYS_ERR_OK;
}
