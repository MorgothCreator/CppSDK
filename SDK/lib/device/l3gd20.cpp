/*
 *  device/l3gd20.cpp
 */

#include <stdlib.h>
#include <string.h>
#include "l3gd20.h"

/* Read/Write command */
#define READWRITE_CMD              ((unsigned char)0x80)
/* Multiple byte read/write command */
#define MULTIPLEBYTE_CMD           ((unsigned char)0x40)

GI::Sensor::L3gd20::L3gd20(string spiPath)
{
	memset(this, 0, sizeof(*this));
	SPI = new GI::IO(spiPath);
	unsigned char tmp;

	/* Read WHO I AM register */
	readRegs(&tmp, L3GD20_WHO_AM_I_ADDR, 1);

	unsigned char init_ctl1 = L3GD20_MODE_ACTIVE | L3GD20_OUTPUT_DATARATE_1
			| L3GD20_AXES_ENABLE | L3GD20_BANDWIDTH_4;
	unsigned char init_ctl4 = L3GD20_BlockDataUpdate_Continous | L3GD20_BLE_LSB
			| L3GD20_FULLSCALE_500;
	writeRegs(&init_ctl1, L3GD20_CTRL_REG1_ADDR, 1);
	writeRegs(&init_ctl4, L3GD20_CTRL_REG4_ADDR, 1);
	filterCfg(L3GD20_HPM_NORMAL_MODE_RES | L3GD20_HPFCF_0);
	filterEn(true);
}

GI::Sensor::L3gd20::~L3gd20()
{

}

SysErr GI::Sensor::L3gd20::writeRegs(unsigned char* pBuffer,
		unsigned char WriteAddr, unsigned short NumByteToWrite)
{
	unsigned char *tmp_buff = (unsigned char *) malloc(NumByteToWrite + 1);
	if (!tmp_buff)
		return SYS_ERR_OUT_OF_MEMORY;
	tmp_buff[0] = WriteAddr;
	memcpy(tmp_buff + 1, pBuffer, NumByteToWrite);
	if (SPI->write(tmp_buff, NumByteToWrite + 1) != (int)NumByteToWrite + 1)
		return SYS_ERR_UNKNOWN;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::L3gd20::readRegs(unsigned char* pBuffer,
		unsigned char ReadAddr, unsigned short NumByteToRead)
{
	if (NumByteToRead > 0x01)
	{
		ReadAddr |= (unsigned char) (READWRITE_CMD | MULTIPLEBYTE_CMD);
	}
	else
	{
		ReadAddr |= (unsigned char) READWRITE_CMD;
	}
	ioCtlRwMsgs_t msg;
	ioCtlMsg_t msgs[2];
	msg.nMsgs = 2;
	msg.msgs = msgs;

	msgs[0].buff = &ReadAddr;
	msgs[0].len = 1;
	msgs[1].buff = pBuffer;
	msgs[1].len = NumByteToRead;
	if (SPI->ctl(GI::IO::IO_CTL_WR, (u32 *) &msg) != SYS_ERR_OK)
		return SYS_ERR_INVALID_HANDLER;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::L3gd20::filterCfg(unsigned char cfg)
{
	if (!this || !SPI)
		return SYS_ERR_INVALID_HANDLER;
	unsigned char tmpreg;
	readRegs(&tmpreg, L3GD20_CTRL_REG2_ADDR, 1);
	/* Configure MEMS: mode and cutoff frequency */
	tmpreg |= cfg;
	writeRegs(&tmpreg, L3GD20_CTRL_REG2_ADDR, 1);
	return SYS_ERR_OK;
}

SysErr GI::Sensor::L3gd20::filterEn(bool state)
{
	if (!this || !SPI)
		return SYS_ERR_INVALID_HANDLER;
	unsigned char tmpreg;
	readRegs(&tmpreg, L3GD20_CTRL_REG5_ADDR, 1);
	/* Configure MEMS: mode and cutoff frequency */
	tmpreg &= 0xEF;
	if (state)
		tmpreg |= L3GD20_HIGHPASSFILTER_ENABLE;
	writeRegs(&tmpreg, L3GD20_CTRL_REG5_ADDR, 1);
	return SYS_ERR_OK;
}

/**
 * @brief  Calculate the L3GD20 angular data.
 * @param  pfData: Data out pointer
 * @retval None
 */
SysErr GI::Sensor::L3gd20::read(float *X, float *Y, float *Z)
{
	if (!this || !SPI)
		return SYS_ERR_INVALID_HANDLER;
	unsigned char tmpbuffer[6] = { 0 };
	signed short RawData[3] = { 0 };
	unsigned char tmpreg = 0;
	float sensitivity = 0;
	int i = 0;

	readRegs(&tmpreg, L3GD20_CTRL_REG4_ADDR, 1);

	readRegs(tmpbuffer, L3GD20_OUT_X_L_ADDR, 6);

	/* check in the control register 4 the data alignment (Big Endian or Little Endian)*/
	if (!(tmpreg & L3GD20_BLE_MSB))
	{
		for (i = 0; i < 3; i++)
		{
			RawData[i] = (signed short) (((unsigned short) tmpbuffer[(2 * i) + 1]
					<< 8) + tmpbuffer[2 * i]);
		}
	}
	else
	{
		for (i = 0; i < 3; i++)
		{
			RawData[i] =
					(signed short) (((unsigned short) tmpbuffer[2 * i] << 8)
							+ tmpbuffer[(2 * i) + 1]);
		}
	}

	/* Switch the sensitivity value set in the CRTL4 */
	switch (tmpreg & L3GD20_FULLSCALE_SELECTION)
	{
	case L3GD20_FULLSCALE_250:
		sensitivity = L3GD20_SENSITIVITY_250DPS;
		break;

	case L3GD20_FULLSCALE_500:
		sensitivity = L3GD20_SENSITIVITY_500DPS;
		break;

	case L3GD20_FULLSCALE_2000:
		sensitivity = L3GD20_SENSITIVITY_2000DPS;
		break;
	}
	*X = (float) (RawData[0] * sensitivity);
	*Y = (float) (RawData[1] * sensitivity);
	*Z = (float) (RawData[2] * sensitivity);
	return SYS_ERR_OK;
}

