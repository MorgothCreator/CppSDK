/*
 * device/mi0283_gpio.h
 */

#ifndef LIB_DEVICE_MI0283_GPIO_H_
#define LIB_DEVICE_MI0283_GPIO_H_

#include <api/dev_request.h>
#include <api/lcd_def.h>
#include <api/gpio.h>

static const unsigned char MI0283_screen_setup_data[]
#ifdef __AVR_XMEGA__
                                                    PROGMEM
#endif
                                                    =
{
    0xEA,0x00, 0xEB,0x20, 0xEC,0x0C, 0xED,0xC4, 0xE8,0x40, 0xE9,0x38, 0xF1,0x01, 0xF2,0x10, 0x27,0xA3,
    //power voltage
    0x1B,0x1B, 0x1A,0x01, 0x24,0x2F, 0x25,0x57,
    //VCOM offset
    0x23,0x8D, //for flicker adjust
    /* Gamma settings  -----------------------------------------------------------*/
    0x40,0x00,   //   default setup
    0x41,0x00,   //
    0x42,0x01,   //
    0x43,0x13,   //
    0x44,0x10,   //
    0x45,0x26,   //
    0x46,0x08,   //
    0x47,0x51,   //
    0x48,0x02,   //
    0x49,0x12,   //
    0x4A,0x18,   //
    0x4B,0x19,   //
    0x4C,0x14,   //
    0x50,0x19,   //
    0x51,0x2F,   //
    0x52,0x2C,   //
    0x53,0x3E,   //
    0x54,0x3F,   //
    0x55,0x3F,   //
    0x56,0x2E,   //
    0x57,0x77,   //
    0x58,0x0B,   //
    0x59,0x06,   //
    0x5A,0x07,   //
    0x5B,0x0D,   //
    0x5C,0x1D,   //
    0x5D,0xCC,   //

    //power on
    0x18,0x36, 0x19,0x01, //start osc
    0x01,0x00, //wakeup
    0x1F,0x88, 254, 0x1F,0x80, 254, 0x1F,0x90, 254, 0x1F,0xD0, 254,
    //color selection
    0x17,0x05, //0x0005=65k, 0x0006=262k
    //panel characteristic
    0x36,0x00, 254 ,
    //0x3c, 0xFF,
    //0x3d, 0x04,
    //0x3f, 0xFF,
    //display on
    0x28,0x38, 254, 0x28,0x3C, 255
};
/*#####################################################*/

namespace GI {
namespace Dev {
class Mi0283Gpio : public Screen {
public:
    Mi0283Gpio(LCD_TIMINGS *timings, GI::Dev::Gpio* backlight);
    ~Mi0283Gpio();

    void reset();
    static bool _setOrientation(void *driverHandlerPtr, LCD_ORIENTATION orientation);
    static SysErr _setBacklight(void *driverHandlerPtr, unsigned char value);
    //static void _cacheClean(void *driverHandlerPtr, signed int x_start, signed int y_start, signed int x_len, signed int y_len);
    static void _drawPixel(void *driverHandlerPtr, signed int X_Coordonate, signed int Y_Coordonate, unsigned int Color16);
    static void _drawRectangle(void *driverHandlerPtr, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color);
    static void _drawHLine(void *driverHandlerPtr, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
    static void _drawVLine(void *driverHandlerPtr, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
    static void _clear(void *driverHandlerPtr, unsigned int color);

    void wrCmd(unsigned char cmd);
    void wrData(unsigned char data);
    void setArea(signed int x0, signed int y0, signed int x1, signed int y1);
    void setCursor(signed int x, signed int y);
    void sendPixels(unsigned long PixelsNumber, unsigned long color);

    GI::Dev::Gpio *DATA;
    GI::Dev::Gpio *RESET;
    GI::Dev::Gpio *RS;
    GI::Dev::Gpio *CS;
    GI::Dev::Gpio *WRITE;
    GI::Dev::Gpio *READ;
private:


};
}
}

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "mi0283.cpp"
#endif
/*#####################################################*/

#endif /* LIB_DEVICE_MI0283_GPIO_H_ */
