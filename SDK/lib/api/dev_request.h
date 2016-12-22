/*
 * dev_request.h
 *
 *  Created on: Dec 22, 2016
 *      Author: John Smith
 */

#ifndef LIB_API_DEV_REQUEST_H_
#define LIB_API_DEV_REQUEST_H_

#include <include/global.h>
#include <interface/gpio.h>
#include <interface/i2c.h>
#include <interface/spi.h>
#include <interface/uart.h>

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




#endif /* LIB_API_DEV_REQUEST_H_ */
