/*
 *  lib/device/mpu60x0.h
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
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
/*#####################################################*/
#ifndef MPU60X0_9150_9150_H_
#define MPU60X0_9150_9150_H_
/*#####################################################*/
#include "api/i2c.h"
#include "api/timer.h"
#include <include/global.h>
/*#####################################################*/
#define MPU60x0_9150_ADDR				(0x68)

/* Register table */

#define MPU60X0_9150_SELF_TEST_X			(13)
#define MPU60X0_9150_SELF_TEST_Y			(14)
#define MPU60X0_9150_SELF_TEST_Z			(15)
#define MPU60X0_9150_SELF_TEST_A			(16)
#define MPU60X0_9150_SMPLRT_DIV			(25)
#define MPU60X0_9150_CONFIG				(26)
#define MPU60X0_9150_GYRO_CONFIG			(27)
#define MPU60X0_9150_ACCEL_CONFIG		(28)
#define MPU60X0_9150_MOT_THR				(31)
#define MPU60X0_9150_FIFO_EN				(35)
#define MPU60X0_9150_I2C_MST_CTRL		(36)
#define MPU60X0_9150_I2C_SLV0_ADDR		(37)
#define MPU60X0_9150_I2C_SLV0_REG		(38)
#define MPU60X0_9150_I2C_SLV0_CTRL		(39)
#define MPU60X0_9150_I2C_SLV1_ADDR		(40)
#define MPU60X0_9150_I2C_SLV1_REG		(41)
#define MPU60X0_9150_I2C_SLV1_CTRL		(42)
#define MPU60X0_9150_I2C_SLV2_ADDR		(43)
#define MPU60X0_9150_I2C_SLV2_REG		(44)
#define MPU60X0_9150_I2C_SLV2_CTRL		(45)
#define MPU60X0_9150_I2C_SLV3_ADDR		(46)
#define MPU60X0_9150_I2C_SLV3_REG		(47)
#define MPU60X0_9150_I2C_SLV3_CTRL		(48)
#define MPU60X0_9150_I2C_SLV4_ADDR		(49)
#define MPU60X0_9150_I2C_SLV4_REG		(50)
#define MPU60X0_9150_I2C_SLV4_DO			(51)
#define MPU60X0_9150_I2C_SLV4_CTRL		(52)
#define MPU60X0_9150_I2C_SLV4_DI			(53)
#define MPU60X0_9150_I2C_MST_STATUS		(54)
#define MPU60X0_9150_INT_PIN_CFG			(55)
#define MPU60X0_9150_INT_ENABLE			(56)
#define MPU60X0_9150_INT_STATUS			(58)
#define MPU60X0_9150_ACCEL_XOUT_H		(59)
#define MPU60X0_9150_ACCEL_XOUT_L		(60)
#define MPU60X0_9150_ACCEL_YOUT_H		(61)
#define MPU60X0_9150_ACCEL_YOUT_L		(62)
#define MPU60X0_9150_ACCEL_ZOUT_H		(63)
#define MPU60X0_9150_ACCEL_ZOUT_L		(64)
#define MPU60X0_9150_TEMP_OUT_H			(65)
#define MPU60X0_9150_TEMP_OUT_L			(66)
#define MPU60X0_9150_GYRO_XOUT_H			(67)
#define MPU60X0_9150_GYRO_XOUT_L			(68)
#define MPU60X0_9150_GYRO_YOUT_H			(69)
#define MPU60X0_9150_GYRO_YOUT_L			(70)
#define MPU60X0_9150_GYRO_ZOUT_H			(71)
#define MPU60X0_9150_GYRO_ZOUT_L			(72)
#define MPU60X0_9150_EXT_SENS_DATA_00	(73)
#define MPU60X0_9150_EXT_SENS_DATA_01	(74)
#define MPU60X0_9150_EXT_SENS_DATA_02	(75)
#define MPU60X0_9150_EXT_SENS_DATA_03	(76)
#define MPU60X0_9150_EXT_SENS_DATA_04	(77)
#define MPU60X0_9150_EXT_SENS_DATA_05	(78)
#define MPU60X0_9150_EXT_SENS_DATA_06	(79)
#define MPU60X0_9150_EXT_SENS_DATA_07	(80)
#define MPU60X0_9150_EXT_SENS_DATA_08	(81)
#define MPU60X0_9150_EXT_SENS_DATA_09	(82)
#define MPU60X0_9150_EXT_SENS_DATA_10	(83)
#define MPU60X0_9150_EXT_SENS_DATA_11	(84)
#define MPU60X0_9150_EXT_SENS_DATA_12	(85)
#define MPU60X0_9150_EXT_SENS_DATA_13	(86)
#define MPU60X0_9150_EXT_SENS_DATA_14	(87)
#define MPU60X0_9150_EXT_SENS_DATA_15	(88)
#define MPU60X0_9150_EXT_SENS_DATA_16	(89)
#define MPU60X0_9150_EXT_SENS_DATA_17	(90)
#define MPU60X0_9150_EXT_SENS_DATA_18	(91)
#define MPU60X0_9150_EXT_SENS_DATA_19	(92)
#define MPU60X0_9150_EXT_SENS_DATA_20	(93)
#define MPU60X0_9150_EXT_SENS_DATA_21	(94)
#define MPU60X0_9150_EXT_SENS_DATA_22	(95)
#define MPU60X0_9150_EXT_SENS_DATA_23	(96)
#define MPU60X0_9150_I2C_SLV0_DO			(99)
#define MPU60X0_9150_I2C_SLV1_DO			(100)
#define MPU60X0_9150_I2C_SLV2_DO			(101)
#define MPU60X0_9150_I2C_SLV3_DO			(102)
#define MPU60X0_9150_I2C_MST_DELAY_CTRL	(103)
#define MPU60X0_9150_SIGNAL_PATH_RESET	(104)
#define MPU60X0_9150_MOT_DETECT_CTRL		(105)
#define MPU60X0_9150_USER_CTRL			(106)
#define MPU60X0_9150_PWR_MGMT_1			(107)
#define MPU60X0_9150_PWR_MGMT_2			(108)
#define MPU60X0_9150_FIFO_COUNTH			(114)
#define MPU60X0_9150_FIFO_COUNTL			(115)
#define MPU60X0_9150_FIFO_R_W			(116)
#define MPU60X0_9150_WHO_AM_I			(117)

//----------------------------------------------------------------------------------------
#define MPU60X0_9150_CONFIG_DLPF_CFG_gp			(0)
#define MPU60X0_9150_CONFIG_DLPF_CFG_gm			(0x07)
//----------------------------------------------------------------------------------------
#define MPU60X0_9150_GYRO_CONFIG_ST_gp			(5)
#define MPU60X0_9150_GYRO_CONFIG_ST_gm			(0x07 << MPU60X0_9150_GYRO_CONFIG_ST_gp)
#define MPU60X0_9150_GYRO_CONFIG_ZG_ST_bp		(5)
#define MPU60X0_9150_GYRO_CONFIG_ZG_ST_bm		(0x01 << MPU60X0_9150_GYRO_CONFIG_ZG_ST_bp)
#define MPU60X0_9150_GYRO_CONFIG_YG_ST_bp		(6)
#define MPU60X0_9150_GYRO_CONFIG_YG_ST_bm		(0x01 << MPU60X0_9150_GYRO_CONFIG_YG_ST_bp)
#define MPU60X0_9150_GYRO_CONFIG_XG_ST_bp		(7)
#define MPU60X0_9150_GYRO_CONFIG_XG_ST_bm		(0x01 << MPU60X0_9150_GYRO_CONFIG_XG_ST_bp)

#define MPU60X0_9150_GYRO_CONFIG_FS_SEL_gp		(3)
#define MPU60X0_9150_GYRO_CONFIG_FS_SEL_gm		(0x03 << MPU60X0_9150_GYRO_CONFIG_FS_SEL_gp)
#define MPU60X0_9150_GYRO_CONFIG_FS_SEL_250		(0)
#define MPU60X0_9150_GYRO_CONFIG_FS_SEL_500		(1)
#define MPU60X0_9150_GYRO_CONFIG_FS_SEL_1000		(2)
#define MPU60X0_9150_GYRO_CONFIG_FS_SEL_2000		(3)
//----------------------------------------------------------------------------------------
#define MPU60X0_9150_ACCEL_CONFIG_ST_gp			(5)
#define MPU60X0_9150_ACCEL_CONFIG_ST_gm			(0x07 << MPU60X0_9150_ACCEL_CONFIG_ST_gp)
#define MPU60X0_9150_ACCEL_CONFIG_ZA_ST_bp		(5)
#define MPU60X0_9150_ACCEL_CONFIG_ZA_ST_bm		(0x01 << MPU60X0_9150_ACCEL_CONFIG_ZA_ST_bp)
#define MPU60X0_9150_ACCEL_CONFIG_YA_ST_bp		(6)
#define MPU60X0_9150_ACCEL_CONFIG_YA_ST_bm		(0x01 << MPU60X0_9150_ACCEL_CONFIG_YA_ST_bp)
#define MPU60X0_9150_ACCEL_CONFIG_XA_ST_bp		(7)
#define MPU60X0_9150_ACCEL_CONFIG_XA_ST_bm		(0x01 << MPU60X0_9150_ACCEL_CONFIG_XA_ST_bp)

#define MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gp		(3)
#define MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gm		(0x03 << MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gp)
#define MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_2		(0)
#define MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_4		(1)
#define MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_8		(2)
#define MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_16		(3)
//----------------------------------------------------------------------------------------
#define MPU60X0_9150_PWR_MGMT_1_CLKSEL_gp		(0)
#define MPU60X0_9150_PWR_MGMT_1_CLKSEL_gm		(0x07 << MPU60X0_9150_PWR_MGMT_1_CLKSEL_gp)
#define MPU60X0_9150_PWR_MGMT_1_CLKSEL_INT_8MHZ	(0)
#define MPU60X0_9150_PWR_MGMT_1_CLKSEL_PLL_X_AX	(1)
#define MPU60X0_9150_PWR_MGMT_1_CLKSEL_PLL_Y_AX	(2)
#define MPU60X0_9150_PWR_MGMT_1_CLKSEL_PLL_Z_AX	(3)
#define MPU60X0_9150_PWR_MGMT_1_CLKSEL_PLL_32768	(4)
#define MPU60X0_9150_PWR_MGMT_1_CLKSEL_PLL_19_2M	(5)
#define MPU60X0_9150_PWR_MGMT_1_CLKSEL_STOP		(7)

#define MPU60X0_9150_PWR_MGMT_1_TEMP_DIS_bp		(3)
#define MPU60X0_9150_PWR_MGMT_1_TEMP_DIS_bm		(0x01 << MPU60X0_9150_PWR_MGMT_1_TEMP_DIS_bp)

#define MPU60X0_9150_PWR_MGMT_1_CYCLE_bp			(5)
#define MPU60X0_9150_PWR_MGMT_1_CYCLE_bm			(0x01 << MPU60X0_9150_PWR_MGMT_1_CYCLE_bp)

#define MPU60X0_9150_PWR_MGMT_1_SLEEP_bp			(6)
#define MPU60X0_9150_PWR_MGMT_1_SLEEP_bm			(0x01 << MPU60X0_9150_PWR_MGMT_1_SLEEP_bp)

#define MPU60X0_9150_PWR_MGMT_1_DEVICE_RESET_bp	(7)
#define MPU60X0_9150_PWR_MGMT_1_DEVICE_RESET_bm	(0x01 << MPU60X0_9150_PWR_MGMT_1_DEVICE_RESET_bp)
//----------------------------------------------------------------------------------------
#define MPU60X0_9150_PWR_MGMT_2_STBY_ZG_bp	(0)
#define MPU60X0_9150_PWR_MGMT_2_STBY_ZG_bm	(0x01 << MPU60X0_9150_PWR_MGMT_2_STBY_ZG_bp)

#define MPU60X0_9150_PWR_MGMT_2_STBY_YG_bp	(1)
#define MPU60X0_9150_PWR_MGMT_2_STBY_YG_bm	(0x01 << MPU60X0_9150_PWR_MGMT_2_STBY_YG_bp)

#define MPU60X0_9150_PWR_MGMT_2_STBY_XG_bp	(2)
#define MPU60X0_9150_PWR_MGMT_2_STBY_XG_bm	(0x01 << MPU60X0_9150_PWR_MGMT_2_STBY_XG_bp)

#define MPU60X0_9150_PWR_MGMT_2_STBY_ZA_bp	(3)
#define MPU60X0_9150_PWR_MGMT_2_STBY_ZA_bm	(0x01 << MPU60X0_9150_PWR_MGMT_2_STBY_ZA_bp)

#define MPU60X0_9150_PWR_MGMT_2_STBY_YA_bp	(4)
#define MPU60X0_9150_PWR_MGMT_2_STBY_YA_bm	(0x01 << MPU60X0_9150_PWR_MGMT_2_STBY_YA_bp)

#define MPU60X0_9150_PWR_MGMT_2_STBY_XA_bp	(5)
#define MPU60X0_9150_PWR_MGMT_2_STBY_XA_bm	(0x01 << MPU60X0_9150_PWR_MGMT_2_STBY_XA_bp)

#define MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_gp	(6)
#define MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_gm	(0x03 << MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_gp)
#define MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_1_25HZ	(0)
#define MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_5HZ	(1)
#define MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_20HZ	(2)
#define MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_40HZ	(3)
//----------------------------------------------------------------------------------------
#define MPU60X0_9150_INT_PIN_CFG_I2C_BYPASS_EN_bp	(1)
#define MPU60X0_9150_INT_PIN_CFG_I2C_BYPASS_EN_bm	(0x01 << MPU60X0_9150_INT_PIN_CFG_I2C_BYPASS_EN_bp)
//----------------------------------------------------------------------------------------
/*#####################################################*/
typedef struct MPU60x0_s{
}MPU60x0_9150_t;
/*#####################################################*/
namespace GI {
namespace Sensor {
class Mpu60x0_9x50
{
public:
	Mpu60x0_9x50(char *i2cPath, unsigned char icNr);
	~Mpu60x0_9x50();
	SysErr sampleRateDivSet(unsigned char Value);
	SysErr whoAmI();
	SysErr setDlpfCfg(unsigned char value);
	SysErr setGyroSelfTest(bool X_test, bool Y_test, bool Z_test);
	SysErr setGyroFullScale(unsigned int scale);
	SysErr setAccelSelfTest(bool X_test, bool Y_test, bool Z_test);
	SysErr setAccelFullScale(unsigned int scale);
	SysErr getAccelData(signed short *X_Axis, signed short *Y_Axis, signed short *Z_Axis);
	SysErr getGyroData(signed short *X_Axis, signed short *Y_Axis, signed short *Z_Axis);
	SysErr getTempData(float *Temperature);
	SysErr setClockSel(unsigned char Value);
	SysErr setTempDis(bool Value);
	SysErr setCycle(bool Value);
	SysErr setSleep(bool Value);
	SysErr setDeviceReset(bool Value);
	SysErr setStbyZg(bool Value);
	SysErr setStbyYg(bool Value);
	SysErr setStbyXg(bool Value);
	SysErr setStbyZa(bool Value);
	SysErr setStbyYa(bool Value);
	SysErr setStbyXa(bool Value);
	SysErr setLpWakeCtrl(unsigned char Value);
	SysErr setBypassEnable(bool Value);
	SysErr testGyro(double *XtestResult, double *YtestResult, double *ZtestResult);
	SysErr testAccel(double *XtestResult, double *YtestResult, double *ZtestResult);
	/*bool gyroscope_test_display_result(MPU60x0_9150_t *structure);
	bool accelerometer_test_display_result(MPU60x0_9150_t *structure);
	bool temperature_display_result(MPU60x0_9150_t *structure);
	bool giroscope_display_result(MPU60x0_9150_t *structure);
	bool accelerometer_display_result(MPU60x0_9150_t *structure);*/


	signed short XA_Cal;
	signed short YA_Cal;
	signed short ZA_Cal;
	signed short XG_Cal;
	signed short YG_Cal;
	signed short ZG_Cal;
private:
	GI::Dev::I2c* I2C;
	unsigned char IcNr;
};
}
}

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "MPU60x0_9150.cpp"
#endif
/*#####################################################*/

#endif /* MPU60X0_9150_H_ */
