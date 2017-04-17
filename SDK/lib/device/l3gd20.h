/*
 *  lib/device/l3gd20.h
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
#ifndef LIB_DEVICE_L3GD20_H_
#define LIB_DEVICE_L3GD20_H_

#include <api/io_handle.h>

#define L3GD20_WHO_AM_I_ADDR          0x0F  /* device identification register */
#define L3GD20_CTRL_REG1_ADDR         0x20  /* Control register 1 */
#define L3GD20_CTRL_REG2_ADDR         0x21  /* Control register 2 */
#define L3GD20_CTRL_REG3_ADDR         0x22  /* Control register 3 */
#define L3GD20_CTRL_REG4_ADDR         0x23  /* Control register 4 */
#define L3GD20_CTRL_REG5_ADDR         0x24  /* Control register 5 */
#define L3GD20_REFERENCE_REG_ADDR     0x25  /* Reference register */
#define L3GD20_OUT_TEMP_ADDR          0x26  /* Out temp register */
#define L3GD20_STATUS_REG_ADDR        0x27  /* Status register */
#define L3GD20_OUT_X_L_ADDR           0x28  /* Output Register X */
#define L3GD20_OUT_X_H_ADDR           0x29  /* Output Register X */
#define L3GD20_OUT_Y_L_ADDR           0x2A  /* Output Register Y */
#define L3GD20_OUT_Y_H_ADDR           0x2B  /* Output Register Y */
#define L3GD20_OUT_Z_L_ADDR           0x2C  /* Output Register Z */
#define L3GD20_OUT_Z_H_ADDR           0x2D  /* Output Register Z */
#define L3GD20_FIFO_CTRL_REG_ADDR     0x2E  /* Fifo control Register */
#define L3GD20_FIFO_SRC_REG_ADDR      0x2F  /* Fifo src Register */

#define L3GD20_INT1_CFG_ADDR          0x30  /* Interrupt 1 configuration Register */
#define L3GD20_INT1_SRC_ADDR          0x31  /* Interrupt 1 source Register */
#define L3GD20_INT1_TSH_XH_ADDR       0x32  /* Interrupt 1 Threshold X register */
#define L3GD20_INT1_TSH_XL_ADDR       0x33  /* Interrupt 1 Threshold X register */
#define L3GD20_INT1_TSH_YH_ADDR       0x34  /* Interrupt 1 Threshold Y register */
#define L3GD20_INT1_TSH_YL_ADDR       0x35  /* Interrupt 1 Threshold Y register */
#define L3GD20_INT1_TSH_ZH_ADDR       0x36  /* Interrupt 1 Threshold Z register */
#define L3GD20_INT1_TSH_ZL_ADDR       0x37  /* Interrupt 1 Threshold Z register */
#define L3GD20_INT1_DURATION_ADDR     0x38  /* Interrupt 1 DURATION register */

#define I_AM_L3GD20                 ((unsigned char)0xD4)
#define I_AM_L3GD20_TR              ((unsigned char)0xD5)

/** @defgroup Power_Mode_selection
  * @{
  */
#define L3GD20_MODE_POWERDOWN       ((unsigned char)0x00)
#define L3GD20_MODE_ACTIVE          ((unsigned char)0x08)
/**
  * @}
  */

/** @defgroup OutPut_DataRate_Selection
  * @{
  */
#define L3GD20_OUTPUT_DATARATE_1    ((unsigned char)0x00)
#define L3GD20_OUTPUT_DATARATE_2    ((unsigned char)0x40)
#define L3GD20_OUTPUT_DATARATE_3    ((unsigned char)0x80)
#define L3GD20_OUTPUT_DATARATE_4    ((unsigned char)0xC0)
/**
  * @}
  */

/** @defgroup Axes_Selection
  * @{
  */
#define L3GD20_X_ENABLE            ((unsigned char)0x02)
#define L3GD20_Y_ENABLE            ((unsigned char)0x01)
#define L3GD20_Z_ENABLE            ((unsigned char)0x04)
#define L3GD20_AXES_ENABLE         ((unsigned char)0x07)
#define L3GD20_AXES_DISABLE        ((unsigned char)0x00)
/**
  * @}
  */

/** @defgroup Bandwidth_Selection
  * @{
  */
#define L3GD20_BANDWIDTH_1         ((unsigned char)0x00)
#define L3GD20_BANDWIDTH_2         ((unsigned char)0x10)
#define L3GD20_BANDWIDTH_3         ((unsigned char)0x20)
#define L3GD20_BANDWIDTH_4         ((unsigned char)0x30)
/**
  * @}
  */

/** @defgroup Full_Scale_Selection
  * @{
  */
#define L3GD20_FULLSCALE_250       ((unsigned char)0x00)
#define L3GD20_FULLSCALE_500       ((unsigned char)0x10)
#define L3GD20_FULLSCALE_2000      ((unsigned char)0x20)
#define L3GD20_FULLSCALE_SELECTION ((unsigned char)0x30)
/**
  * @}
  */

/** @defgroup Full_Scale_Sensitivity
  * @{
  */
#define L3GD20_SENSITIVITY_250DPS  ((float)8.75f)         /*!< gyroscope sensitivity with 250 dps full scale [DPS/LSB]  */
#define L3GD20_SENSITIVITY_500DPS  ((float)17.50f)        /*!< gyroscope sensitivity with 500 dps full scale [DPS/LSB]  */
#define L3GD20_SENSITIVITY_2000DPS ((float)70.00f)        /*!< gyroscope sensitivity with 2000 dps full scale [DPS/LSB] */
/**
  * @}
  */


/** @defgroup Block_Data_Update
  * @{
  */
#define L3GD20_BlockDataUpdate_Continous   ((unsigned char)0x00)
#define L3GD20_BlockDataUpdate_Single      ((unsigned char)0x80)
/**
  * @}
  */

/** @defgroup Endian_Data_selection
  * @{
  */
#define L3GD20_BLE_LSB                     ((unsigned char)0x00)
#define L3GD20_BLE_MSB	                   ((unsigned char)0x40)
/**
  * @}
  */

/** @defgroup High_Pass_Filter_status
  * @{
  */
#define L3GD20_HIGHPASSFILTER_DISABLE      ((unsigned char)0x00)
#define L3GD20_HIGHPASSFILTER_ENABLE	     ((unsigned char)0x10)
/**
  * @}
  */

/** @defgroup INT1_INT2_selection
  * @{
  */
#define L3GD20_INT1                        ((unsigned char)0x00)
#define L3GD20_INT2                        ((unsigned char)0x01)
/**
  * @}
  */

/** @defgroup INT1_Interrupt_status
  * @{
  */
#define L3GD20_INT1INTERRUPT_DISABLE       ((unsigned char)0x00)
#define L3GD20_INT1INTERRUPT_ENABLE        ((unsigned char)0x80)
/**
  * @}
  */

/** @defgroup INT2_Interrupt_status
  * @{
  */
#define L3GD20_INT2INTERRUPT_DISABLE       ((unsigned char)0x00)
#define L3GD20_INT2INTERRUPT_ENABLE        ((unsigned char)0x08)
/**
  * @}
  */

/** @defgroup INT1_Interrupt_ActiveEdge
  * @{
  */
#define L3GD20_INT1INTERRUPT_LOW_EDGE      ((unsigned char)0x20)
#define L3GD20_INT1INTERRUPT_HIGH_EDGE     ((unsigned char)0x00)
/**
  * @}
  */

/** @defgroup Boot_Mode_selection
  * @{
  */
#define L3GD20_BOOT_NORMALMODE             ((unsigned char)0x00)
#define L3GD20_BOOT_REBOOTMEMORY           ((unsigned char)0x80)
/**
  * @}
  */

/** @defgroup High_Pass_Filter_Mode
  * @{
  */
#define L3GD20_HPM_NORMAL_MODE_RES         ((unsigned char)0x00)
#define L3GD20_HPM_REF_SIGNAL              ((unsigned char)0x10)
#define L3GD20_HPM_NORMAL_MODE             ((unsigned char)0x20)
#define L3GD20_HPM_AUTORESET_INT           ((unsigned char)0x30)
/**
  * @}
  */

/** @defgroup High_Pass_CUT OFF_Frequency
  * @{
  */
#define L3GD20_HPFCF_0              0x00
#define L3GD20_HPFCF_1              0x01
#define L3GD20_HPFCF_2              0x02
#define L3GD20_HPFCF_3              0x03
#define L3GD20_HPFCF_4              0x04
#define L3GD20_HPFCF_5              0x05
#define L3GD20_HPFCF_6              0x06
#define L3GD20_HPFCF_7              0x07
#define L3GD20_HPFCF_8              0x08
#define L3GD20_HPFCF_9              0x09

/*#####################################################*/
namespace GI {
namespace Sensor {
class L3gd20
{
public:
	L3gd20(char *spiPath);
	~L3gd20();

	SysErr filterCfg(unsigned char cfg);
	SysErr filterEn(bool state);
	SysErr read(float *X, float *Y, float *Z);

	bool busy;
	GI::IO *SPI;
private:
	bool writeRegs(unsigned char* pBuffer, unsigned char WriteAddr, unsigned short NumByteToWrite);
	bool readRegs(unsigned char* pBuffer, unsigned char ReadAddr, unsigned short NumByteToRead);
};
}
}

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "l3gd20.cpp"
#endif
/*#####################################################*/

#endif /* LIB_DEVICE_L3GD20_H_ */
