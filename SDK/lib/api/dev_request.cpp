/*
 *  api/dev_request.cpp
 */

#include <api/init.h>
#include "dev_request.h"

extern ioSettings *ioSetCfg[];


SysErr GI::Dev::DevRequest::request(char *path, GI::Dev::Gpio **device)
{
	unsigned int dev_nr = 0;
	while(1)
	{
		ioSettings *io_item = ioSetCfg[dev_nr];
		if(!io_item || !io_item->cfg)
		{
			*device = NULL;
			return SYS_ERR_INVALID_PATH;
		}
		if(io_item->info.ioType == ioSettings::info_s::ioType_GPIO && !strcmp(io_item->info.name, path))
			break;
		dev_nr++;
	}
	*device = (GI::Dev::Gpio *)ioSetCfg[dev_nr]->ioConstruct;
	return SYS_ERR_OK;
}

SysErr GI::Dev::DevRequest::request(char *path, GI::Dev::I2c **device)
{
	unsigned int dev_nr = 0;
	while(1)
	{
		ioSettings *io_item = ioSetCfg[dev_nr];
		if(!io_item || !io_item->cfg)
		{
			*device = NULL;
			return SYS_ERR_INVALID_PATH;
		}
		if(io_item->info.ioType == ioSettings::info_s::ioType_I2C && !strcmp(io_item->info.name, path))
			break;
		dev_nr++;
	}
	*device = (GI::Dev::I2c *)ioSetCfg[dev_nr]->ioConstruct;
	return SYS_ERR_OK;
}

SysErr GI::Dev::DevRequest::request(char *path, GI::Dev::Spi **device)
{
	unsigned int dev_nr = 0;
	while(1)
	{
		ioSettings *io_item = ioSetCfg[dev_nr];
		if(!io_item || !io_item->cfg)
		{
			*device = NULL;
			return SYS_ERR_INVALID_PATH;
		}
		if(io_item->info.ioType == ioSettings::info_s::ioType_SPI && !strcmp(io_item->info.name, path))
			break;
		dev_nr++;
	}
	*device = (GI::Dev::Spi *)ioSetCfg[dev_nr]->ioConstruct;
	return SYS_ERR_OK;
}

SysErr GI::Dev::DevRequest::request(char *path, GI::Dev::Uart **device)
{
	unsigned int dev_nr = 0;
	while(1)
	{
		ioSettings *io_item = ioSetCfg[dev_nr];
		if(!io_item || !io_item->cfg)
		{
			*device = NULL;
			return SYS_ERR_INVALID_PATH;
		}
		if(io_item->info.ioType == ioSettings::info_s::ioType_UART && !strcmp(io_item->info.name, path))
			break;
		dev_nr++;
	}
	*device = (GI::Dev::Uart *)ioSetCfg[dev_nr]->ioConstruct;
	return SYS_ERR_OK;
}
