/*
 *  lib/device/lepton_flir.cpp
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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lepton_flir.h"
#include <api/timer.h>

const u16 ccitt_16Table[] = {
   0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
   0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
   0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
   0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
   0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
   0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
   0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
   0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
   0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
   0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
   0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
   0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
   0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
   0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
   0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
   0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
   0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
   0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
   0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
   0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
   0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
   0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
   0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
   0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
   0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
   0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
   0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
   0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
   0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
   0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
   0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
   0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

/*
 *  ===== ByteCRC16 =====
 *      Calculate (update) the CRC16 for a single 8-bit byte
 */
int ByteCRC16(int value, int crcin)
{
	return (u16)((crcin << 8) ^  ccitt_16Table[((crcin >> 8) ^ (value)) & 255]);
}

/*
 *  ===== CalcCRC16Words =====
 *      Calculate the CRC for a buffer of 16-bit words.  Supports both
 *  Little and Big Endian formats using conditional compilation.
 *      Note: minimum count is 1 (0 case not handled)
 */
u16 CalcCRC16Words(u32 count, s16 *buffer) {

    int crc = 0;

    do {

        int value = *buffer++;
#ifdef _BIG_ENDIAN
        crc = ByteCRC16(value >> 8, crc);
        crc = ByteCRC16(value, crc);
#else
        crc = ByteCRC16(value, crc);
        crc = ByteCRC16(value >> 8, crc);
#endif
    }
	while (--count);
    return (u16) crc;
}

/*
 *  ===== CalcCRC16Bytes =====
 *      Calculate the CRC for a buffer of 8-bit words.
 *      Note: minimum count is 1 (0 case not handled)
 */
u16 CalcCRC16Bytes(u32 count, s8 *buffer) {

    int crc = 0;

    do {

        int value = *buffer++;
        crc = ByteCRC16(value, crc);
    }
	while (--count);
    return crc;
}

GI::Sensor::LeptonFLIR::LeptonFLIR(s8 *spiPath, s8 *i2cPath) {
	memset(this, 0, sizeof(*this));
	SPI = new GI::IO((char *)spiPath);
	if(!SPI->devHandler || !SPI->ioDevType)
	{
		delete SPI;
		SPI = NULL;
	}
	I2C = new GI::IO((char *)i2cPath);
	if(!I2C->devHandler || !I2C->ioDevType)
	{
		delete I2C;
		I2C = NULL;
	}
}

GI::Sensor::LeptonFLIR::LeptonFLIR(s8 *spiPath, s8 *i2cPath, GI::Sensor::LeptonFLIR::medianRef_s medianRef)
{
	memset(this, 0, sizeof(*this));
	SPI = new GI::IO((char *)spiPath);
	if(!SPI->devHandler || !SPI->ioDevType)
	{
		delete SPI;
		SPI = NULL;
	}
	I2C = new GI::IO((char *)i2cPath);
	if(!I2C->devHandler || !I2C->ioDevType)
	{
		delete I2C;
		I2C = NULL;
	}
	this->medianRef = medianRef;
}


GI::Sensor::LeptonFLIR::~LeptonFLIR() {
	if(SPI)
		delete SPI;
	if(I2C)
		delete I2C;
}

SysErr GI::Sensor::LeptonFLIR::write(u8 *data, u32 len)
{
	u32 slaveAddr = LEPTON_FLIR_ADDR;
	I2C->ctl(GI::IO::IO_CTL_SET_SLAVE_ADDR, &slaveAddr);
	if(I2C->write(data, len) != (s32)len)
		return SYS_ERR_WRITE;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::LeptonFLIR::read(u8 *data, u32 len)
{
	u32 slaveAddr = LEPTON_FLIR_ADDR;
	I2C->ctl(GI::IO::IO_CTL_SET_SLAVE_ADDR, &slaveAddr);
	if(I2C->read(data, len) != (s32)len)
		return SYS_ERR_WRITE;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::LeptonFLIR::writeRead(u8 *dataSend, u32 lenSend, u8 *dataReceive, u32 lenReceive)
{
	u32 slaveAddr = LEPTON_FLIR_ADDR;
	I2C->ctl(GI::IO::IO_CTL_SET_SLAVE_ADDR, &slaveAddr);
	ioCtlRwMsgs_t msg;
	ioCtlMsg_t msgs[2];
	msg.nMsgs = 2;
	msg.msgs = msgs;

	msgs[0].buff = dataSend;
	msgs[0].len = lenSend;
	msgs[1].buff = dataReceive;
	msgs[1].len = lenReceive;
	return I2C->ctl(GI::IO::IO_CTL_WR, (u32 *)&msg);
}


bool GI::Sensor::LeptonFLIR::runCommand(u16 commandID) {
	if(!this || !I2C)
		return false;
    u16 statusReg;
    s16 statusCode;
    bool done;
    u16 timeoutCount = LEPTON_I2C_COMMAND_BUSY_WAIT_COUNT;

	//TwiStruct->MasterSlaveAddr = LEPTON_FLIR_ADDR;
	/*
	 * Implement the Lepton TWI WRITE Protocol
	 */
	/*
	 * First wait until the Camera is ready to receive a new
	 * command by polling the STATUS REGISTER BUSY Bit until it
	 * reports NOT BUSY.
	 */
	do
	{
		/*
		 * Read the Status REGISTER and peek at the BUSY Bit
		 */
		u8 tmp[2];
		u8 result[2];
		tmp[0] = LEP_I2C_STATUS_REG >> 8;
		tmp[1] = (u8)LEP_I2C_STATUS_REG;
		if(writeRead(tmp, 2, result, 2) != SYS_ERR_OK)
			return false;
		statusReg = (result[0] << 8) | result[1];
		done = (statusReg & LEP_I2C_STATUS_BUSY_BIT_MASK)? false : true;
		/*
		 * Add timout check
		 */
		if( timeoutCount-- == 0 )
		{
		/*
		 * Timed out waiting for command busy to go away
		 */
		return false;
		}
		GI::Sys::Timer::delay(2);
	}while( !done );
	/*
	 * Set the Lepton's DATA LENGTH REGISTER first to inform the
	 * Lepton Camera no 16-bit DATA words being transferred.
	 */
	u8 tmp[4];
	tmp[0] = LEP_I2C_DATA_LENGTH_REG >> 8;
	tmp[1] = (u8)LEP_I2C_DATA_LENGTH_REG;
	if(!I2C->write(tmp, 2))
		return false;

	/*
	 * Now issue the Run Command
	 */
	tmp[0] = LEP_I2C_COMMAND_REG >> 8;
	tmp[1] = (u8)LEP_I2C_COMMAND_REG;
	tmp[2] = commandID >> 8;
	tmp[3] = (u8)commandID;
	if(write(tmp, 4))
		return false;
	/*
	 * Now wait until the Camera has completed this command by
	 * polling the statusReg REGISTER BUSY Bit until it reports NOT
	 * BUSY.
	 */
	tmp[0] = LEP_I2C_STATUS_REG >> 8;
	tmp[1] = (u8)LEP_I2C_STATUS_REG;
	u8 result[2];
	do
	{
		/*
		 * Read the Status REGISTER and peek at the BUSY Bit
		 */
		if(writeRead(tmp, 2, result, 2) != SYS_ERR_OK)
			return false;
		statusReg = (result[0] << 8) | result[1];
		done = (statusReg & LEP_I2C_STATUS_BUSY_BIT_MASK)? false : true;
		/*
		 * Add timout check
		 */
		if( timeoutCount-- == 0 )
		{
		/*
		 * Timed out waiting for command busy to go away
		 */
		return false;
		}
		GI::Sys::Timer::delay(2);
	}while( !done );
    statusCode = (statusReg >> 8) ? ((statusReg >> 8) | 0xFF00) : 0;
    if(statusCode)
    {
      return false;
    }
    return true;
}


bool GI::Sensor::LeptonFLIR::regWrite(u16 commandID, u16 *attributePtr, u16 attributeWordLength) {
	if(!this || !I2C)
		return false;
    u16 statusReg;
    s16 statusCode;
    bool done;
    u16 timeoutCount = LEPTON_I2C_COMMAND_BUSY_WAIT_COUNT;

	u8 tmp[4];
	u8 result[2];
	/*
	 * Implement the Lepton TWI WRITE Protocol
	 */
	/*
	 * First wait until the Camera is ready to receive a new
	 * command by polling the STATUS REGISTER BUSY Bit until it
	 * reports NOT BUSY.
	 */
	tmp[0] = LEP_I2C_STATUS_REG >> 8;
	tmp[1] = (u8)LEP_I2C_STATUS_REG;
	do
	{
		/*
		 * Read the Status REGISTER and peek at the BUSY Bit
		 */
		if(writeRead(tmp, 2, result, 2) != SYS_ERR_OK)
			return false;
		statusReg = (result[0] << 8) | result[1];
		done = (statusReg & LEP_I2C_STATUS_BUSY_BIT_MASK)? false : true;
		/*
		 * Add timout check
		 */
		if( timeoutCount-- == 0 )
		{
		/*
		 * Timed out waiting for command busy to go away
		 */
		return false;
		}
		GI::Sys::Timer::delay(2);
	}while( !done );

	/*
	 * Now WRITE the DATA to the DATA REGISTER(s)
	 */
	u8 *buff = (u8 *)malloc((attributeWordLength * 2) + 2);
	memcpy(buff + 2, attributePtr, attributeWordLength * 2);
	if( attributeWordLength <= 16 )
	{
		/*
		 * WRITE to the DATA Registers - always start from DATA 0
		 */
		buff[0] = LEP_I2C_DATA_0_REG >> 8;
		buff[1] = (u8)LEP_I2C_DATA_0_REG;
		if(write(buff, (attributeWordLength * 2) + 2))
		{
			free(buff);
			return false;
		}
	}
	else if( attributeWordLength <= 1024 )
	{
		/*
		 * WRITE to the DATA Block Buffer
		 */
		tmp[0] = LEP_I2C_DATA_BUFFER_0 >> 8;
		tmp[1] = (u8)LEP_I2C_DATA_BUFFER_0;
		if(write(buff, (attributeWordLength * 2) + 2))
		{
			free(buff);
			return false;
		}
	}
	else {
		free(buff);
		return false;
	}
	free(buff);
    /*
     * Set the Lepton's DATA LENGTH REGISTER first to inform the
     * Lepton Camera how many 16-bit DATA words we want to read.
     */
	buff[0] = LEP_I2C_DATA_LENGTH_REG >> 8;
	buff[1] = (u8)LEP_I2C_DATA_LENGTH_REG;
	buff[2] = attributeWordLength >> 8;
	buff[3] = (u8)attributeWordLength;
	if(write(buff, 4))
		return false;
	/*
	 * Now issue the SET Attribute Command
	 */
	buff[0] = LEP_I2C_COMMAND_REG >> 8;
	buff[1] = (u8)LEP_I2C_COMMAND_REG;
	buff[2] = commandID >> 8;
	buff[3] = (u8)commandID;
	if(write(buff, 4))
		return false;
	/*
	 * Now wait until the Camera has completed this command by
	 * polling the statusReg REGISTER BUSY Bit until it reports NOT
	 * BUSY.
	 */
	buff[0] = LEP_I2C_STATUS_REG >> 8;
	buff[1] = (u8)LEP_I2C_STATUS_REG;
	do
	{
		/*
		 * Read the Status REGISTER and peek at the BUSY Bit
		 */
		if(writeRead(tmp, 2, result, 2) != SYS_ERR_OK)
			return false;
		statusReg = (result[0] << 8) | result[1];
		done = (statusReg & LEP_I2C_STATUS_BUSY_BIT_MASK)? false : true;
		/*
		 * Add timout check
		 */
		if( timeoutCount-- == 0 )
		{
		/*
		 * Timed out waiting for command busy to go away
		 */
		return false;
		}
		GI::Sys::Timer::delay(2);
	}while( !done );
	/*
	 * Check statusReg word for Errors?
	 */
	statusCode = (statusReg >> 8) ? ((statusReg >> 8) | 0xFF00) : 0;
	if(statusCode)
		return false;

	return true;

}

bool GI::Sensor::LeptonFLIR::regRead(u16 commandID, u16 *attributePtr, u16 attributeWordLength) {
	if(!this || !I2C)
		return false;
    u16 statusReg;
    s16 statusCode;
    bool done;
    u16 crcExpected, crcActual;
    u16 timeoutCount = LEPTON_I2C_COMMAND_BUSY_WAIT_COUNT;

	//TwiStruct->MasterSlaveAddr = LEPTON_FLIR_ADDR;


	u8 tmp[4];
	u8 result[2];
	/*
	 * Implement the Lepton TWI WRITE Protocol
	 */
	/*
	 * First wait until the Camera is ready to receive a new
	 * command by polling the STATUS REGISTER BUSY Bit until it
	 * reports NOT BUSY.
	 */
	do
	{
		/*
		 * Read the Status REGISTER and peek at the BUSY Bit
		 */
		tmp[0] = LEP_I2C_STATUS_REG >> 8;
		tmp[1] = (u8)LEP_I2C_STATUS_REG;
		if(writeRead(tmp, 2, result, 2) != SYS_ERR_OK)
			return false;
		statusReg = (result[0] << 8) | result[1];
		done = (statusReg & LEP_I2C_STATUS_BUSY_BIT_MASK)? false : true;
		/*
		 * Add timout check
		 */
		if( timeoutCount-- == 0 )
		{
		/*
		 * Timed out waiting for command busy to go away
		 */
		return false;
		}
		GI::Sys::Timer::delay(2);
	}while( !done );
    /*
     * Set the Lepton's DATA LENGTH REGISTER first to inform the
     * Lepton Camera how many 16-bit DATA words we want to read.
     */
	tmp[0] = LEP_I2C_DATA_LENGTH_REG >> 8;
	tmp[1] = (u8)LEP_I2C_DATA_LENGTH_REG;
	tmp[2] = attributeWordLength >> 8;
	tmp[3] = attributeWordLength;
	if(write(tmp, 3))
		return false;
    /*
     * Now issue the GET Attribute Command
     */
	tmp[0] = LEP_I2C_COMMAND_REG >> 8;
	tmp[1] = (u8)LEP_I2C_COMMAND_REG;
	tmp[2] = commandID >> 8;
	tmp[3] = commandID;
	if(write(tmp, 4))
		return false;
	/*
	 * Now wait until the Camera has completed this command by
	 * polling the statusReg REGISTER BUSY Bit until it reports NOT
	 * BUSY.
	 */
	tmp[0] = LEP_I2C_STATUS_REG >> 8;
	tmp[1] = (u8)LEP_I2C_STATUS_REG;
	do
	{
		/*
		 * Read the Status REGISTER and peek at the BUSY Bit
		 */
		if(writeRead(tmp, 2, result, 2) != SYS_ERR_OK)
			return false;
		statusReg = (result[0] << 8) | result[1];
		done = (statusReg & LEP_I2C_STATUS_BUSY_BIT_MASK)? false : true;
		/*
		 * Add timout check
		 */
		if( timeoutCount-- == 0 )
		{
		/*
		 * Timed out waiting for command busy to go away
		 */
		return false;
		}
		GI::Sys::Timer::delay(2);
	}while( !done );
	/*
	 * Check statusReg word for Errors?
	 */
	statusCode = (statusReg >> 8) ? ((statusReg >> 8) | 0xFF00) : 0;
	if(statusCode)
	{
		return false;
	}
    /*
     * If NO Errors then READ the DATA from the DATA REGISTER(s)
    */
    if( attributeWordLength <= 16 )
    {
    	/*
    	 * Read from the DATA Registers - always start from DATA 0
    	 * Little Endean
    	 */
    	tmp[0] = LEP_I2C_DATA_0_REG >> 8;
    	tmp[1] = (u8)LEP_I2C_DATA_0_REG;
		if(writeRead(tmp, 2, (u8 *)attributePtr, attributeWordLength * 2) != SYS_ERR_OK)
			return false;
    }
    else if( attributeWordLength <= 1024 )
    {
    	/*
    	 * Read from the DATA Block Buffer
    	 */
    	tmp[0] = LEP_I2C_DATA_BUFFER_0 >> 8;
    	tmp[1] = (u8)LEP_I2C_DATA_BUFFER_0;
    	if(writeRead(tmp, 2, (u8 *)attributePtr, attributeWordLength * 2) != SYS_ERR_OK)
			return false;
    }
	//memcpy(attributePtr, (void *)TwiStruct->RxBuff, attributeWordLength * 2);
    if(attributeWordLength > 0)
    {
    	/*
    	 * Check CRC
    	 */
    	tmp[0] = LEP_I2C_DATA_CRC_REG >> 8;
    	tmp[1] = (u8)LEP_I2C_DATA_CRC_REG;
		if(writeRead(tmp, 2, result, 2) != SYS_ERR_OK)
			return false;
		crcExpected = (result[0] << 8) | result[1];
		crcActual = (u16)CalcCRC16Words(attributeWordLength, (s16*)attributePtr);

		/*
		 * Check for 0 in the register in case the camera does not support CRC check
         */
		if(crcExpected != 0 && crcExpected != crcActual)
		{
			return false;
		}
    }
    return true;
}

bool GI::Sensor::LeptonFLIR::directWriteBuff(u16 *attributePtr, u16 attributeWordLength) {
	if(!this || !I2C)
		return false;
	/*
	 * Read from the DATA Block Buffer
	 */
	u8 *tmp = (u8 *)malloc((attributeWordLength * 2) + 2);
	tmp[0] = LEP_I2C_DATA_BUFFER_0 >> 8;
	tmp[1] = (u8)LEP_I2C_DATA_BUFFER_0;
	memcpy(tmp + 2, attributePtr, attributeWordLength * 2);
	if(write(tmp, (attributeWordLength * 2) + 2))
		return false;
	return true;
}

bool GI::Sensor::LeptonFLIR::directWriteReg(u16 regAddress, u16 regValue) {
	if(!this || !I2C)
		return false;
	/*
	 * Read from the DATA Block Buffer
	 */
	u8 tmp[4];
	tmp[0] = regAddress >> 8;
	tmp[1] = (u8)regAddress;
	tmp[2] = regValue >> 8;
	tmp[3] = (u8)regValue;
	if(write(tmp, 4))
		return false;
	return true;
}

bool GI::Sensor::LeptonFLIR::getImage(u16 *image) {
	if(!this || !SPI || !image)
		return false;
	//GI::Sys::Timer::delay(300);
	u8 line_buff[(LEPTON_FLIR_LINE_SIZE * 2) + 4];
	u8 checkByte = 0x0F;
	u8 packet_nr = 0;
	while((checkByte & 0x0F) == 0x0F) {
		memset(line_buff, 0xFF, (LEPTON_FLIR_LINE_SIZE * 2) + 4);
		SPI->read(line_buff, (LEPTON_FLIR_LINE_SIZE * 2) + 4);
		checkByte = line_buff[0];
		packet_nr = line_buff[1];
	}

	// sync done, first packet is ready, store packets
	while(packet_nr < 60)
	{
		// ignore discard packets
		if((line_buff[0] & 0x0F) != 0x0F) {
			u32 x_cnt = 0;
			for(; x_cnt < LEPTON_FLIR_LINE_SIZE; x_cnt++)
			{
				u8 tmp_x_cnt = x_cnt << 1;
				u16 tmp = line_buff[4 + tmp_x_cnt];
				tmp += line_buff[4 + tmp_x_cnt + 1] << 8;
				image[(packet_nr * LEPTON_FLIR_LINE_SIZE) + x_cnt] = tmp;
			}
			//memcpy(&image[packet_nr * LEPTON_FLIR_LINE_SIZE], line_buff + 4, LEPTON_FLIR_LINE_SIZE * 2);
		}
		// read next packet
		memset(line_buff, 0xFF, (LEPTON_FLIR_LINE_SIZE * 2) + 4);
		SPI->read(line_buff, (LEPTON_FLIR_LINE_SIZE * 2) + 4);
		checkByte = line_buff[0];
		packet_nr = line_buff[1];
	}

	return true;
}

bool GI::Sensor::LeptonFLIR::getImageBW(Color::ARGB *image, u8 alpha)
{
	if(!this || !SPI || !image)
		return false;
	//GI::Sys::Timer::delay(300);
	u8 line_buff[(LEPTON_FLIR_LINE_SIZE * 2) + 4];
	u8 checkByte = 0x0F;
	u8 packet_nr = 0;
	s32 med = 0;
	if(medianRef == BOTTOM_HEAT)
		med = 65536;
	while((checkByte & 0x0F) == 0x0F) {
		memset(line_buff, 0xFF, (LEPTON_FLIR_LINE_SIZE * 2) + 4);
		SPI->read(line_buff, (LEPTON_FLIR_LINE_SIZE * 2) + 4);
		checkByte = line_buff[0];
		packet_nr = line_buff[1];
	}

	// sync done, first packet is ready, store packets
	while(packet_nr < 60)
	{
		// ignore discard packets
		if((line_buff[0] & 0x0F) != 0x0F) {
			u32 x_cnt = 0;
			for(; x_cnt < LEPTON_FLIR_LINE_SIZE; x_cnt++)
			{
				u8 tmp_x_cnt = 4 + (x_cnt << 1);
				s16 tmp = line_buff[tmp_x_cnt] << 8;
				tmp += line_buff[tmp_x_cnt + 1];
				switch((u8)medianRef)
				{
				case MIDLE_HEAT:
					med += tmp;
					tmp = tmp - median;
					tmp = tmp + 128;
					break;
				case BOTTOM_HEAT:
                    if (med > tmp)
                    {
                        med = tmp;
                    }
                    tmp = tmp - median;
					break;
				case TOP_HEAT:
                    if (med < tmp)
                    {
                        med = tmp;
                    }
                    tmp = 255 - (median - tmp);
					break;
				}
    			if (tmp > 255)
    			{
    				tmp = 255;
    			}
    			if (tmp < 0)
    			{
    				tmp = 0;
    			}
				u8 bw = tmp;
				u32 target_pixel = (packet_nr * LEPTON_FLIR_LINE_SIZE) + (LEPTON_FLIR_LINE_SIZE - x_cnt - 1);
				image[target_pixel] = Color::fromArgb(bw, bw, bw, alpha);
			}
		}
		// read next packet
		memset(line_buff, 0xFF, (LEPTON_FLIR_LINE_SIZE * 2) + 4);
		SPI->read(line_buff, (LEPTON_FLIR_LINE_SIZE * 2) + 4);
		checkByte = line_buff[0];
		packet_nr = line_buff[1];
	}
	switch((u8)medianRef)
	{
	case MIDLE_HEAT:
		median = med / (LEPTON_FLIR_LINE_SIZE * LEPTON_FLIR_LINES_NR);
		break;
	default:
		median = med;
	}

	return true;
}

bool GI::Sensor::LeptonFLIR::getImageARGB(Color::ARGB *image, u8 alpha)
{
	if(!this || !SPI || !image)
		return false;
	//GI::Sys::Timer::delay(300);
	u8 line_buff[(LEPTON_FLIR_LINE_SIZE * 2) + 4];
	u8 checkByte = 0x0F;
	u8 packet_nr = 0;
	s32 med = 0;
	if(medianRef == BOTTOM_HEAT)
		med = 65536;
	while((checkByte & 0x0F) == 0x0F) {
		memset(line_buff, 0xFF, (LEPTON_FLIR_LINE_SIZE * 2) + 4);
		SPI->read(line_buff, (LEPTON_FLIR_LINE_SIZE * 2) + 4);
		checkByte = line_buff[0];
		packet_nr = line_buff[1];
	}

	// sync done, first packet is ready, store packets
	while(packet_nr < 60)
	{
		// ignore discard packets
		if((line_buff[0] & 0x0F) != 0x0F) {
			u32 x_cnt = 0;
			for(; x_cnt < LEPTON_FLIR_LINE_SIZE; x_cnt++)
			{
				u8 tmp_x_cnt = 4 + (x_cnt << 1);
				s16 tmp = line_buff[tmp_x_cnt] << 8;
				tmp += line_buff[tmp_x_cnt + 1];
				switch((u8)medianRef)
				{
				case MIDLE_HEAT:
					med += tmp;
					if(!customMedian)
						tmp = tmp - median;
					else
					{
						tmp = tmp - 4096;
						tmp /= 4;
					}
					tmp = tmp + 128;
					break;
				case BOTTOM_HEAT:
                    if (med > tmp)
                    {
                        med = tmp;
                    }
                    tmp = tmp - median;
					break;
				case TOP_HEAT:
                    if (med < tmp)
                    {
                        med = tmp;
                    }
                    tmp = 1280 - (median - tmp);
					break;
				}
				if (tmp > 1279)
				{
					tmp = 1279;
				}
				if (tmp < 0)
				{
					tmp = 0;
				}
				u32 target_pixel = (packet_nr * LEPTON_FLIR_LINE_SIZE) + (LEPTON_FLIR_LINE_SIZE - x_cnt - 1);
				if(tmp < 256) //black to blue
					image[target_pixel] = Color::fromArgb(0, 0, tmp & 0xFF, alpha);
				else if(tmp < 512) // blue to cyan (0,0,255)
					image[target_pixel] = Color::fromArgb(0, tmp & 0xFF, 255, alpha);
				else if(tmp < 768) // Cyan to green (0,255,255)
					image[target_pixel] = Color::fromArgb(0, 255, 255 - (tmp & 0xFF), alpha);
				else if(tmp < 1024) // Green to yellow (0,255,0)
					image[target_pixel] = Color::fromArgb(tmp & 0xFF, 255, 0, alpha);
				else // Yellow (255,255,0) to Red (255,0,0)
					image[target_pixel] = Color::fromArgb(255, 255 - (tmp & 0xFF), 0, alpha);
			}
		}
		// read next packet
		memset(line_buff, 0xFF, (LEPTON_FLIR_LINE_SIZE * 2) + 4);
		SPI->read(line_buff, (LEPTON_FLIR_LINE_SIZE * 2) + 4);
		checkByte = line_buff[0];
		packet_nr = line_buff[1];
	}
	switch((u8)medianRef)
	{
	case MIDLE_HEAT:
		median = med / (LEPTON_FLIR_LINE_SIZE * LEPTON_FLIR_LINES_NR);
		break;
	default:
		median = med;
	}
	return true;
}


