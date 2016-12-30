/*
 *  lib/device/mpl3115a2.c
 *
 *  Copyright (C) 2015  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
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

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "mpl3115a2.h"
#include <api/dev_request.h>

GI::Sensor::Mpl3115a2::Mpl3115a2(char *i2cPath)
{
	memset(this, 0, sizeof(*this));
	GI::Dev::DevRequest::request(i2cPath, &I2C);
	if(!I2C)
		return;
}

GI::Sensor::Mpl3115a2::~Mpl3115a2()
{

}

SysErr GI::Sensor::Mpl3115a2::regRead(unsigned char reg, unsigned char *data, unsigned char len)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	if(I2C->writeRead(MPL3115A2_ADDR, &reg, 1, data, len))
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpl3115a2::regWrite(unsigned char reg, unsigned char *data, unsigned char len)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char *tmp = (unsigned char *)malloc(len + 1);
	tmp[0] = reg;
	memcpy(tmp + 1, data, len);
	if(I2C->writeBytes(MPL3115A2_ADDR, tmp, 1 + len) != 1 + len)
	{
		free(tmp);
		return SYS_ERR_BUS;
	}
	free(tmp);
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpl3115a2::getAltTemp(unsigned char oversample, float *pressure, float *altitude, float *temperature)
{
	unsigned long out_p = 0;
	//signed short out_t = 0;
	unsigned char tmp_buff[5];
	unsigned char tmp;
	tmp = (1 << 7) | ((oversample & 0x07) << 3);
	SysErr err;
	if((err = regWrite(MPL3115A2_CTRL_REG1, &tmp, 1)))
		return err;
	tmp = 0x07;
	if((err = regWrite(MPL3115A2_PT_DATA_CFG, &tmp, 1)))
		return err;
	tmp = (1 << 7) | ((oversample & 0x07) << 3) | 1;
	if((err = regWrite(MPL3115A2_CTRL_REG1, &tmp, 1)))
		return err;
	unsigned int timeout = 1000;
	do {
		if((err = regRead(MPL3115A2_STATUS, &tmp, 1)))
			return err;
		if(!timeout--)
			return SYS_ERR_TIMEOUT;
		GI::Sys::Timer::delay(2);
	}while(!(tmp & 0x08));
	if((err = regRead(MPL3115A2_OUT_P_MSB, tmp_buff, 5)))
		return err;
	out_p = ((tmp_buff[0] << 12) + (tmp_buff[1] << 4) + (tmp_buff[2] >> 4));
	//if(!mpl3115a2_reg_read(structure, MPL3115A2_OUT_T_MSB, tmp_buff, 2))
		//return false;
	signed char _tmp = tmp_buff[3];
	float temp = (float)_tmp;
	unsigned char temp_frac = tmp_buff[4] >> 4;
	if((err = regRead(MPL3115A2_OFF_H, &tmp, 1)))
		return err;
	*altitude = (44330.77 * (1.0 - ((float)out_p / (float)101326.0) * 0.1902632) + tmp) / 1000.0;
	*temperature = temp + ((1.0 * temp_frac) / 256.0);
	return SYS_ERR_OK;
}
