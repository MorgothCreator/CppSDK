/*
 * st7565.h
 */

#ifndef LIB_DEVICE_ST7565_H_
#define LIB_DEVICE_ST7565_H_

#include <api/dev_request.h>
#include <api/lcd_def.h>
#include <api/gpio.h>
#include <api/spi.h>
/*#####################################################*/

namespace GI {
namespace Dev {
class St7565 : public Screen {
public:
    St7565(LCD_TIMINGS *timings, char *spiPath);
    ~St7565();

    void reset();
    static void _setOn(void *driverHandlerPtr);
    static void _setOff(void *driverHandlerPtr);
    static bool _setOrientation(void *driverHandlerPtr, LCD_ORIENTATION orientation);
    static void _cacheClean(void *driverHandlerPtr, signed int x_start, signed int y_start, signed int x_len, signed int y_len);
    static void _drawPixel(void *driverHandlerPtr, signed int X_Coordonate, signed int Y_Coordonate, unsigned int color);
    static void _drawRectangle(void *driverHandlerPtr, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color);
    static void _drawHLine(void *driverHandlerPtr, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
    static void _drawVLine(void *driverHandlerPtr, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
    static void _clear(void *driverHandlerPtr, unsigned int color);
    static void _setLuminosity(void *driverHandlerPtr, unsigned char luminosity);
    static void _refresh(void *driverHandlerPtr);

    void wrCmd(unsigned char cmd);

    GI::Dev::Gpio *RESET;
    GI::Dev::Gpio *CD;
    GI::Dev::Spi *spiDevice;
private:
    unsigned char buff[1024];

};
}
}

/*#####################################################*/
#endif /* LIB_DEVICE_ST7565_H_ */
