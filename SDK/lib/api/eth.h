/*
 *  api/eth.h
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
/*#####################################################*/
#ifndef LWIP_DEF_H_
#define LWIP_DEF_H_
/*#####################################################*/
#include <stdbool.h>
#include <interface/eth.h>
/*#####################################################*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* DHCP process states */

typedef struct lan_s
{
	void (*init)(unsigned char instance, unsigned long static_ip);
	unsigned char (*idle)(unsigned char instance);
} lan_t;

/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "adc_def.c"
#endif
/*#####################################################*/
#endif /* ADC_DEF_H_ */
/*#####################################################*/
