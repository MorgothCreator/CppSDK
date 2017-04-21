/*
 *  api/cursor_ctl_def.h
 */

#ifndef TOUCHSCREEN_DEF_H_
#define TOUCHSCREEN_DEF_H_
/*#####################################################*/
#include <stdbool.h>
#include "i2c.h"
#include "gpio.h"
#include <api/lcd_def.h>
#include <api/i2c.h>
#include <api/gpio.h>
#include <lib/gfx/controls_definition.h>
/*#####################################################*/
#define TouchScreen_Type_Int	0
#define TouchScreen_Type_FT5x06	1
#define TouchScreen_Type_AR1020	2
/*#####################################################*/
namespace GI
{
namespace Sensor {
class Cursor
{
public:
	//Cursor(GI::Dev::Screen *pDisplay, char *i2cpath, char *irqPinPath);
	//~Cursor();
	bool dataReady();

	double LcdTouch_U_Calibration_Value;
	double LcdTouch_R_Calibration_Value;
	double LcdTouch_D_Calibration_Value;
	double LcdTouch_L_Calibration_Value;
	double LcdTouch_MID_Calibration_Value;
	//double screen_max_x;
	//double screen_max_y;
	unsigned int touch_max_x;
	unsigned int touch_max_y;
	unsigned int press_max;
	unsigned int TouchScreen_Type;

	unsigned char LastState1;
	unsigned char LastState2;
	unsigned char LastState3;
	unsigned char LastState4;
	unsigned char LastState5;
	struct
	{
		signed int	x1;
		signed int	y1;
		signed int	x2;
		signed int	y2;
		signed int	x3;
		signed int	y3;
		signed int	x4;
		signed int	y4;
		signed int	x5;
		signed int	y5;
		signed int	pressure;
		signed int touch_ID1;
		signed int touch_ID2;
		signed int touch_ID3;
		signed int touch_ID4;
		signed int touch_ID5;
		signed int touch_event1;
		signed int touch_event2;
		signed int touch_event3;
		signed int touch_event4;
		signed int touch_event5;
		unsigned char  touch_point;
	}TouchResponse;
	bool flip_x;
	bool flip_y;
	unsigned char twi_addr;
	GI::Dev::Screen *pDisplay;
	GI::Dev::I2c *twiStruct;
	GI::Dev::Gpio *irqHeandle;
	void *UsrData;
	tControlCommandData cursor_ctl;
private:

};
}
}
/*#####################################################*/
#define new_touchscreen LcdTouch_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_touchscreen(address) free(address);\
		address = NULL
/*#####################################################*/
#endif /* TOUCHSCREEN_DEF_H_ */
