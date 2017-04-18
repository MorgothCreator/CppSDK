/*
 * string.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: John Smith
 */

#include "string.h"


GI::String::String()
{
	memset(this, 0, sizeof(*this));
	buff = (char *) calloc(1, 1);
	if (buff)
	{
		error = SYS_ERR_OK;

	}
}

GI::String::String(char *String)
{
	memset(this, 0, sizeof(*this));
	unsigned int len = strlen(String);
	buff = (char *) malloc(len + 1);
	if (buff)
	{
		strcpy((char *) buff, (const char *) String);
		length = len;
		error = SYS_ERR_OK;
		modifyed++;
	}
}

GI::String::String(GI::String *String)
{
	memset(this, 0, sizeof(*this));
	unsigned int len = String->length;
	buff = (char *) malloc(len + 1);
	if (buff)
	{
		strcpy((char *) buff, (const char *) String->buff);
		length = len;
		error = SYS_ERR_OK;
		modifyed++;
	}
}

GI::String::~String()
{
	if (buff)
		free((void *) buff);
	memset(this, 0, sizeof(*this));
}

bool GI::String::equal(GI::String *string)
{
	if(!this && !string)
		return false;
	if((modifyed != string->modifyed) ||
	//(strcmp(buff, string.buff) == 0)   ? false :
		(length != string->length))
		return false;
	return true;
}

bool GI::String::equal(char *string)
{
	if(!this && !string)
		return false;
	if(strcmp(buff, string))
		return false;
	return true;
}

void GI::String::toUper()
{
	unsigned int TmpCnt = length;
	char* Str = buff;
	do{
		*Str = (char)toupper((int)*Str);
		Str++;
	}while(--TmpCnt);
	error = SYS_ERR_OK;
	modifyed++;
}

void GI::String::toLower()
{
	unsigned int TmpCnt = length;
	char* Str = buff;
	do{
		*Str = (char)tolower((int)*Str);
		Str++;
	}while(--TmpCnt);
	error = SYS_ERR_OK;
	modifyed++;
}

void GI::String::removeNewLine(GI::String *string)
{
	if(!this && !string)
		return;
	char *_item = buff;
	char *tmp_str1 = buff;
	char *tmp_str2 = buff;
	while (*_item != 0)
	{
		if (*_item == '\n' || *_item == '\r')
			_item++;
		else
			*tmp_str1++ = *_item++;
	}
	*tmp_str1 = 0;
	length = tmp_str1 - tmp_str2;
	buff = (char *) realloc(tmp_str2, (tmp_str1 - tmp_str2) + 1);
	error = SYS_ERR_OK;
	modifyed++;
}

void GI::String::set(char* string)
{
	if (!string)
		return;
	unsigned int LenSrc = strlen(string);
	char *Return;
	Return = (char *) realloc(buff, LenSrc + 1);
	*Return = 0;
	if (!Return)
		return;
	strcat(Return, string);
	buff = Return;
	length = LenSrc;
	modifyed++;
	error = SYS_ERR_OK;
}

void GI::String::set(GI::String *string)
{
	if (!string)
		return;
	unsigned int LenSrc = string->length;
	char *Return;
	Return = (char *) realloc(buff, LenSrc + 1);
	*Return = 0;
	if (!Return)
		return;
	strcat(Return, string->buff);
	buff = Return;
	length = LenSrc;
	modifyed++;
	error = SYS_ERR_OK;
}

void GI::String::append(GI::String *string)
{
	if(!buff || !string)
		return;
	unsigned int LenSrc = string->length;
	unsigned int LenDest = length;
	char *Return = (char *)realloc(buff, LenDest + LenSrc + 1);
	//char *Return = (char *)realloc(dest, LenDest + LenSrc + 1);
	if(!Return)
		return;
	strcat(Return, string->buff);
	buff = Return;
	length = LenDest + LenSrc;
	modifyed++;
	error = SYS_ERR_OK;
}

void GI::String::append(char* string)
{
	if(!buff || !string)
		return;
	unsigned int LenSrc = strlen(string);
	unsigned int LenDest = length;
	char *Return = (char *)realloc(buff, LenDest + LenSrc + 1);
	//char *Return = (char *)realloc(dest, LenDest + LenSrc + 1);
	if(!Return)
		return;
	strcat(Return, string);
	buff = Return;
	length = LenDest + LenSrc;
	modifyed++;
	error = SYS_ERR_OK;
}

void GI::String::append(char Char)
{
	signed int LenSrc = (Char == 0x08 || Char == 0x7F) ? -1 : 1;
	if(!length && LenSrc == -1)
		return;
	unsigned int LenDest = length;
	char *Return = (char *) realloc(buff, LenDest + LenSrc + 1);
	//char *Return = (char *)realloc(dest, LenDest + LenSrc + 1);
	if (!Return)
		return;
	char buff_char[2];
	if(LenSrc != -1)
	{
		buff_char[0] = Char;
		buff_char[1] = '\0';
		strcat(Return, buff_char);
	}
	Return[LenDest + LenSrc] = '\0';
	buff = Return;
	length = LenDest + LenSrc;
	modifyed = true;
	error = SYS_ERR_OK;
}

void GI::String::insert(GI::String* string, unsigned int location)
{
	if(!string)
		return;
	unsigned int LenSrc = string->length;
	if(location > LenSrc)
		return;
	if(!buff)
	{
		return;
	}

	char *Return = (char *)realloc(buff, length + LenSrc + 1);
	if(!Return)
		return;
	unsigned int LenDest = length + location;
	char *Tmp = (char *)malloc((length - location) +1);
	if(!Tmp)
		return;
	strcpy(Tmp, buff + location);
	strcpy(Return + location, string->buff);
	strcat(Return, Tmp);
	free(Tmp);
	buff = Return;
	length = LenDest + LenSrc;
	error = SYS_ERR_OK;
	modifyed++;
}

void GI::String::insert(char *string, unsigned int location)
{
	if(!string)
		return;
	unsigned int LenSrc = strlen(string);
	if(location > LenSrc)
		return;
	if(!buff)
	{
		return;
	}

	char *Return = (char *)realloc(buff, length + LenSrc + 1);
	if(!Return)
		return;
	unsigned int LenDest = length + location;
	char *Tmp = (char *)malloc((length - location) +1);
	if(!Tmp)
		return;
	strcpy(Tmp, buff + location);
	strcpy(Return + location, string);
	strcat(Return, Tmp);
	free(Tmp);
	buff = Return;
	length = LenDest + LenSrc;
	error = SYS_ERR_OK;
	modifyed++;
}

void GI::String::clone(GI::String* string)
{
	if (!string)
		return;
	unsigned int LenSrc = string->length;
	char *Return = (char *) realloc(string->buff, LenSrc + 1);
	//char *Return = (char *)realloc(dest, LenDest + LenSrc + 1);
	if (!Return)
		return;
	strcpy(Return, string->buff);
	buff = Return;
	error = SYS_ERR_OK;
    modifyed = string->modifyed;
    length = string->length;
}

void GI::String::clear()
{
	char *Return = (char *) realloc(buff, 1);
	*Return = 0;
	length = 0;
	buff = Return;
	error = SYS_ERR_OK;
	modifyed++;
}
char *GI::String::subString(unsigned int position, unsigned int len)
{
	if (!buff)
		return NULL;
	if (position + len >= (unsigned int) length)
		return NULL;
	char *Return = (char *) malloc(len + 1);
	if (!Return)
		return NULL;
	*Return = 0;
	strncpy(Return, buff + position, len);
	Return[len] = '\0';
	return Return;
}

char *GI::String::subString(unsigned int position)
{
	if (!buff)
		return NULL;
	if (position >= (unsigned int) length)
		return NULL;
	char *Return = (char *) malloc((length - position) + 1);
	if (!Return)
		return NULL;
	*Return = 0;
	strcpy(Return, buff + position);
	return Return;
}

GI::StringArray::StringArray()
{
	memset(this, 0, sizeof(*this));
	array = (GI::String **)calloc(1, sizeof(GI::String *));
}

GI::StringArray::~StringArray()
{
	if(!array)
		return;
	while(itemsCount)
	{
		delete array[itemsCount - 1];
		itemsCount--;
	}
	free(array);
	array = NULL;
}

SysErr GI::StringArray::add(GI::String *str)
{
	if(!str)
		return SYS_ERR_INVALID_PARAM;
	if(!this)
		return SYS_ERR_INVALID_HANDLER;
	if(!array)
		return SYS_ERR_UNKNOWN;
	GI::String * tmp_str = new GI::String(str);
	if(!tmp_str)
		return SYS_ERR_OUT_OF_MEMORY;
	GI::String **tmp_array = (GI::String **)realloc(array, sizeof(GI::String *) * (itemsCount + 1));
	if(!tmp_array)
	{
		delete tmp_str;
		return SYS_ERR_OUT_OF_MEMORY;
	}
	tmp_str->set(str);
	tmp_array[itemsCount] = tmp_str;
	array = tmp_array;
	itemsCount++;
	return SYS_ERR_OK;
}

SysErr GI::StringArray::add(char *str)
{
	if(!str)
		return SYS_ERR_INVALID_PARAM;
	if(!this)
		return SYS_ERR_INVALID_HANDLER;
	if(!array)
		return SYS_ERR_UNKNOWN;
	GI::String * tmp_str = new GI::String(str);
	if(!tmp_str)
		return SYS_ERR_OUT_OF_MEMORY;
	GI::String **tmp_array = (GI::String **)realloc(array, sizeof(GI::String *) * (itemsCount + 1));
	if(!tmp_array)
		return SYS_ERR_OUT_OF_MEMORY;
	tmp_array[itemsCount] = tmp_str;
	array = tmp_array;
	itemsCount++;
	return SYS_ERR_OK;
}

SysErr GI::StringArray::insert(GI::String *str, unsigned int position)
{
	if(!str)
		return SYS_ERR_INVALID_PARAM;
	if(!this)
		return SYS_ERR_INVALID_HANDLER;
	if(!array)
		return SYS_ERR_UNKNOWN;
	if(itemsCount == 0 && position == 0)
	{
		return add(str);
	}
	GI::String * tmp_str = new GI::String(str);
	if(!tmp_str)
		return SYS_ERR_OUT_OF_MEMORY;
	GI::String **tmp_array = (GI::String **)realloc(array, sizeof(GI::String *) * (itemsCount + 1));
	if(!tmp_array)
	{
		delete tmp_str;
		return SYS_ERR_OUT_OF_MEMORY;
	}
	//memmove(tmp_array + (sizeof(GI::String *) * (position + 1)), tmp_array + (sizeof(GI::String *) * position), sizeof(GI::String *) * (itemsCount - position));
	unsigned long cnt = itemsCount - position;
	for(; cnt > 0; cnt--)
	{
		tmp_array[cnt + position] = tmp_array[(cnt - 1) + position];
	}
	tmp_array[position] = tmp_str;
	array = tmp_array;
	itemsCount++;
	return SYS_ERR_OK;
}

SysErr GI::StringArray::insert(char *str, unsigned int position)
{
	if(!str)
		return SYS_ERR_INVALID_PARAM;
	if(!this)
		return SYS_ERR_INVALID_HANDLER;
	if(!array)
		return SYS_ERR_UNKNOWN;
	if(itemsCount == 0 && position == 0)
	{
		return add(str);
	}
	GI::String * tmp_str = new GI::String(str);
	if(!tmp_str)
		return SYS_ERR_OUT_OF_MEMORY;
	GI::String **tmp_array = (GI::String **)realloc(array, sizeof(GI::String *) * (itemsCount + 1));
	if(!tmp_array)
		return SYS_ERR_OUT_OF_MEMORY;
	memmove(tmp_array + (sizeof(GI::String *) * (position + 1)), tmp_array + (sizeof(GI::String *) * position), sizeof(GI::String *) * (itemsCount - position));
	tmp_array[itemsCount] = tmp_str;
	array = tmp_array;
	itemsCount++;
	return SYS_ERR_OK;
}

SysErr GI::StringArray::remove(unsigned int position)
{
	if(!this)
		return SYS_ERR_INVALID_HANDLER;
	if(!array)
		return SYS_ERR_UNKNOWN;
	if(position >= itemsCount)
		return SYS_ERR_OUT_OF_RANGE;
	delete array[position];
	memmove(array + (sizeof(GI::String *) * position), array + (sizeof(GI::String *) * (position + 1)), sizeof(GI::String *) * (itemsCount - position - 1));
	GI::String **tmp_array = (GI::String **)realloc(array, sizeof(GI::String *) * (itemsCount - 1));
	array = tmp_array;
	itemsCount--;
	if(!tmp_array)
		return SYS_ERR_OUT_OF_MEMORY;
	return SYS_ERR_OK;
}

GI::StringCharArray::StringCharArray()
{
	memset(this, 0, sizeof(*this));
	array = (char **)calloc(1, sizeof(char *));
}

GI::StringCharArray::StringCharArray(GI::StringArray *strArray)
{
	memset(this, 0, sizeof(*this));
	char **char_array = (char **)malloc(sizeof(char *) * strArray->itemsCount);
	if(!char_array)
		return;
	unsigned int cnt = 0;
	for(; cnt < strArray->itemsCount; cnt++)
	{
		char *item_str = (char *)malloc(strArray->array[cnt]->length + 1);
		if(!item_str)
			break;
		strcpy(item_str, strArray->array[cnt]->buff);
		char_array[cnt] = item_str;
	}
	itemsCount = strArray->itemsCount;
	array = char_array;
}

GI::StringCharArray::StringCharArray(GI::StringArray *strArray, unsigned int offset)
{
	memset(this, 0, sizeof(*this));
	if(offset > strArray->itemsCount)
		return;
	char **char_array = (char **)malloc(sizeof(char *) * strArray->itemsCount - offset);
	if(!char_array)
		return;
	unsigned int cnt = offset;
	unsigned int dest_cnt = 0;
	for(; cnt < strArray->itemsCount; cnt++)
	{
		char *item_str = (char *)malloc(strArray->array[cnt]->length + 1);
		if(!item_str)
			break;
		strcpy(item_str, strArray->array[cnt]->buff);
		char_array[dest_cnt++] = item_str;
	}
	itemsCount = strArray->itemsCount - offset;
	array = char_array;
}

GI::StringCharArray::~StringCharArray()
{
	if(!array)
		return;
	while(itemsCount)
	{
		free((void *)array[itemsCount - 1]);
		itemsCount--;
	}
	free(array);
}


SysErr GI::StringCharArray::convert(GI::StringArray *strArray)
{
	char **char_array = (char **)realloc(array, sizeof(char *) * strArray->itemsCount);
	if(!char_array)
		return SYS_ERR_OUT_OF_MEMORY;
	unsigned int cnt = 0;
	for(; cnt < strArray->itemsCount; cnt++)
	{
		char *item_str = (char *)malloc(strArray->array[cnt]->length + 1);
		if(!item_str)
			break;
		strcpy(item_str, strArray->array[cnt]->buff);
		char_array[cnt] = item_str;
	}
	itemsCount = strArray->itemsCount;
	array = char_array;
	if(cnt == strArray->itemsCount)
		return SYS_ERR_OUT_OF_MEMORY;
	return SYS_ERR_OK;
}

SysErr GI::StringCharArray::convert(GI::StringArray *strArray, unsigned int offset)
{
	if(offset >= strArray->itemsCount)
		return SYS_ERR_OUT_OF_RANGE;
	char **char_array = (char **)malloc(sizeof(char *) * strArray->itemsCount - offset);
	if(!char_array)
		return SYS_ERR_OUT_OF_MEMORY;
	unsigned int cnt = offset;
	unsigned int dest_cnt = 0;
	for(; cnt < strArray->itemsCount; cnt++)
	{
		char *item_str = (char *)malloc(strArray->array[cnt]->length + 1);
		if(!item_str)
			break;
		strcpy(item_str, strArray->array[cnt]->buff);
		char_array[dest_cnt++] = item_str;
	}
	itemsCount = strArray->itemsCount;
	array = char_array;
	return SYS_ERR_OK;
}



