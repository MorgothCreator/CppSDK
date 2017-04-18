/*
 * lib/util/hex_string.h
 */

#ifndef LIB_LIB_UTIL_HEX_STRING_H_
#define LIB_LIB_UTIL_HEX_STRING_H_

#include <stdbool.h>
#include <ctype.h>
#include <include/global.h>

void GetHexChar(char *hex_str, u8 data);
void GetHexBuff(char *hex_str, u8 *data, u32 data_len);
bool GetBinFromHexChar(u8 *dest, char src);
u32 GetBinFromHexBuff(u8 *bin_buff, char *data,
		u32 dest_buff_len);
/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "hex_string.cpp"
#endif
/*#####################################################*/

#endif /* LIB_LIB_UTIL_HEX_STRING_H_ */
