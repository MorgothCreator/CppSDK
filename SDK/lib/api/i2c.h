/*
 *  api/i2c.h
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

#ifndef TWI_DEF_H_
#define TWI_DEF_H_
#include <stdlib.h>
#include <api/gpio.h>
#include <api/i2c.h>
#include <include/global.h>
/*#####################################################*/
#define twi_timeout	500000
/*#####################################################*/
typedef struct
{
	char name[12];
	unsigned int scl;
	unsigned int sda;
	unsigned long speed;
	enum i2cMode{
		i2cMode0,
		i2cMode1,
		i2cMode2,
		i2cMode3,
	}i2cMode;

} CfgI2c;
/*#####################################################*/
/*#####################################################*/
namespace GI
{
namespace Dev
{
class I2c
{
public:
	I2c(const char *path);
	~I2c();
	SysErr writeRead(unsigned char addr, unsigned char *buffSend,
			unsigned int lenSend, unsigned char *buffReceive,
			unsigned int lenReceive);
	int readBytes(unsigned char addr, unsigned char *buff, unsigned int len);
	int writeBytes(unsigned char addr, unsigned char *buff, unsigned int len);

	unsigned char unitNr;
	volatile unsigned char flag;
	volatile unsigned char error_flag;
	volatile unsigned char result;
	volatile unsigned char status;
	unsigned int tCount;
	unsigned int rCount;
	unsigned int numOfBytes;
	unsigned int bytesToWrite;
	unsigned int bytesToRead;
	unsigned long BusyTimeOut;
	bool noSendWriteOnRead;
	CfgI2c cfg;
	void *udata;
	SysErr err;
private:
	SysErr WR(unsigned char addr, unsigned char *buff_send,
			unsigned int TransmitBytes, unsigned char *buff_receive,
			unsigned int ReceiveBytes);
};
}
}
/*#####################################################*/
#endif /* TWI_DEF_H_ */
/*#####################################################*/
