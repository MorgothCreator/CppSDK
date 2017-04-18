/*
 * lib/util/hex_string.c
 */

#include "hex_string.h"

void GetHexChar(char *hex_str, u8 data)
{
	char char1 = (data >> 4) & 0x0F;
	char char2 = data & 0x0F;
	if (char1 <= 9)
		hex_str[0] = (char1 + '0');
	else
		hex_str[0] = ((char1 - 10) + 'A');
	if (char2 <= 9)
		hex_str[1] = (char2 + '0');
	else
		hex_str[1] = ((char2 - 10) + 'A');
}

void GetHexBuff(char *hex_str, u8 *data, u32 data_len)
{
	u32 cnt = 0;
	char tmp_str[2];
	for (; cnt < data_len; cnt++)
	{
		GetHexChar(tmp_str, data[cnt]);
		hex_str[cnt * 2] = tmp_str[0];
		hex_str[(cnt * 2) + 1] = tmp_str[1];
	}
	hex_str[cnt * 2] = 0;
}

bool GetBinFromHexChar(u8 *dest, char src)
{
	int tmp = tolower(src);
	if ((tmp < '0' || tmp > '9') && (tmp < 'a' || tmp > 'f'))
		return false;
	if (tmp <= '9')
		*dest = (tmp - '0') & 0x0F;
	else
	{
		*dest = ((tmp - 'a') + 10) & 0x0F;
	}
	return true;
}

u32 GetBinFromHexBuff(u8 *bin_buff, char *data,
		u32 dest_buff_len)
{
	u32 cnt = 0;
	while (*data != 0 && dest_buff_len != 0)
	{
		u8 tmp0 = 0;
		u8 tmp1 = 0;
		if (!GetBinFromHexChar(&tmp1, *data++))
			return 0;
		if (!GetBinFromHexChar(&tmp0, *data++))
			return 0;
		bin_buff[cnt++] = (tmp1 << 4) + tmp0;
		dest_buff_len--;
	}
	return cnt;
}

