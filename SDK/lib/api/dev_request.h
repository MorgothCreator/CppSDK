/*
 *  api/dev_request.h
 *
 *  Copyright (C) 2016  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of CppSDK.
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




#endif /* LIB_API_DEV_REQUEST_H_ */
