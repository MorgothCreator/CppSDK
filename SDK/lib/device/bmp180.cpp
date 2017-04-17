/*
 *  lib/device/bmp180.c
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
#include "bmp180.h"
#include "api/i2c.h"
#include "api/timer.h"
#include <api/dev_request.h>

GI::Sensor::Bmp180::Bmp180(char *i2cPath, unsigned char icNr)
{
	memset(this, 0, sizeof(*this));
	GI::Dev::DevRequest::request(i2cPath, &I2C);
	if(!I2C)
		return;
	this->IcNr = icNr & 0x03;
}

GI::Sensor::Bmp180::~Bmp180()
{

}

SysErr GI::Sensor::Bmp180::readCalibration() {
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char addr = BMP180_CALIB;
	unsigned char result[BMP180_CALIB_LEN];
	SysErr err;
	if((err = I2C->writeRead(BMP180_ADDR | (IcNr & 0x01), &addr, 1, result, BMP180_CALIB_LEN)))
		return err;
	cal_data.AC1 = (signed short)((result[0] << 8) + result[1]);
	cal_data.AC2 = (signed short)((result[2] << 8) + result[3]);
	cal_data.AC3 = (signed short)((result[4] << 8) + result[5]);
	cal_data.AC4 = (unsigned short)((result[6] << 8) + result[7]);
	cal_data.AC5 = (unsigned short)((result[8] << 8) + result[9]);
	cal_data.AC6 = (unsigned short)((result[10] << 8) + result[11]);
	cal_data.B1 = (unsigned short)((result[12] << 8) + result[13]);
	cal_data.B2 = (unsigned short)((result[14] << 8) + result[15]);
	cal_data.MB = (unsigned short)((result[16] << 8) + result[17]);
	cal_data.MC = (unsigned short)((result[18] << 8) + result[19]);
	cal_data.MD = (unsigned short)((result[20] << 8) + result[21]);
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Bmp180::readTemp(signed long *temp) {
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	tmp[0] = BMP180_CTRL_MEAS;
	tmp[1] = 0x2E;
	if(I2C->writeBytes(BMP180_ADDR | (IcNr & 0x03), tmp, 2) != 2)
		return SYS_ERR_BUS;
	GI::Sys::Timer::delay(6);
	unsigned char reg = BMP180_OUT_MSB;
	unsigned char result[2];
	SysErr err;
	if((err = I2C->writeRead(BMP180_ADDR | (IcNr & 0x03), &reg, 1, result, 2)))
		return err;
	*temp = (signed long)((result[0] << 8) + result[1]);
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Bmp180::readPressure(signed long *pressure, unsigned char oss) {
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	tmp[0] = BMP180_CTRL_MEAS;
	tmp[1] = 0x34 + (oss << BMP180_CTRL_MEAS_OSS_gp);
	if(I2C->writeBytes(BMP180_ADDR | (IcNr & 0x03), tmp, 2) != 2)
		return SYS_ERR_BUS;
	GI::Sys::Timer::delay(6 * (1 << oss));
	unsigned char reg = BMP180_OUT_MSB;
	unsigned char result[3];
	SysErr err;
	if((err = I2C->writeRead(BMP180_ADDR | (IcNr & 0x03), &reg, 1, result, 3)))
		return err;
	*pressure = (signed long)(((unsigned long)result[0] << 16) + ((unsigned long)result[1] << 8) + ((unsigned long)result[0] >> (8 - oss)));
	return SYS_ERR_OK;
}

/*SysErr bmp180_display_result(BMP180_t *structure, unsigned char oss) {
	if(!TWI)
		return false;
	if(!bmp180_read_calibration(structure))
		return false;
	signed long UT = 0, UP = 0;
	if(!bmp180_read_temp(structure, &UT))
		return false;
	signed long X1 = (UT - cal_data.AC6) * cal_data.AC5 / 32768;
	signed long X2 = cal_data.MC * 2048 / (X1 + cal_data.MD);
	signed long B5 = X1 + X2;
	signed long T = (B5 + 8) / 16;

	if(!bmp180_read_pressure(structure, &UP, oss))
		return false;
	signed long B6 = B5 - 4000;
	X1 = (cal_data.B2 * (B6 * B6 / 4096)) / 2048;
	X2 = cal_data.AC2 * B6 / 2048;
	signed long X3 = X1 + X2;
	signed long B3 = (((cal_data.AC1 * 4 + X3) << oss) + 2) / 4;
	X1 = cal_data.AC3 * B6 / 8192;
	X2 = (cal_data.B1 * (B6 * B6 / 4096)) / 65536;
	X3 = ((X1 + X2) + 2) / 4;
	unsigned long B4 = cal_data.AC4 * (unsigned long)(X3 + 32768) / 32768;
	unsigned long B7 = ((unsigned long)UP - B3) * (50000 >> oss);
	signed long p = 0;
	if(B7 < 80000000) {
		p = (B7 * 2) / B4;
	} else {
		p = (B7 / B4) * 2;
	}
	X1 = (p / 256) * (p / 256);
	X1 = (X1 * 3038) / 65536;
	X2 = (-7357 * p) / 65536;
	p = p + (X1 + X2 + 3791) / 16;

	float altitude = 44330.0 * ((1.0 - (((float)p / 100.0) / 1013.25)) * (1.0 / 5.255));
	uart.printf(DebugCom, "BMP180: T = %2.1f, P = %4.2f, Alt = %4.2f\n\r", (float)(T / 10.0), (float)p / 100.0, altitude);
	return true;
}
*/
SysErr GI::Sensor::Bmp180::getTemp(float *temp) {
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	SysErr err;
	if((err = readCalibration()))
		return err;
	signed long UT = 0;
	if((err = readTemp(&UT)))
		return err;
	signed long X1 = (UT - cal_data.AC6) * cal_data.AC5 / 32768;
	signed long X2 = cal_data.MC * 2048 / (X1 + cal_data.MD);
	signed long B5 = X1 + X2;
	signed long T = (B5 + 8) / 16;
	*temp = (float)(T / 10.0);
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Bmp180::getPressure(float *pressure, unsigned char oss) {
	SysErr err;
	if((err = readCalibration()))
		return err;
	signed long UT = 0, UP = 0;
	if((err = readTemp(&UT)))
		return err;
	signed long X1 = (UT - cal_data.AC6) * cal_data.AC5 / 32768;
	signed long X2 = cal_data.MC * 2048 / (X1 + cal_data.MD);
	signed long B5 = X1 + X2;
	//signed long T = (B5 + 8) / 16;

	if((err = readPressure(&UP, oss)))
		return err;
	signed long B6 = B5 - 4000;
	X1 = (cal_data.B2 * (B6 * B6 / 4096)) / 2048;
	X2 = cal_data.AC2 * B6 / 2048;
	signed long X3 = X1 + X2;
	signed long B3 = (((cal_data.AC1 * 4 + X3) << oss) + 2) / 4;
	X1 = cal_data.AC3 * B6 / 8192;
	X2 = (cal_data.B1 * (B6 * B6 / 4096)) / 65536;
	X3 = ((X1 + X2) + 2) / 4;
	unsigned long B4 = cal_data.AC4 * (unsigned long)(X3 + 32768) / 32768;
	unsigned long B7 = ((unsigned long)UP - B3) * (50000 >> oss);
	signed long p = 0;
	if(B7 < 80000000) {
		p = (B7 * 2) / B4;
	} else {
		p = (B7 / B4) * 2;
	}
	X1 = (p / 256) * (p / 256);
	X1 = (X1 * 3038) / 65536;
	X2 = (-7357 * p) / 65536;
	p = p + (X1 + X2 + 3791) / 16;
	*pressure = (float)p / 100.0;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Bmp180::getAltitude(float *altitude, unsigned char oss) {
	float pressure = 0.0;
	SysErr err;
	if((err = getPressure(&pressure, oss)))
		return err;
	*altitude = 44330.0 * ((1.0 - (pressure / 1013.25)) * (1.0 / 5.255));
	return SYS_ERR_OK;
}

