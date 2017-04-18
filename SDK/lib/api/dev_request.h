/*
 *  api/dev_request.h
 */

#ifndef LIB_API_DEV_REQUEST_H_
#define LIB_API_DEV_REQUEST_H_

#include <include/global.h>
#include <api/gpio.h>
#include <api/i2c.h>
#include <api/spi.h>
#include <api/uart.h>

namespace GI
{
namespace Dev
{
class DevRequest
{
public:
	static SysErr request(char *path, GI::Dev::Gpio **device);
	static SysErr request(char *path, GI::Dev::I2c **device);
	static SysErr request(char *path, GI::Dev::Spi **device);
	static SysErr request(char *path, GI::Dev::Uart **device);

};
}
}

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "dev_request.cpp"
#endif
/*#####################################################*/



#endif /* LIB_API_DEV_REQUEST_H_ */
