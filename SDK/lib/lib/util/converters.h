/*
 *  lib/generic.h
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

#ifndef __Generic_H__
#define __Generic_H__

//#####################################################
typedef struct ToPercentageWithDecimals_Struct
{
	signed int Value;
	signed char Decimal;
}ToPercentageWithDecimals_t;
//#####################################################
namespace GUtil
{
class Converters
{
public:
	static signed toPercent(signed int MinValue, signed int MaxValue, signed int MaxPercentageValue, signed int Value);
	static unsigned int toPercent(unsigned int MinValue, unsigned int MaxValue, unsigned int MaxPercentageValue, unsigned int Value);
	static double toPercent(double MinValue, double MaxValue, double MaxPercentageValue, double Value);
	static ToPercentageWithDecimals_t toPercentDecimal(signed int MinValue, signed int MaxValue, signed int MaxPercentageValue, signed int Value);
	static signed int percentTo(signed int MinValue, signed int MaxValue, signed int MaxPercentageValue, signed int Value);
	static double percentTo(double MinValue, double MaxValue, double MaxPercentageValue, double Value);
};
}
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include <lib/util/converters.cpp>
#endif
//#####################################################
#endif
