/*
 *  api/rtc_def.h
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

#ifndef LIB_API_RTC_DEF_H_
#define LIB_API_RTC_DEF_H_

typedef struct rtc_s
{
	unsigned char Year;
	unsigned char Month;
	unsigned char Date;
	unsigned char WeekDay;
	unsigned char Hours;
	unsigned char Minutes;
	unsigned char Seconds;
	unsigned char TimeFormat;
} rtc_t;

#endif /* LIB_API_RTC_DEF_H_ */
