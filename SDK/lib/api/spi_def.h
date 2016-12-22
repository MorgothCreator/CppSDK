/*
 *  lib/api/mcspi_def.h
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

#ifndef MCSPI_DEF_H_
#define MCSPI_DEF_H_
/*#####################################################*/
#include <stdbool.h>
#include <api/gpio_def.h>
#include <interface/spi_def.h>
/*#####################################################*/
typedef struct
{
	char name[9];
	unsigned int sck;
	unsigned int moSi;
	unsigned int miSo;
	unsigned int cs;
	unsigned int speed;
	enum spiMode{
		spiMode0,
		spiMode1,
		spiMode2,
		spiMode3,
	}spiMode;
	bool msbFirst;
} CfgSpi;
/*#####################################################*/
/*#####################################################*/
#endif /* MCSPI_DEF_H_ */
/*#####################################################*/
