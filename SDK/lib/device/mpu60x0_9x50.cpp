/*
 *  device/mpu60x0_9x50.cpp
 */

#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <api/dev_request.h>
#include <device/mpu60x0_9x50.h>

GI::Sensor::Mpu60x0_9x50::Mpu60x0_9x50(char *i2cPath, unsigned char icNr)
{
	memset(this, 0, sizeof(*this));
	GI::Dev::DevRequest::request(i2cPath, &I2C);
	if(!I2C)
		return;
	this->IcNr = icNr & 0x01;
	if(GI::Sensor::Mpu60x0_9x50::whoAmI())
		return;
	if(setSleep(false))
		return;
	if(setClockSel(MPU60X0_9150_PWR_MGMT_1_CLKSEL_PLL_X_AX))
		return;
	if(setGyroFullScale(MPU60X0_9150_GYRO_CONFIG_FS_SEL_2000))
		return;
	if(setBypassEnable(true))
		return;
}

SysErr GI::Sensor::Mpu60x0_9x50::sampleRateDivSet(unsigned char Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	tmp[0] = MPU60X0_9150_SMPLRT_DIV;
	tmp[1] = Value;
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::whoAmI()
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char reg = MPU60X0_9150_WHO_AM_I;
	unsigned char result = 0;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), &reg, 1, &result, 1))
		return SYS_ERR_BUS;
	if((result & 0x02 >> 1) == (IcNr & 0x01))
	{
		if((result >> 2) != (MPU60x0_9150_ADDR >> 2))
			return SYS_ERR_DEVICE_NOT_MATCH;
	}
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setDlpfCfg(unsigned char value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	tmp[0] = MPU60X0_9150_CONFIG;
	unsigned char result = 0;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~MPU60X0_9150_CONFIG_DLPF_CFG_gm) | (value & MPU60X0_9150_CONFIG_DLPF_CFG_gm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setGyroSelfTest(bool X_test, bool Y_test, bool Z_test)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	tmp[0] = MPU60X0_9150_GYRO_CONFIG;
	unsigned char result = 0;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~MPU60X0_9150_GYRO_CONFIG_ST_gm) | (X_test << MPU60X0_9150_GYRO_CONFIG_XG_ST_bp) | (Y_test << MPU60X0_9150_GYRO_CONFIG_YG_ST_bp) | (Z_test << MPU60X0_9150_GYRO_CONFIG_ZG_ST_bp);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setGyroFullScale(unsigned int scale)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_GYRO_CONFIG;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	unsigned char _tmp = 0;
	switch(scale)
	{
	case MPU60X0_9150_GYRO_CONFIG_FS_SEL_250:
		_tmp = MPU60X0_9150_GYRO_CONFIG_FS_SEL_250 << MPU60X0_9150_GYRO_CONFIG_FS_SEL_gp;
		break;
	case MPU60X0_9150_GYRO_CONFIG_FS_SEL_500:
		_tmp = MPU60X0_9150_GYRO_CONFIG_FS_SEL_500 << MPU60X0_9150_GYRO_CONFIG_FS_SEL_gp;
		break;
	case MPU60X0_9150_GYRO_CONFIG_FS_SEL_1000:
		_tmp = MPU60X0_9150_GYRO_CONFIG_FS_SEL_1000 << MPU60X0_9150_GYRO_CONFIG_FS_SEL_gp;
		break;
	case MPU60X0_9150_GYRO_CONFIG_FS_SEL_2000:
		_tmp = MPU60X0_9150_GYRO_CONFIG_FS_SEL_2000 << MPU60X0_9150_GYRO_CONFIG_FS_SEL_gp;
		break;
	default:
		return SYS_ERR_INVALID_COMMAND;
	}
	tmp[1] = (result & ~MPU60X0_9150_GYRO_CONFIG_FS_SEL_gm) | _tmp;
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setAccelSelfTest(bool X_test, bool Y_test, bool Z_test)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_ACCEL_CONFIG;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~MPU60X0_9150_ACCEL_CONFIG_ST_gm) | (X_test << MPU60X0_9150_ACCEL_CONFIG_XA_ST_bp) | (Y_test << MPU60X0_9150_ACCEL_CONFIG_YA_ST_bp) | (Z_test << MPU60X0_9150_ACCEL_CONFIG_ZA_ST_bp);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setAccelFullScale(unsigned int scale)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_ACCEL_CONFIG;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	unsigned char _tmp = 0;
	switch(scale)
	{
	case 250:
		_tmp = MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_2 << MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gp;
		break;
	case 500:
		_tmp = MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_4 << MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gp;
		break;
	case 1000:
		_tmp = MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_8 << MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gp;
		break;
	case 2000:
		_tmp = MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_16 << MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gp;
		break;
	default:
		return SYS_ERR_INVALID_COMMAND;
	}
	tmp[1] = (result & ~MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gm) | _tmp;
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::getAccelData(signed short *X_Axis, signed short *Y_Axis, signed short *Z_Axis)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char reg = MPU60X0_9150_ACCEL_XOUT_H;
	unsigned char result[6];
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), &reg, 1, result, 6))
		return SYS_ERR_BUS;
	*X_Axis = ((result[0] << 8) + result[1]) + XA_Cal;
	*Y_Axis = ((result[2] << 8) + result[3]) + YA_Cal;
	*Z_Axis = ((result[4] << 8) + result[5]) + ZA_Cal;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::getGyroData(signed short *X_Axis, signed short *Y_Axis, signed short *Z_Axis)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char reg = MPU60X0_9150_GYRO_XOUT_H;
	unsigned char result[6];
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), &reg, 1, result, 6))
		return SYS_ERR_BUS;
	*X_Axis = ((result[0] << 8) + result[1]) + XG_Cal;
	*Y_Axis = ((result[2] << 8) + result[3]) + YG_Cal;
	*Z_Axis = ((result[4] << 8) + result[5]) + ZG_Cal;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::getTempData(float *Temperature)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char reg = MPU60X0_9150_TEMP_OUT_H;
	unsigned char result[2];
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), &reg, 1, result, 2))
		return SYS_ERR_BUS;
	*Temperature = (((float)(((signed short)(result[0] << 8) + result[1])))/340.0) + 36.53;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setClockSel(unsigned char Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_PWR_MGMT_1;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~MPU60X0_9150_PWR_MGMT_1_CLKSEL_gm) | (Value & MPU60X0_9150_PWR_MGMT_1_CLKSEL_gm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setTempDis(bool Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_PWR_MGMT_1;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~(MPU60X0_9150_PWR_MGMT_1_TEMP_DIS_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_1_TEMP_DIS_bp) & MPU60X0_9150_PWR_MGMT_1_TEMP_DIS_bm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setCycle(bool Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_PWR_MGMT_1;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~(MPU60X0_9150_PWR_MGMT_1_CYCLE_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_1_CYCLE_bp) & MPU60X0_9150_PWR_MGMT_1_CYCLE_bm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setSleep(bool Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_PWR_MGMT_1;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~(MPU60X0_9150_PWR_MGMT_1_SLEEP_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_1_SLEEP_bp) & MPU60X0_9150_PWR_MGMT_1_SLEEP_bm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setDeviceReset(bool Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_PWR_MGMT_1;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~(MPU60X0_9150_PWR_MGMT_1_DEVICE_RESET_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_1_DEVICE_RESET_bp) & MPU60X0_9150_PWR_MGMT_1_DEVICE_RESET_bm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setStbyZg(bool Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_PWR_MGMT_2;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~(MPU60X0_9150_PWR_MGMT_2_STBY_ZG_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_STBY_ZG_bp) & MPU60X0_9150_PWR_MGMT_2_STBY_ZG_bm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setStbyYg(bool Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_PWR_MGMT_2;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~(MPU60X0_9150_PWR_MGMT_2_STBY_YG_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_STBY_YG_bp) & MPU60X0_9150_PWR_MGMT_2_STBY_YG_bm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setStbyXg(bool Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_PWR_MGMT_2;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~(MPU60X0_9150_PWR_MGMT_2_STBY_XG_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_STBY_XG_bp) & MPU60X0_9150_PWR_MGMT_2_STBY_XG_bm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setStbyZa(bool Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_PWR_MGMT_2;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~(MPU60X0_9150_PWR_MGMT_2_STBY_ZA_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_STBY_ZA_bp) & MPU60X0_9150_PWR_MGMT_2_STBY_ZA_bm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setStbyYa(bool Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_PWR_MGMT_2;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~(MPU60X0_9150_PWR_MGMT_2_STBY_YA_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_STBY_YA_bp) & MPU60X0_9150_PWR_MGMT_2_STBY_YA_bm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setStbyXa(bool Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_PWR_MGMT_2;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~(MPU60X0_9150_PWR_MGMT_2_STBY_XA_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_STBY_XA_bp) & MPU60X0_9150_PWR_MGMT_2_STBY_XA_bm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setLpWakeCtrl(unsigned char Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_PWR_MGMT_2;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~(MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_gm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_gp) & MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_gm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::setBypassEnable(bool Value)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	unsigned char result = 0;
	tmp[0] = MPU60X0_9150_INT_PIN_CFG;
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 1, &result, 1))
		return SYS_ERR_BUS;
	tmp[1] = (result & ~(MPU60X0_9150_INT_PIN_CFG_I2C_BYPASS_EN_bm)) | ((Value << MPU60X0_9150_INT_PIN_CFG_I2C_BYPASS_EN_bp) & MPU60X0_9150_INT_PIN_CFG_I2C_BYPASS_EN_bm);
	if(I2C->writeBytes(MPU60x0_9150_ADDR | (IcNr & 0x01), tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::testGyro(double *XtestResult, double *YtestResult, double *ZtestResult)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	signed short Xdata = 0;
	signed short Ydata = 0;
	signed short Zdata = 0;
	signed short _Xdata = 0;
	signed short _Ydata = 0;
	signed short _Zdata = 0;
	SysErr err;
	if((err = getGyroData(&Xdata, &Ydata, &Zdata)))
		return err;
	if((err = setGyroSelfTest(true, true, true)))
		return err;
	GI::Sys::Timer::delay(100);
	if((err = getGyroData(&_Xdata, &_Ydata, &_Zdata)))
		return err;
	unsigned char reg = MPU60X0_9150_SELF_TEST_X;
	unsigned char result[3];
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), &reg, 1, result, 3))
		return SYS_ERR_BUS;
	double XG_TEST = (double)(result[0] & 0x1F);
	double YG_TEST = (double)(result[1] & 0x1F);
	double ZG_TEST = (double)(result[2] & 0x1F);
	if((err = setGyroSelfTest(false, false, false)))
		return err;
	*XtestResult = 0;
	*YtestResult = 0;
	*ZtestResult = 0;
	if(XG_TEST != 0.0) *XtestResult = 25.0 * 131.0 * pow(1.046, XG_TEST - 1.0);
	if(YG_TEST != 0.0) *YtestResult = -25.0 * 131.0 * pow(1.046, YG_TEST - 1.0);
	if(ZG_TEST != 0.0) *ZtestResult = 25.0 * 131.0 * pow(1.046, ZG_TEST - 1.0);
	*XtestResult = (((double)_Xdata - (double)Xdata) - *XtestResult) / *XtestResult;
	*YtestResult = (((double)_Ydata - (double)Ydata) - *YtestResult) / *YtestResult;
	*ZtestResult = (((double)_Zdata - (double)Zdata) - *ZtestResult) / *ZtestResult;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Mpu60x0_9x50::testAccel(double *XtestResult, double *YtestResult, double *ZtestResult)
{
	if(!I2C)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	signed short Xdata = 0;
	signed short Ydata = 0;
	signed short Zdata = 0;
	signed short _Xdata = 0;
	signed short _Ydata = 0;
	signed short _Zdata = 0;
	SysErr err;
	if((err = getAccelData(&Xdata, &Ydata, &Zdata)))
		return err;
	if((err = setAccelSelfTest(true, true, true)))
		return err;
	GI::Sys::Timer::delay(100);
	if((err = getAccelData(&_Xdata, &_Ydata, &_Zdata)))
		return err;
	unsigned char reg = MPU60X0_9150_SELF_TEST_X;
	unsigned char result[3];
	if(I2C->writeRead(MPU60x0_9150_ADDR | (IcNr & 0x01), &reg, 1, result, 4))
		return SYS_ERR_BUS;
	double XA_TEST = (double)(((result[0] >> 4) & 0x0C) | ((result[3] >> 4) & 0x03));
	double YA_TEST = (double)(((result[1] >> 4) & 0x0C) | ((result[3] >> 2) & 0x03));
	double ZA_TEST = (double)(((result[2] >> 4) & 0x0C) | ((result[3] >> 0) & 0x03));

	if((err = setAccelSelfTest(false, false, false)))
		return err;
	*XtestResult = 0;
	*YtestResult = 0;
	*ZtestResult = 0;
	if(XA_TEST != 0.0) *XtestResult = 4096.0 * 0.34 * (pow(0.92, (XA_TEST - 1.0) / ((1<<5) - 2)) / 0.34);
	if(YA_TEST != 0.0) *YtestResult = 4096.0 * 0.34 * (pow(0.92, (YA_TEST - 1.0) / ((1<<5) - 2)) / 0.34);
	if(ZA_TEST != 0.0) *ZtestResult = 4096.0 * 0.34 * (pow(0.92, (ZA_TEST - 1.0) / ((1<<5) - 2)) / 0.34);
	*XtestResult = (((double)_Xdata - (double)Xdata) - *XtestResult) / *XtestResult;
	*YtestResult = (((double)_Ydata - (double)Ydata) - *YtestResult) / *YtestResult;
	*ZtestResult = (((double)_Zdata - (double)Zdata) - *ZtestResult) / *ZtestResult;
	return SYS_ERR_OK;
}
/*
bool mpu60x0_9150_gyroscope_test_display_result()
{
	if(!I2C)
		return false;
	double XtestResult = 0;
	double YtestResult = 0;
	double ZtestResult = 0;
	if(!testGyro(&XtestResult, &YtestResult, &ZtestResult))
		return false;
	double XtestResultInt = 0;
	double XtestResultDec = modf(((double)XtestResult), &XtestResultInt);
	double YtestResultInt = 0;
	double YtestResultDec = modf(((double)YtestResult), &YtestResultInt);
	double ZtestResultInt = 0;
	double ZtestResultDec = modf(((double)ZtestResult), &ZtestResultInt);
	uart.printf(DebugCom, "MPU60x0: Giroscope calibration value: Xg = %d.%u, Yg = %d.%u, Zg = %d.%u\n\r", (signed int)XtestResultInt, (unsigned int)(XtestResultDec * 10000), (signed int)YtestResultInt, (unsigned int)(YtestResultDec * 10000), (signed int)YtestResultInt, (unsigned int)(ZtestResultDec * 10000));
	return true;
}

bool mpu60x0_9150_accelerometer_test_display_result()
{
	if(!I2C)
		return false;
	double XtestResult = 0;
	double YtestResult = 0;
	double ZtestResult = 0;
	if(!testAccel(&XtestResult, &YtestResult, &ZtestResult))
		return false;
	double XtestResultInt = 0;
	double XtestResultDec = modf(((double)XtestResult), &XtestResultInt);
	double YtestResultInt = 0;
	double YtestResultDec = modf(((double)YtestResult), &YtestResultInt);
	double ZtestResultInt = 0;
	double ZtestResultDec = modf(((double)ZtestResult), &ZtestResultInt);
	uart.printf(DebugCom, "MPU60x0: Accelerometer calibration value: Xa = %d.%u, Ya = %d.%u, Za = %d.%u\n\r", (signed int)XtestResultInt, (unsigned int)(XtestResultDec * 10000), (signed int)YtestResultInt, (unsigned int)(YtestResultDec * 10000), (signed int)ZtestResultInt, (unsigned int)(ZtestResultDec * 10000));
	return true;
}

bool mpu60x0_9150_temperature_display_result()
{
	if(!I2C)
		return false;
	float Temperature = 0;
	if(!getTempData(&Temperature))
		return false;
#ifndef _TINY_PRINT_
	uart.printf(DebugCom, "MPU60x0: Temperature: %2.2f Gr Celsius\n\r", Temperature);
#else
	float GrCelsius = 0;
	float GrCelsiusMod = modff(Temperature, &GrCelsius);
	uart.printf(DebugCom, "MPU60x0: Temperature: %d.%u Gr Celsius\n\r", (unsigned int)GrCelsius, (unsigned int)(GrCelsiusMod*1000));
#endif
	return true;
}

bool mpu60x0_9150_giroscope_display_result()
{
	if(!I2C)
		return false;
	signed short Xg = 0;
	signed short Yg = 0;
	signed short Zg = 0;
	if(!getGyroData(&Xg, &Yg, &Zg))
		return false;
	uart.printf(DebugCom, "MPU60x0: Giroscope: Xg = %d, Yg = %d, Zg = %d\n\r", Xg, Yg, Zg);
	return true;
}

bool mpu60x0_9150_accelerometer_display_result()
{
	if(!I2C)
		return false;
	signed short Xa = 0;
	signed short Ya = 0;
	signed short Za = 0;
	if(!getAccelData(&Xa, &Ya, &Za))
		return false;
	uart.printf(DebugCom, "MPU60x0: Accelerometer: Xa = %d, Ya = %d, Za = %d\n\r", Xa, Ya, Za);
	return true;
}
*/
