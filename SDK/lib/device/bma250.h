/*
 * bma250.h
 */

#ifndef LIB_DEVICE_BMA250_H_
#define LIB_DEVICE_BMA250_H_

#include <include/global.h>
#include <api/io_handle.h>

#define BMA250_I2CADDR      0x18

#define BMA250_CHIP_ID_REG                      0x00
#define BMA250_VERSION_REG                      0x01
#define BMA250_X_AXIS_LSB_REG                   0x02
#define BMA250_X_AXIS_MSB_REG                   0x03
#define BMA250_Y_AXIS_LSB_REG                   0x04
#define BMA250_Y_AXIS_MSB_REG                   0x05
#define BMA250_Z_AXIS_LSB_REG                   0x06
#define BMA250_Z_AXIS_MSB_REG                   0x07
#define BMA250_TEMP_RD_REG                      0x08
#define BMA250_STATUS1_REG                      0x09
#define BMA250_STATUS2_REG                      0x0A
#define BMA250_STATUS_TAP_SLOPE_REG             0x0B
#define BMA250_STATUS_ORIENT_HIGH_REG           0x0C
#define BMA250_RANGE_SEL_REG                    0x0F
#define BMA250_BW_SEL_REG                       0x10
#define BMA250_MODE_CTRL_REG                    0x11
#define BMA250_LOW_NOISE_CTRL_REG               0x12
#define BMA250_DATA_CTRL_REG                    0x13
#define BMA250_RESET_REG                        0x14
#define BMA250_INT_ENABLE1_REG                  0x16
#define BMA250_INT_ENABLE2_REG                  0x17
#define BMA250_INT1_PAD_SEL_REG                 0x19
#define BMA250_INT_DATA_SEL_REG                 0x1A
#define BMA250_INT2_PAD_SEL_REG                 0x1B
#define BMA250_INT_SRC_REG                      0x1E
#define BMA250_INT_SET_REG                      0x20
#define BMA250_INT_CTRL_REG                     0x21
#define BMA250_LOW_DURN_REG                     0x22
#define BMA250_LOW_THRES_REG                    0x23
#define BMA250_LOW_HIGH_HYST_REG                0x24
#define BMA250_HIGH_DURN_REG                    0x25
#define BMA250_HIGH_THRES_REG                   0x26
#define BMA250_SLOPE_DURN_REG                   0x27
#define BMA250_SLOPE_THRES_REG                  0x28
#define BMA250_TAP_PARAM_REG                    0x2A
#define BMA250_TAP_THRES_REG                    0x2B
#define BMA250_ORIENT_PARAM_REG                 0x2C
#define BMA250_THETA_BLOCK_REG                  0x2D
#define BMA250_THETA_FLAT_REG                   0x2E
#define BMA250_FLAT_HOLD_TIME_REG               0x2F
#define BMA250_STATUS_LOW_POWER_REG             0x31
#define BMA250_SELF_TEST_REG                    0x32
#define BMA250_EEPROM_CTRL_REG                  0x33
#define BMA250_SERIAL_CTRL_REG                  0x34
#define BMA250_CTRL_UNLOCK_REG                  0x35
#define BMA250_OFFSET_CTRL_REG                  0x36
#define BMA250_OFFSET_PARAMS_REG                0x37
#define BMA250_OFFSET_FILT_X_REG                0x38
#define BMA250_OFFSET_FILT_Y_REG                0x39
#define BMA250_OFFSET_FILT_Z_REG                0x3A
#define BMA250_OFFSET_UNFILT_X_REG              0x3B
#define BMA250_OFFSET_UNFILT_Y_REG              0x3C
#define BMA250_OFFSET_UNFILT_Z_REG              0x3D
#define BMA250_SPARE_0_REG                      0x3E
#define BMA250_SPARE_1_REG                      0x3F

/*#####################################################*/
namespace GI {
namespace Sensor {
class Bma250
{
public:
    Bma250(string hwPath);
    ~Bma250();

    enum range_e {
        BMA250_RANGE_MASK = 0x0C,
        BMA250_RANGE_2G = 0x03,   // 0x03 =  2g
        BMA250_RANGE_4G = 0x05,   // 0x05 =  4g
        BMA250_RANGE_8G = 0x08,   // 0x08 =  8g
        BMA250_RANGE_16G = 0x0C   // 0x0C = 16g
    };

    enum bandwidth_e {
        BMA250_BW_7_81HZ  = 0x8,
        BMA250_BW_15_63HZ = 0x9,
        BMA250_BW_31_25HZ = 0xA,
        BMA250_BW_62_50HZ = 0xB,
        BMA250_BW_125HZ   = 0xC,
        BMA250_BW_250HZ   = 0xD,
        BMA250_BW_500HZ   = 0xE,
        BMA250_BW_1000HZ  = 0xF,
    };

    enum mode_e {
        BMA250_MODE_NORMAL,
        BMA250_MODE_LOWPOWER1,
        BMA250_MODE_SUSPEND,

        BMA250E_MODE_LOWPOWER2,
        BMA250E_MODE_STANDBY,
        BMA250E_MODE_DEEP_SUSPEND,

        BMA250_MODE_COUNT
    };

    SysErr setRange(GI::Sensor::Bma250::range_e range);
    SysErr setBandwidth(GI::Sensor::Bma250::bandwidth_e bandwidth);
    SysErr read(signed short *X, signed short *Y, signed short *Z);
    SysErr read(float *temp);

    bool busy;
    GI::IO *hwIo;
private:
    SysErr writeRegs(unsigned char* pBuffer, unsigned char WriteAddr, unsigned short NumByteToWrite);
    SysErr readRegs(unsigned char* pBuffer, unsigned char ReadAddr, unsigned short NumByteToRead);
};
}
}

/*#####################################################*/



#endif /* LIB_DEVICE_BMA250_H_ */
