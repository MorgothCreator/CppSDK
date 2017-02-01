/*
 *  lib/device/ak8975.cpp
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
#include "api/timer.h"
#include <api/dev_request.h>
#include "ak8975.h"

GI::Sensor::Ak8975::Ak8975(char *i2cPath, unsigned char icNr)
{
	memset(this, 0, sizeof(*this));
	GI::Dev::DevRequest::request(i2cPath, &I2C);
	if(!I2C)
		return;
	this->IcNr = icNr & 0x03;
}

GI::Sensor::Ak8975::~Ak8975()
{

}

SysErr GI::Sensor::Ak8975::startMeasure() {
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char buff[2];
	buff[0] = AK8975_CNTL_REG;
	buff[1] = AK8975_CTRL_MODE_SINGLE;
	if(I2C->writeBytes(AK8975_ADDR | (IcNr & 0x03), buff, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Ak8975::checkReady() {
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char addr = AK8975_ST1_REG;
	unsigned char tmp = 0;
	if(I2C->writeRead(AK8975_ADDR | (IcNr & 0x03), &addr, 1, &tmp, 1))
		return SYS_ERR_BUS;
	if(tmp && AK8975_ST1_DREADY_bm)
		return SYS_ERR_OK;
	else
		return SYS_ERR_BUSY;
}

SysErr GI::Sensor::Ak8975::getData(signed short *X_Axis, signed short *Y_Axis, signed short *Z_Axis) {
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char addr = AK8975_HXL_REG;
	unsigned char result[6];
	if(I2C->writeRead(AK8975_ADDR | (IcNr & 0x03), &addr, 1, result, 6))
		return SYS_ERR_BUS;
	*X_Axis = (signed short)((result[1] << 8) + result[0]);
	*Y_Axis = (signed short)((result[3] << 8) + result[2]);
	*Z_Axis = (signed short)((result[5] << 8) + result[4]);
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Ak8975::getMag(signed short *X_Axis, signed short *Y_Axis, signed short *Z_Axis) {
	SysErr err;
	if((err = startMeasure()) != SYS_ERR_OK)
		return err;
	GI::Sys::Timer Timeout = GI::Sys::Timer(11);
	while(checkReady()) {
		if(Timeout.tick())
			return SYS_ERR_TIMEOUT;
	}
	if((err = getData(X_Axis, Y_Axis, Z_Axis)))
		return err;
	return SYS_ERR_OK;
}

/*SysErr GI::Sensor::Ak8975::ak8975_display_result(AK8975_t *structure) {
	signed short Xg = 0;
	signed short Yg = 0;
	signed short Zg = 0;
	if(!ak8975_start_measure(structure))
		return false;
	GI::Sys::Timer Timeout = GI::Sys::Timer(11);
	while(!ak8975_ready(structure)) {
		if(Timeout.tick())
			return false;
	}
	if(!ak8975_read_data(structure, &Xg, &Yg, &Zg))
		return false;
	uart.printf(DebugCom, "AK8975: Magnetometer: Xg = %d, Yg = %d, Zg = %d\n\r", Xg, Yg, Zg);
	return true;
}
*/

