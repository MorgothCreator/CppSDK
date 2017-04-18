/*
 *  device/bmp180.h
 */

/*#####################################################*/
#ifndef BMP180_H_
#define BMP180_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/i2c.h"
/*#####################################################*/
#define BMP180_ADDR					(0x77)

#define BMP180_CALIB				0xAA
#define BMP180_ID					0xD0
#define BMP180_SOFT_RESET			0xE0
#define BMP180_CTRL_MEAS			0xF4
#define BMP180_OUT_MSB				0xF6
#define BMP180_OUT_LSB				0xF7
#define BMP180_OUT_XLSB				0xF8

/*
 * CTRL_MEAS reg
 */
#define BMP180_CTRL_MEAS_OSS_gp		6
#define BMP180_CTRL_MEAS_OSS_gm		(0x03 << BMP180_CTRL_MEAS_OSS_gp)

#define BMP180_CTRL_MEAS_SCO_bp		5
#define BMP180_CTRL_MEAS_SCO_bm		(0x01 << BMP180_CTRL_MEAS_SCO_bp)

/*
 * SOFT_RESET reg
 */
#define BMP180_SOFT_RESET_gp		0
#define BMP180_SOFT_RESET_gm		(0xFF << BMP180_CTRL_MEAS_SCO_bp)
#define BMP180_SOFT_RESET_value		0xB6

/*
 * CALIB reg's
 */
#define BMP180_CALIB_LEN			22

/*#####################################################*/
namespace GI {
namespace Sensor {
class Bmp180 {
public:
	Bmp180(char *i2cPath, unsigned char icNr);
	~Bmp180();
	//bool bmp180_display_result(BMP180_t *structure, unsigned char oss);
	SysErr getTemp(float *temp);
	SysErr getPressure(float *pressure, unsigned char oss);
	SysErr getAltitude(float *altitude, unsigned char oss);

	enum oss_e{
		BMP180_CTRL_MEAS_OSS_1 = 0,
		BMP180_CTRL_MEAS_OSS_2,
		BMP180_CTRL_MEAS_OSS_4,
		BMP180_CTRL_MEAS_OSS_8,
	}oss;

private:
	SysErr readTemp(signed long *temp);
	SysErr readPressure(signed long *pressure, unsigned char oss);
	SysErr readCalibration();

	bool busy;
	GI::Dev::I2c* I2C;
	unsigned char IcNr;
	unsigned char Stage;
	struct {
		signed short AC1;
		signed short AC2;
		signed short AC3;
		unsigned short AC4;
		unsigned short AC5;
		unsigned short AC6;
		signed short B1;
		signed short B2;
		signed short MB;
		signed short MC;
		signed short MD;
	}cal_data;
};
}
}
typedef struct BMP180_s{
	//STimer_t Timeout_Timer;
}BMP180_t;

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "bmp180.cpp"
#endif

/*#####################################################*/
#endif
/*#####################################################*/
