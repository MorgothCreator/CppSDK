/*
 *  device/hih613x.cpp
 */
#include <stdbool.h>
#include <string.h>
#include "hih6130.h"
#include "api/i2c.h"
#include "api/timer.h"
#include <api/dev_request.h>

GI::Sensor::Hih613x::Hih613x(char *i2cPath)
{
	memset(this, 0, sizeof(*this));
	GI::Dev::DevRequest::request(i2cPath, &I2C);

}

GI::Sensor::Hih613x::~Hih613x()
{

}

SysErr GI::Sensor::Hih613x::get(unsigned char *status, float *hum, float *temp) {
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	if(I2C->writeBytes(HIH613x_ADDR, NULL, 0))
		return SYS_ERR_BUS;
	//Sysdelay(100);
	unsigned int timeout = 100;
	I2C->noSendWriteOnRead = true;
	unsigned char result[4];
	do {
		GI::Sys::Timer::delay(2);
		if(I2C->writeRead(HIH613x_ADDR, NULL, 0, result, 4)) {
			I2C->noSendWriteOnRead = false;
			return SYS_ERR_BUS;
		}
		if(!timeout--)
			return SYS_ERR_TIMEOUT;
	}while(((result[0] >> 6) & 0x03) == 0x01);
	I2C->noSendWriteOnRead = false;
	*status = (result[0] >> 6) & 0x03;
	result[0] = result[0] & 0x3F;
	*hum = (float)((((unsigned short)result[0]) << 8) | result[1]) * 6.10e-3;
	*temp = (float)(((((unsigned short)result[2]) << 8) | result[3]) >> 2) * 1.007e-2 - 40.0;
	return SYS_ERR_OK;
}
