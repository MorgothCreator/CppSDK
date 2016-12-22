/*
 *  lib/api/twi_def.h
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

#ifndef TWI_DEF_H_
#define TWI_DEF_H_
#include <api/gpio_def.h>
/*#####################################################*/
#define twi_timeout	500000
/*#####################################################*/
typedef struct
{
	char name[9];
	unsigned int scl;
	unsigned int sda;
	unsigned long speed;
	enum i2cMode{
		i2cMode0,
		i2cMode1,
		i2cMode2,
		i2cMode3,
	}i2cMode;

} CfgI2c;
/*#####################################################*/
/*#####################################################*/
#endif /* TWI_DEF_H_ */
/*#####################################################*/
