/*
 *  lib/util/converters.h
 */

#ifndef __Generic_H__
#define __Generic_H__

#include <include/global.h>
//#####################################################
typedef struct ToPercentageWithDecimals_Struct
{
	s32 Value;
	signed char Decimal;
}ToPercentageWithDecimals_t;
//#####################################################
namespace GUtil
{
class Converters
{
public:
	static s32 toPercent(s32 MinValue, s32 MaxValue, s32 MaxPercentageValue, s32 Value);
	static u32 toPercent(u32 MinValue, u32 MaxValue, u32 MaxPercentageValue, u32 Value);
	static double toPercent(double MinValue, double MaxValue, double MaxPercentageValue, double Value);
	static ToPercentageWithDecimals_t toPercentDecimal(s32 MinValue, s32 MaxValue, s32 MaxPercentageValue, s32 Value);
	static s32 percentTo(s32 MinValue, s32 MaxValue, s32 MaxPercentageValue, s32 Value);
	static double percentTo(double MinValue, double MaxValue, double MaxPercentageValue, double Value);
};
}
//#####################################################
#ifdef USE_VISUAL_STUDIO
#include "converters.cpp"
#endif
//#####################################################
#endif
