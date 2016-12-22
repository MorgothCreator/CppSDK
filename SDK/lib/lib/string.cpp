/*
 * string.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: John Smith
 */

#include "string.h"


GI::String::String() :
				buff(NULL),
				length(0),
				modifyed(0),
				error(SYS_ERR_OK)
{
	buff = (char *) malloc(1);
	if (buff)
	{
		error = SYS_ERR_OK;

	}
}

GI::String::String(char *String) :
		buff(NULL),
		length(0),
		modifyed(0),
		error(SYS_ERR_OK)
{
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

GI::String::~String()
{
	if (buff)
		free((void *) buff);
}

bool GI::String::equal(GI::String *string)
{
	  if((modifyed != string->modifyed) ||
		//(strcmp(buff, string.buff) == 0)   ? false :
		(length != string->length))
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

void GI::String::append(char* string)
{
	if(!buff)
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
	unsigned int LenSrc = 1;
	unsigned int LenDest = length;
	char *Return = (char *) realloc(buff, LenDest + LenSrc + 1);
	//char *Return = (char *)realloc(dest, LenDest + LenSrc + 1);
	if (!Return)
		return;
	char buff_char[2];
	buff_char[0] = Char;
	buff_char[1] = '\0';
	strcat(Return, buff_char);
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
	if (buff)
		return NULL;
	if (position + len >= (unsigned int) length)
		return NULL;
	char *Return = (char *) calloc(1, len + 1);
	if (!Return)
		return NULL;
	*Return = 0;
	strncpy(Return, buff + position, len);
	return Return;
}


