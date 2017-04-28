/*
 * device/mpr121.cpp
 */ 

//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <avr/pgmspace.h>
#include <stdbool.h>
#include <string.h>
#include "mpr121.h"
#include <api/dev_request.h>

#ifdef __AVR_XMEGA__
static const mpr121_setup mpr121_set[] PROGMEM = {
#else
static const mpr121_setup mpr121_set[] = {
#endif
	{MPR_RESET, 0x63},
	{MHD_R, 0x01},
	{NCL_R, 0x00},
	{FDL_R, 0x00},
	{NHD_F, 0x01},
	{NCL_F, 0xFF},
	{FDL_F, 0x02},

	{ELE0_T, TOU_THRESH},
	{ELE0_R, REL_THRESH},

	{ELE1_T, TOU_THRESH},
	{ELE1_R, REL_THRESH},

	{ELE2_T, TOU_THRESH},
	{ELE2_R, REL_THRESH},

	{ELE3_T, TOU_THRESH},
	{ELE3_R, REL_THRESH},

	{ELE4_T, TOU_THRESH},
	{ELE4_R, REL_THRESH},

	{ELE5_T, TOU_THRESH},
	{ELE5_R, REL_THRESH},

	{ELE6_T, TOU_THRESH},
	{ELE6_R, REL_THRESH},

	{ELE7_T, TOU_THRESH},
	{ELE7_R, REL_THRESH},

	{ELE8_T, TOU_THRESH},
	{ELE8_R, REL_THRESH},

	{ELE9_T, TOU_THRESH},
	{ELE9_R, REL_THRESH},

	{ELE10_T, TOU_THRESH},
	{ELE10_R, REL_THRESH},

	{ELE11_T, TOU_THRESH},
	{ELE11_R, REL_THRESH},

	{FIL_CFG, 0x01},
	{ELE_CFG, 0x0C},

	};

GI::Sensor::Mpr121::Mpr121(char *i2cPath, char *irqPath, unsigned char icNr)
{
	memset(this, 0, sizeof(*this));
	GI::Dev::DevRequest::request(i2cPath, &I2C);
	GI::Dev::DevRequest::request(i2cPath, &irqPin);
	if(!I2C)
		return;
	this->IcNr = icNr & 0x03;
	unsigned int cnt = 0;
	for (; cnt < sizeof(mpr121_set) / sizeof(mpr121_setup); cnt++)
	{
#ifdef __AVR_XMEGA__
		if(write(pgm_read_byte(&mpr121_set[cnt].addr) , pgm_read_byte(&mpr121_set[cnt].data)))
		{
			I2C = NULL;
			irqPin = NULL;
			return;
		}
#else
		if(write(mpr121_set[cnt].addr , mpr121_set[cnt].data))
		{
			I2C = NULL;
			irqPin = NULL;
			return;
		}
#endif
	}
}

GI::Sensor::Mpr121::~Mpr121()
{

}

SysErr GI::Sensor::Mpr121::readShort(unsigned char reg, unsigned short *return_data)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	if(I2C->writeRead(MPR121_ADDR | (IcNr & 0x03), &reg, 1, (unsigned char *)return_data, 2))
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpr121::read(unsigned char reg, unsigned char *return_data)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	if(I2C->writeRead(MPR121_ADDR | (IcNr & 0x03), &reg, 1, return_data, 1))
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpr121::write(unsigned char reg, unsigned char data)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	tmp[0] = reg;
	tmp[1] = data;
	if(I2C->writeBytes(MPR121_ADDR | (IcNr & 0x03), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}


SysErr GI::Sensor::Mpr121::idle(mpr121_ret_t *return_keys)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	/* Check if interrupt pin is provided, if not skip irq check. */
	if(irqPin)
	{
		if(irqPin->in())
			return SYS_ERR_NOTHING_CHANGED;
	}
	unsigned short keys;
	SysErr return_new_event = SYS_ERR_BUS;
	if(readShort(0, &keys))
		return return_new_event;
	unsigned short keys_back = keys;
	int cnt = 0;
	mpr121_ret_t return_keys_tmp;
	return_keys_tmp.pushed = 0;
	return_keys_tmp.released = 0;
	for(; cnt < 12; cnt++)
	{
		if((keys & 0x01) != (old_keys & 0x01))
		{
			if(keys & 0x01)
			{
				return_keys_tmp.pushed |= 1 << cnt;
				return_new_event = SYS_ERR_OK;
			}
			else
			{
				return_keys_tmp.released |= 1 << cnt;
				return_new_event = SYS_ERR_OK;
			}
		}
		keys = keys >> 1;
		old_keys = old_keys >> 1;
	}
	old_keys = keys_back;
	memcpy(return_keys, &return_keys_tmp, sizeof(mpr121_ret_t));
	return return_new_event;
}

