/*
 * string.cpp
 *
 *  Created on: Dec 10, 2016
 *      Author: John Smith
 */

#include <stdio.h>
#include "string.h"

GI::Screen::String::String(GI::Dev::Screen *pDisplay) :
				buff(NULL), pFont((tFont*) &g_sFontCmss14b), length(0), foreground_color(0), background_color(
				0), ulOpaque(true), ulVisible(true), wordWrap(true), lX(0), lY(
				0), _SelStart(0), _SelLen(0), textAlign(alignLeft), modifyed(0), error(
				SYS_ERR_OK)
{
	this->pDisplay = pDisplay;
	buff = (char *) malloc(1);
	if (buff)
	{
		error = SYS_ERR_OK;

	}
}

GI::Screen::String::String(GI::Dev::Screen *pDisplay, char *String) :
		buff(NULL), pFont((tFont*) &g_sFontCmss14b), length(0), foreground_color(0), background_color(
				0), ulOpaque(true), ulVisible(true), wordWrap(true), lX(0), lY(
				0), _SelStart(0), _SelLen(0), textAlign(alignLeft), modifyed(0), error(
				SYS_ERR_OK)
{
	this->pDisplay = pDisplay;
	gfx_u32 len = strlen(String);
	buff = (char *) malloc(len + 1);
	if (buff)
	{
		strcpy((char *) buff, (const char *) String);
		length = len;
		error = SYS_ERR_OK;
		modifyed++;
	}
}

GI::Screen::String::~String()
{
	if(!this)
		return;
	if (buff)
		free((void *) buff);
}

bool GI::Screen::String::equal(GI::Screen::String *string)
{
	if(!this && !string)
		return false;
	if(!buff)
		return false;
	  if((modifyed != string->modifyed) ||
		//(strcmp(buff, string.buff) == 0)   ? false :
		(pFont != string->pFont) ||
		(length != string->length) ||
		//(foreground_color != string->foreground_color) ||
		(ulOpaque != string->ulOpaque) ||
		(ulVisible != string->ulVisible) ||
		(wordWrap != string->wordWrap) ||
		(lX != string->lX) ||
		(lY != string->lY) ||
		(_SelStart != string->_SelStart) ||
		(_SelLen != string->_SelLen) ||
		(textAlign != string->textAlign) ||
		(pDisplay != string->pDisplay))
		  return false;
	  return true;
}

bool GI::Screen::String::equal(char *string)
{
	if(!this && !string)
		return false;
	if(strcmp(buff, string))
		return false;
	return true;
}

void GI::Screen::String::toUper()
{
	if(!this)
		return;
	if(!buff)
		return;
	gfx_u32 TmpCnt = length;
	char* Str = buff;
	do{
		*Str = (char)toupper((int)*Str);
		Str++;
	}while(--TmpCnt);
	error = SYS_ERR_OK;
	modifyed++;
}

void GI::Screen::String::toLower()
{
	if(!this)
		return;
	if(!buff)
		return;
	gfx_u32 TmpCnt = length;
	char* Str = buff;
	do{
		*Str = (char)tolower((int)*Str);
		Str++;
	}while(--TmpCnt);
	error = SYS_ERR_OK;
	modifyed++;
}

void GI::Screen::String::removeNewLine(GI::Screen::String *string)
{
	if(!this && !string)
		return;
	if(!buff)
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

void GI::Screen::String::setText(char* string)
{
	if(!this && !string)
		return;
	if(!buff)
		return;
	gfx_u32 LenSrc = strlen(string);
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

void GI::Screen::String::setText(GI::Screen::String *string)
{
	if(!this && !string)
		return;
	if(!buff)
		return;
	gfx_u32 LenSrc = string->length;
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

void GI::Screen::String::setText(GI::String *string)
{
	if(!this && !string)
		return;
	if(!buff)
		return;
	gfx_u32 LenSrc = string->length;
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

void GI::Screen::String::append(char* string)
{
	if(!this && !string)
		return;
	if(!buff)
		return;
	gfx_u32 LenSrc = strlen(string);
	gfx_u32 LenDest = length;
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

void GI::Screen::String::append(GI::Screen::String *string)
{
	if(!this && !string)
		return;
	if(!buff)
		return;
	gfx_u32 LenSrc = string->length;
	gfx_u32 LenDest = length;
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

void GI::Screen::String::append(GI::String *string)
{
	if(!this && !string)
		return;
	if(!buff)
		return;
	gfx_u32 LenSrc = string->length;
	gfx_u32 LenDest = length;
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

void GI::Screen::String::append(char Char)
{
	if(!this)
		return;
	if(!buff)
		return;
	gfx_u32 LenSrc = 1;
	gfx_u32 LenDest = length;
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

void GI::Screen::String::insert(GI::Screen::String* string, gfx_u32 location)
{
	if(!this && !string)
		return;
	if(!buff)
		return;
	gfx_u32 LenSrc = string->length;
	if(location > LenSrc)
		return;
	if(!buff)
	{
		return;
	}

	char *Return = (char *)realloc(buff, length + LenSrc + 1);
	if(!Return)
		return;
	gfx_u32 LenDest = length + location;
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

void GI::Screen::String::insert(GI::String* string, gfx_u32 location)
{
	if(!this && !string)
		return;
	if(!buff)
		return;
	gfx_u32 LenSrc = string->length;
	if(location > LenSrc)
		return;
	if(!buff)
	{
		return;
	}

	char *Return = (char *)realloc(buff, length + LenSrc + 1);
	if(!Return)
		return;
	gfx_u32 LenDest = length + location;
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

void GI::Screen::String::insert(char *string, gfx_u32 location)
{
	if(!this && !string)
		return;
	if(!buff)
		return;
	gfx_u32 LenSrc = strlen(string);
	if(location > LenSrc)
		return;

	char *Return = (char *)realloc(buff, length + LenSrc + 1);
	if(!Return)
		return;
	gfx_u32 LenDest = length + location;
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

void GI::Screen::String::clone(GI::Screen::String* string)
{
	if (!this && !string)
		return;
	if(!buff)
		return;
	gfx_u32 LenSrc = string->length;
	char *Return = (char *) realloc(string->buff, LenSrc + 1);
	//char *Return = (char *)realloc(dest, LenDest + LenSrc + 1);
	if (!Return)
		return;
	strcpy(Return, string->buff);
	buff = Return;
	error = SYS_ERR_OK;
    modifyed = string->modifyed;
	pFont = string->pFont;
    length = string->length;
    foreground_color = string->foreground_color;
    ulOpaque = string->ulOpaque;
	ulVisible = string->ulVisible;
	wordWrap = string->wordWrap;
	lX = string->lX;
	lY = string->lY;
	_SelStart = string->_SelStart;
	_SelLen = string->_SelLen;
	textAlign = string->textAlign;
	pDisplay = string->pDisplay;
}

void GI::Screen::String::clear()
{
	if(!this && !buff)
		return;
	char *Return = (char *) realloc(buff, 1);
	*Return = 0;
	length = 0;
	buff = Return;
	error = SYS_ERR_OK;
	modifyed++;
}

char *GI::Screen::String::subString(gfx_u32 position, gfx_u32 len)
{
	if(!this && !buff)
		return NULL;
	if (position + len >= (gfx_u32) length)
		return NULL;
	char *Return = (char *) calloc(1, len + 1);
	if (!Return)
		return NULL;
	*Return = 0;
	strncpy(Return, buff + position, len);
	return Return;
}

void GI::Screen::String::getStrSelect(
		gfx_s32 *Start, gfx_s32 *SelStartReturn,
		gfx_s32 *SelLenReturn, gfx_s32 _XPush, gfx_s32 _YPush,
		gfx_s32 _XPull, gfx_s32 _YPull, gfx_s32 lX, gfx_s32 lY,
		gfx_u32 *return_command)
{
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;
	char *pcString = buff;
	gfx_s32 lLength = -1;
	if (LcdStruct->sClipRegion.sXMin > _XPull)
	{
		*return_command = ReturnCommand_GoLeft;
		gfx_u32 tmp = LcdStruct->sClipRegion.sXMin - _XPull;
		if (tmp > ReturnCommand_MaxValue)
			tmp = ReturnCommand_MaxValue;
		*return_command |= tmp;
	}
	if (LcdStruct->sClipRegion.sXMax < _XPull)
	{
		*return_command = ReturnCommand_GoRight;
		gfx_u32 tmp = _XPull - LcdStruct->sClipRegion.sXMax;
		if (tmp > ReturnCommand_MaxValue)
			tmp = ReturnCommand_MaxValue;
		*return_command |= tmp;
	}
	if (LcdStruct->sClipRegion.sYMin > _YPull)
	{
		*return_command = ReturnCommand_GoUp;
		gfx_u32 tmp = LcdStruct->sClipRegion.sYMin - _YPull;
		if (tmp > ReturnCommand_MaxValue)
			tmp = ReturnCommand_MaxValue;
		*return_command |= tmp;
	}
	if (LcdStruct->sClipRegion.sYMax < _YPull)
	{
		*return_command = ReturnCommand_GoDn;
		gfx_u32 tmp = _YPull - LcdStruct->sClipRegion.sYMax;
		if (tmp > ReturnCommand_MaxValue)
			tmp = ReturnCommand_MaxValue;
		*return_command |= tmp;
	}
	if (*return_command)
		return;

	gfx_s32 StringColsHeight_Pixels = lY;
	//gfx_s32 CharHeight_Pixels = 0;

	const u16 *pusOffset;
	const gfx_u8 *pucData;
	//gfx_s32 lRows;
	volatile gfx_s32 StringLengthOfEveryRow = lX;

	//
	// Get some pointers to relevant information in the font to make things
	// easier, and give the compiler a hint about extraneous loads that it can
	// avoid.
	//
#ifdef FLASH_DEVICE
	pusOffset = (const u16 *)pFont + 2;
	pucData = (const gfx_u8 *)&pFont->pucData + 2;
#else
	pusOffset = pFont->pusOffset;
	pucData = pFont->pucData;
#endif

	gfx_s32 XPush = _XPush;
	gfx_s32 YPush = _YPush;
	gfx_s32 XPull = _XPull;
	gfx_s32 YPull = _YPull;

	/*if(XPush > XPull)
	 {
	 gfx_s32 Tmp = XPull;
	 XPull = XPush;
	 XPush = Tmp;
	 }
	 if(YPush > YPull)
	 {
	 gfx_s32 Tmp = YPull;
	 YPull = YPush;
	 YPush = Tmp;
	 }*/

	gfx_s32 CharCount = 0;

	bool StartSelected = false;
	bool LenSelected = false;

	//
	// Loop through the characters in the string.
	//
	for (; *pcString && lLength; pcString++, lLength--)
	{
		if (*Start
				< 0&& XPush < StringLengthOfEveryRow + (read_data_byte(pucData[read_data_word(pusOffset[*pcString - ' ']) + 1]) >> 1) && YPush < StringColsHeight_Pixels + read_data_byte(pFont->ucHeight) && StartSelected == false)
		{
			*SelStartReturn = CharCount;
			*Start = CharCount;
			StartSelected = true;
		}
		if (*Start
				>= 0&& XPull < StringLengthOfEveryRow + (read_data_byte(pucData[read_data_word(pusOffset[*pcString - ' ']) + 1]) >> 1) && YPull < StringColsHeight_Pixels + read_data_byte(pFont->ucHeight) && LenSelected == false)
		{
			*SelLenReturn = CharCount - *SelStartReturn;
			LenSelected = true;
			return;
		}

		CharCount += 1;
		//
		// Get a pointer to the font data for the next character from the
		// string.  If there is not a glyph for the next character, replace it
		// with a ".".
		//
		if (((StringLengthOfEveryRow - LcdStruct->sClipRegion.sXMin)
				+ read_data_byte(
						pucData[read_data_word(pusOffset[*pcString - ' '] ) + 1])
				>= (LcdStruct->sClipRegion.sXMax - LcdStruct->sClipRegion.sXMin)
				&& wordWrap == true))
		{
			//pucData = 0;

			StringLengthOfEveryRow = lX;
			StringColsHeight_Pixels += read_data_byte(pFont->ucHeight);
		}
		if (*pcString == '\r')
		{
			//pucData = 0;

			StringLengthOfEveryRow = lX;
			StringColsHeight_Pixels += read_data_byte(pFont->ucHeight);
		}
		else if (*pcString == '\n')
		{
			//pucData = 0;
			StringLengthOfEveryRow +=
					read_data_byte(
							pucData[read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']) + 1]);
		}
		else
		{
			if ((*pcString >= ' ') && (*pcString) <= '~')
			{
				//
				// Add the width of this character as drawn with the given font.
				//
				StringLengthOfEveryRow +=
						read_data_byte(
								pucData[read_data_word(pusOffset[*pcString - ' ']
												) + 1]);
			}
			else
			{
				//
				// This character does not exist in the font so replace it with
				// a '.' instead.  This matches the approach taken in Grput_string
				// and ensures that the width returned here represents the
				// rendered dimension of the string.
				//
				StringLengthOfEveryRow +=
						read_data_byte(
								pucData[read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']) + 1]);
			}
		}
	}

	if (*Start
			>= 0&& *pcString == 0 && YPull < StringColsHeight_Pixels + read_data_byte(pFont->ucHeight) && LenSelected == false)
	{
		*SelLenReturn = CharCount - *SelStartReturn;
		LenSelected = true;
	}

}

gfx_s32 GI::Screen::String::getStrWidth()
{
	//tDisplay* LcdStruct = (tDisplay *) pDisplay;
	char *pcString = buff;
	const u16 *pusOffset;
	const gfx_u8 *pucData;
	gfx_s32 lWidth;
	int lLength = -1;

	//
	// Check the arguments.
	//
	if (!pFont)
		return 0;
	if (!pcString)
		return 0;

	//
	// Get some pointers to relevant information in the font to make things
	// easier, and give the compiler a hint about extraneous loads that it can
	// avoid.
	//
#ifdef FLASH_DEVICE
	pusOffset = (const u16 *)pFont + 2;
	pucData = (const gfx_u8 *)&pFont->pucData + 2;
#else
	pusOffset = pFont->pusOffset;
	pucData = pFont->pucData;
#endif

	//
	// Loop through the characters in the string.
	//
	for (lWidth = 0; *pcString && lLength; pcString++, lLength--)
	{
		//
		// Get a pointer to the font data for the next character from the
		// string.  If there is not a glyph for the next character, replace it
		// with a ".".
		//
		if (*pcString == '\n')
		{
			//pucData = 0;
			lWidth +=
					read_data_byte(
							pucData[read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']) + 1]);
		}
		if ((*pcString >= ' ') && (*pcString <= '~'))
		{
			//
			// Add the width of this character as drawn with the given font.
			//
			lWidth += read_data_byte(
					pucData[read_data_word(pusOffset[*pcString - ' '] ) + 1]);
		}
		else
		{
			//
			// This character does not exist in the font so replace it with
			// a '.' instead.  This matches the approach taken in Grput_string
			// and ensures that the width returned here represents the
			// rendered dimension of the string.
			//
			lWidth +=
					read_data_byte(
							pucData[read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']) + 1]);
		}
	}

	//
	// Return the width of the string.
	//
	return (lWidth);
}

gfx_s32 GI::Screen::String::getStrRowsNr()
{
	char *pcString = buff;
	int lLength = -1;
	//const u16 *pusOffset;
	//const gfx_u8 *pucData;
	gfx_s32 lRows;

	//
	// Check the arguments.
	//
	if (!pcString)
		return 0;

	//
	// Get some pointers to relevant information in the font to make things
	// easier, and give the compiler a hint about extraneous loads that it can
	// avoid.
	//
	//
	// Loop through the characters in the string.
	//
	for (; *pcString && lLength; pcString++, lLength--)
	{
		//
		// Get a pointer to the font data for the next character from the
		// string.  If there is not a glyph for the next character, replace it
		// with a ".".
		//
		if (*pcString == '\r')
		{
			lRows++;
			//pcString++;
		}
	}
	//
	// Return the number of Rows of the string.
	//
	return (lRows);
}

StringProperties_t GI::Screen::String::getStrProp()
{
	gfx_s32 lLength = -1;
	char *pcString = buff;
	bool WordWrap = wordWrap;
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;
	StringProperties_t StringReturnProperties;

	volatile gfx_s32 FirstRowLength_Pixels = 0;
	volatile gfx_s32 FirstRowLength_Chars = 0;
	volatile gfx_s32 StringLengthWithSpecialChars_Chars = 0;
	volatile gfx_s32 StringLengthWithOutSpecialChars_Chars = 0;
	volatile gfx_s32 StringRowsMaxLength_Pixels = 0;
	//gfx_s32 StringColsHeight_Pixels = 0;
	volatile gfx_s32 StringColsHeight_Rows = 0;
	//gfx_s32 CharHeight_Pixels = 0;

	volatile gfx_u8 EndOfFirstRow = false;

	//u16 *pusOffset;
	//gfx_u8 *pucData;
	//gfx_s32 lRows;
	volatile gfx_s32 StringLengthOfEveryRow = 0;

	//
	// Check the arguments.
	//
	//if(pDisplay) return;
	memset(&StringReturnProperties, 0, sizeof(StringReturnProperties));

	//
	// Get some pointers to relevant information in the font to make things
	// easier, and give the compiler a hint about extraneous loads that it can
	// avoid.
	//
#ifdef FLASH_DEVICE
	const u16 *pusOffset = (const u16 *)pFont + 2;
	const gfx_u8 *pucData = (const gfx_u8 *)&pFont->pucData + 2;
#else
	const u16 *pusOffset = pFont->pusOffset;
	const gfx_u8 *pucData = pFont->pucData;
#endif

	StringReturnProperties.FirstRowLength_Pixels = 0;
	StringReturnProperties.FirstRowLength_Chars = 0;
	//
	// Loop through the characters in the string.
	//
	for (; *pcString && lLength; pcString++, lLength--)
	{
		//
		// Get a pointer to the font data for the next character from the
		// string.  If there is not a glyph for the next character, replace it
		// with a ".".
		//
		if ((StringLengthOfEveryRow
				+ read_data_byte(
						pucData[read_data_word(pusOffset[*pcString - ' '] ) + 1])
				>= (LcdStruct->sClipRegion.sXMax - LcdStruct->sClipRegion.sXMin)
				&& WordWrap == true))
		{
			//pucData = 0;

			if (EndOfFirstRow == false)
			{
				StringReturnProperties.FirstRowLength_Pixels =
						FirstRowLength_Pixels;
				StringReturnProperties.FirstRowLength_Chars =
						FirstRowLength_Chars;
			}
			EndOfFirstRow = true;
			StringColsHeight_Rows++;
			//tringLengthWithOutSpecialChars_Chars--;
			//StringLengthWithSpecialChars_Chars++;
			if (StringLengthOfEveryRow > StringRowsMaxLength_Pixels)
			{
				StringRowsMaxLength_Pixels = StringLengthOfEveryRow;
			}
			StringLengthOfEveryRow = 0;
			//FirstRowLength_Chars--;
			//FirstRowLength_Pixels -= pucData[pusOffset[ABSENT_CHAR_REPLACEMENT - ' '] + 1];
		}
		if (*pcString == '\r')
		{
			//pucData = 0;

			if (EndOfFirstRow == false)
			{
				StringReturnProperties.FirstRowLength_Pixels =
						FirstRowLength_Pixels;
				StringReturnProperties.FirstRowLength_Chars =
						FirstRowLength_Chars;
			}
			EndOfFirstRow = true;
			StringColsHeight_Rows++;
			//StringLengthWithOutSpecialChars_Chars--;
			StringLengthWithSpecialChars_Chars++;
			if (StringLengthOfEveryRow > StringRowsMaxLength_Pixels)
			{
				StringRowsMaxLength_Pixels = StringLengthOfEveryRow;
			}
			StringLengthOfEveryRow = 0;
			//FirstRowLength_Chars--;
			//FirstRowLength_Pixels -= pucData[pusOffset[ABSENT_CHAR_REPLACEMENT - ' '] + 1];
		}
		else if (*pcString == '\n')
		{
			//pucData = 0;
			gfx_u8 CharLen =
					read_data_byte(
							pucData[read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']) + 1]);
			FirstRowLength_Pixels += CharLen;
			StringLengthOfEveryRow += CharLen;
			FirstRowLength_Chars++;
			StringLengthWithOutSpecialChars_Chars++;
			StringLengthWithSpecialChars_Chars++;
		}
		else
		{
			if ((*pcString >= ' ') && (*pcString <= '~'))
			{
				//
				// Add the width of this character as drawn with the given font.
				//
				gfx_u8 CharLen =
						read_data_byte(
								pucData[read_data_word(pusOffset[*pcString - ' ']
												) + 1]);
				FirstRowLength_Pixels += CharLen;
				StringLengthOfEveryRow += CharLen;
				FirstRowLength_Chars++;
				StringLengthWithOutSpecialChars_Chars++;
				StringLengthWithSpecialChars_Chars++;
			}
			else
			{
				//
				// This character does not exist in the font so replace it with
				// a '.' instead.  This matches the approach taken in Grput_string
				// and ensures that the width returned here represents the
				// rendered dimension of the string.
				//
				gfx_u8 CharLen =
						read_data_byte(
								pucData[read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']) + 1]);
				FirstRowLength_Pixels += CharLen;
				StringLengthOfEveryRow += CharLen;
				FirstRowLength_Chars++;
				StringLengthWithOutSpecialChars_Chars++;
				StringLengthWithSpecialChars_Chars++;
			}
		}
	}

	if (StringLengthWithSpecialChars_Chars)
		StringColsHeight_Rows++;

	if (StringLengthOfEveryRow > StringRowsMaxLength_Pixels)
		StringRowsMaxLength_Pixels = StringLengthOfEveryRow;
	StringReturnProperties.CharHeight_Pixels = read_data_byte(pFont->ucHeight);
	StringReturnProperties.StringColsHeight_Pixels = StringColsHeight_Rows
			* read_data_byte(pFont->ucHeight);
	StringReturnProperties.StringColsHeight_Rows = StringColsHeight_Rows;
	StringReturnProperties.StringLengthWithOutSpecialChars_Chars =
			StringLengthWithOutSpecialChars_Chars;
	StringReturnProperties.StringLengthWithSpecialChars_Chars =
			StringLengthWithSpecialChars_Chars;
	StringReturnProperties.StringRowsMaxLength_Pixels =
			StringRowsMaxLength_Pixels;
	//
	// Return the number of Rows of the string.
	//
	return (StringReturnProperties);
}

static gfx_u8 _NumLeadingZeros(gfx_u32 x)
{
	register gfx_u8 count = 0;    //sizeof(x)*8;

	while (x)
	{
		if (!(x & 0xFF000000))
		{
			count = count + 8;
			x = x << 8;
		}
		else
		{
			while (!(x & 0x80000000))
			{
				x = x << 1;
				count++;
			}
			break;
		}
	}

	return count;
}

gfx_s32 GI::Screen::String::drawString()
{
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;
	char *pcString = buff;
	bool WordWrap = wordWrap;
	int lLength = -1;

	gfx_s32 lIdx, lX0, lY0, lCount, lOff, lOn, lBit;
	gfx_s32 lXBackup = lX;
	const u16 *pusOffset;
	const gfx_u8 *pucData;

	bool _ulOpaque = ulOpaque;

	gfx_s32 SelStart = _SelStart;
	gfx_s32 SelLen = _SelLen;

	if (SelLen < 0)
	{
		SelStart -= 0 - SelLen;
		SelLen = 0 - SelLen;
	}

#ifdef FLASH_DEVICE
	pusOffset = (const u16 *)pFont + 2;
	pucData = (const gfx_u8 *)&pFont->pucData + 2;
#else
	pusOffset = pFont->pusOffset;
	pucData = pFont->pucData;
#endif

	//
	// Check the arguments.
	//
	if (!pDisplay || !pFont || !pcString)
		return 0;

	gfx_s32 CharCount = 0;
	gfx_s32 ChCount = 0;

	//
	// Copy the drawing context into a local structure that can be modified.
	//
	//
	// Loop through the characters in the string.
	//
	while (*pcString && lLength--)
	{
		ChCount++;
		if (SelLen)
		{
			if (CharCount >= SelStart && CharCount < SelStart + SelLen)
				_ulOpaque = true;
			else
				_ulOpaque = false;
			CharCount++;
		}

		//
		// Stop drawing the string if the right edge of the clipping region has
		// been exceeded.
		//
		/*if(lX > sCon.sClipRegion.sXMax)
		 {
		 break;
		 }*/

		//
		// Get a pointer to the font data for the next character from the
		// string.  If there is not a glyph for the next character, replace it
		// with a ".".
		//
		if (*pcString == '\r')
		{
			lX = lXBackup;
			pucData = 0;
			pcString++;
		}
		else if (*pcString == '\n')
		{
			lY += read_data_byte(pFont->ucHeight);
			pucData = 0;
			pcString++;
		}
		else
		{
			if ((*pcString >= ' ') && (*pcString <= '~'))
			{
#ifdef FLASH_DEVICE
				pucData = ((const gfx_u8 *)&pFont->pucData + 2 + read_data_word(pusOffset[*pcString++ - ' ']));
#else
				pucData = (pFont->pucData
						+ read_data_word(pusOffset[*pcString++ - ' ']));
#endif
			}
			else
			{
#ifdef FLASH_DEVICE
				pucData = ((const gfx_u8 *)&pFont->pucData + 2 + read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']));
#else
				pucData = (pFont->pucData
						+ read_data_word(
								pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']));
#endif
				pcString++;
			}

			if (lX + read_data_byte(pucData[1])
					>= LcdStruct->sClipRegion.sXMax&& WordWrap == true)
			{
				lX = lXBackup;
				lY += read_data_byte(pFont->ucHeight);
			}
			else
			//
			// See if the entire character is to the left of the clipping region.
			//
			if ((lX + read_data_byte(pucData[1]))
					< LcdStruct->sClipRegion.sXMin)
			{
				//
				// Increment the X coordinate by the width of the character.
				//
				lX += read_data_byte(pucData[1]);

				//
				// Go to the next character in the string.
				//
				continue;
			}

			//
			// Loop through the bytes in the encoded data for this glyph.
			//
			for (lIdx = 2, lX0 = 0, lBit = 0, lY0 = 0;
					lIdx < read_data_byte(pucData[0]);)
			{
				//
				// See if the bottom of the clipping region has been exceeded.
				//
				if ((lY + lY0) > LcdStruct->sClipRegion.sYMax)
				{
					//
					// Stop drawing this character.
					//
					break;
				}

				//
				// See if the font is uncompressed.
				//
				if (read_data_byte(pFont->ucFormat) == FONT_FMT_UNCOMPRESSED)
				{
					//
					// Count the number of off pixels from this position in the
					// glyph image.
					//
					for (lOff = 0; lIdx < read_data_byte(pucData[0]);)
					{
						//
						// Get the number of zero pixels at this position.
						//
						lCount = _NumLeadingZeros(read_data_byte(pucData[lIdx])<< (24 + lBit));

						//
						// If there were more than 8, then it is a "false" result
						// since it counted beyond the end of the current byte.
						// Therefore, simply limit it to the number of pixels
						// remaining in this byte.
						//
						if(lCount > 8)
						{
							lCount = 8 - lBit;
						}

						//
						// Increment the number of off pixels.
						//
						lOff += lCount;

						//
						// Increment the bit position within the byte.
						//
						lBit += lCount;

						//
						// See if the end of the byte has been reached.
						//
						if(lBit == 8)
						{
							//
							// Advance to the next byte and continue counting off
							// pixels.
							//
							lBit = 0;
							lIdx++;
						}
						else
						{
							//
							// Since the end of the byte was not reached, there
							// must be an on pixel.  Therefore, stop counting off
							// pixels.
							//
							break;
						}
					}

					//
					// Count the number of on pixels from this position in the
					// glyph image.
					//
					for(lOn = 0; lIdx < read_data_byte(pucData[0]); )
					{
						//
						// Get the number of one pixels at this location (by
						// inverting the data and counting the number of zeros).
						//
						lCount = _NumLeadingZeros(~(read_data_byte(pucData[lIdx]) << (24 + lBit)));

						//
						// If there were more than 8, then it is a "false" result
						// since it counted beyond the end of the current byte.
						// Therefore, simply limit it to the number of pixels
						// remaining in this byte.
						//
						if(lCount > 8)
						{
							lCount = 8 - lBit;
						}

						//
						// Increment the number of on pixels.
						//
						lOn += lCount;

						//
						// Increment the bit position within the byte.
						//
						lBit += lCount;

						//
						// See if the end of the byte has been reached.
						//
						if(lBit == 8)
						{
							//
							// Advance to the next byte and continue counting on
							// pixels.
							//
							lBit = 0;
							lIdx++;
						}
						else
						{
							//
							// Since the end of the byte was not reached, there
							// must be an off pixel.  Therefore, stop counting on
							// pixels.
							//
							break;
						}
					}
				}

				//
				// Otherwise, the font is compressed with a pixel RLE scheme.
				//
				else
				{
					//
					// See if this is a byte that encodes some on and off pixels.
					//
					if(read_data_byte(pucData[lIdx]))
					{
						//
						// Extract the number of off pixels.
						//
						lOff = (read_data_byte(pucData[lIdx]) >> 4) & 15;

						//
						// Extract the number of on pixels.
						//
						lOn = read_data_byte(pucData[lIdx]) & 15;

						//
						// Skip past this encoded byte.
						//
						lIdx++;
					}

					//
					// Otherwise, see if this is a repeated on pixel byte.
					//
					else if(read_data_byte(pucData[lIdx + 1]) & 0x80)
					{
						//
						// There are no off pixels in this encoding.
						//
						lOff = 0;

						//
						// Extract the number of on pixels.
						//
						lOn = (read_data_byte(pucData[lIdx + 1]) & 0x7f) * 8;

						//
						// Skip past these two encoded bytes.
						//
						lIdx += 2;
					}

					//
					// Otherwise, this is a repeated off pixel byte.
					//
					else
					{
						//
						// Extract the number of off pixels.
						//
						lOff = read_data_byte(pucData[lIdx + 1]) * 8;

						//
						// There are no on pixels in this encoding.
						//
						lOn = 0;

						//
						// Skip past these two encoded bytes.
						//
						lIdx += 2;
					}
				}

				//
				// Loop while there are any off pixels.
				//
				while (lOff)
				{
					//
					// See if the bottom of the clipping region has been exceeded.
					//
					if ((lY + lY0) > LcdStruct->sClipRegion.sYMax)
					{
						//
						// Ignore the remainder of the on pixels.
						//
						break;
					}

					//
					// See if there is more than one on pixel that will fit onto
					// the current row.
					//
					if ((lOff > 1) && ((lX0 + 1) < read_data_byte(pucData[1])))
					{
						//
						// Determine the number of on pixels that will fit on this
						// row.
						//
						lCount = (
								((lX0 + lOff) > read_data_byte(pucData[1])) ?
										read_data_byte(pucData[1])- lX0 :
						lOff);

						//
						// If this row is within the clipping region, draw a
						// horizontal line that corresponds to the sequence of on
						// pixels.
						//
						if(((lY + lY0) >= LcdStruct->sClipRegion.sYMin) && _ulOpaque)
						{
							if(ulVisible == true)
							LcdStruct->drawHLine(lX + lX0, lCount, lY + lY0, 1, background_color);
						}

						//
						// Decrement the count of on pixels by the number on this
						// row.
						//
						lOff -= lCount;

						//
						// Increment the X offset by the number of on pixels.
						//
						lX0 += lCount;
					}

					//
					// Otherwise, there is only a single on pixel that can be
					// drawn.
					//
					else
					{
						//
						// If this pixel is within the clipping region, then draw
						// it.
						//
						if(((lX + lX0) >= LcdStruct->sClipRegion.sXMin) &&
						((lX + lX0) <= LcdStruct->sClipRegion.sXMax) &&
						((lY + lY0) >= LcdStruct->sClipRegion.sYMin) && _ulOpaque == true)
						{
#if (__SIZEOF_INT__ == 2)
							if(ulVisible == true)
							LcdStruct->drawPixel(lX + lX0, lY + lY0, background_color);
#else
							if(ulVisible == true)
							LcdStruct->drawPixel(lX + lX0, lY + lY0, background_color<<8);
#endif
						}

						//
						// Decrement the count of on pixels.
						//
						lOff--;

						//
						// Increment the X offset.
						//
						lX0++;
					}

					//
					// See if the X offset has reached the right side of the
					// character glyph.
					//
					if (lX0 == read_data_byte(pucData[1]))
					{
						//
						// Increment the Y offset.
						//
						lY0++;

						//
						// Reset the X offset to the left side of the character
						// glyph.
						//
						lX0 = 0;
					}
				}

				//
				// Loop while there are any on pixels.
				//
				while (lOn)
				{
					//
					// See if the bottom of the clipping region has been exceeded.
					//
					if ((lY + lY0) > LcdStruct->sClipRegion.sYMax)
					{
						//
						// Ignore the remainder of the on pixels.
						//
						break;
					}

					//
					// See if there is more than one on pixel that will fit onto
					// the current row.
					//
					if ((lOn > 1) && ((lX0 + 1) < read_data_byte(pucData[1])))
					{
						//
						// Determine the number of on pixels that will fit on this
						// row.
						//
						lCount = (
								((lX0 + lOn) > read_data_byte(pucData[1])) ?
										read_data_byte(pucData[1])- lX0 :
						lOn);

						//
						// If this row is within the clipping region, draw a
						// horizontal line that corresponds to the sequence of on
						// pixels.
						//
						if((lY + lY0) >= LcdStruct->sClipRegion.sYMin)
						{
							if(ulVisible == true)
							LcdStruct->drawHLine(lX + lX0, lCount, lY + lY0, 1, foreground_color);
						}

						//
						// Decrement the count of on pixels by the number on this
						// row.
						//
						lOn -= lCount;

						//
						// Increment the X offset by the number of on pixels.
						//
						lX0 += lCount;
					}

					//
					// Otherwise, there is only a single on pixel that can be
					// drawn.
					//
					else
					{
						//
						// If this pixel is within the clipping region, then draw
						// it.
						//
						if(((lX + lX0) >= LcdStruct->sClipRegion.sXMin) &&
						((lX + lX0) <= LcdStruct->sClipRegion.sXMax) &&
						((lY + lY0) >= LcdStruct->sClipRegion.sYMin))
						{
#if (__SIZEOF_INT__ == 2)
							if(ulVisible == true)
							LcdStruct->drawPixel(lX + lX0, lY + lY0, foreground_color);
#else
							if(ulVisible == true)
							LcdStruct->drawPixel(lX + lX0, lY + lY0, foreground_color<<8);
#endif
						}

						//
						// Decrement the count of on pixels.
						//
						lOn--;

						//
						// Increment the X offset.
						//
						lX0++;
					}

					//
					// See if the X offset has reached the right side of the
					// character glyph.
					//
					if (lX0 == read_data_byte(pucData[1]))
					{
						//
						// Increment the Y offset.
						//
						lY0++;

						//
						// Reset the X offset to the left side of the character
						// glyph.
						//
						lX0 = 0;
					}
				}
			}

			//
			// Increment the X coordinate by the width of the character.
			//
			lX += read_data_byte(pucData[1]);
		}
	}
	return ChCount;
}

gfx_s32 GI::Screen::String::drawStringTiny()
{
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;
	//tFont *pFont = properties->pFont;
	char *pcString = buff;
	bool WordWrap = wordWrap;
	int lLength = -1;
	//gfx_s32 _SelStart = properties->_SelStart;
	//gfx_s32 _SelLen = properties->_SelLen;

	//struct Display_Struct* ScreenStruct = (struct Display_Struct*)ScreenFile->udata;
	gfx_s8 chWidth = 0;
	//gfx_s8 chWidth_Tmp = 0;
	gfx_s8 chHeight = 0;
	gfx_s32 CharPtr;
	gfx_s8 Tmp = 0;
	bool CompactWriting = true;
	gfx_s32 Cursor_X = lX;
	gfx_s32 Cursor_Y = lY;
	gfx_s32 CharCnt = 0;
#ifdef FLASH_DEVICE
	chWidth = pgm_read_byte(&CharTable6x8[2]);
	chHeight = pgm_read_byte(&CharTable6x8[3]);
#else
	chWidth = CharTable6x8[2];
	chHeight = CharTable6x8[3];
#endif
	do
	{
		gfx_u8 Char = *pcString;
		if (lLength >= 0 && CharCnt > lLength)
			return CharCnt - 1;
		if (Char == 0)
			return CharCnt - 1;
#ifdef FLASH_DEVICE
		CharPtr = ((Char - pgm_read_byte(&CharTable6x8[4])) * chWidth) + pgm_read_byte(&CharTable6x8[0]);
		if(Char < pgm_read_byte(&CharTable6x8[4]) || Char > pgm_read_byte(&CharTable6x8[5]))
#else
		CharPtr = ((Char - CharTable6x8[4]) * chWidth) + CharTable6x8[0];
		if (Char < CharTable6x8[4] || Char > CharTable6x8[5])
#endif
		{
			//chWidth_Tmp = chWidth;
			chWidth = 0;
		}
		else
		{
			gfx_u8 Temp;
			if (CompactWriting)
			{
				for (Tmp = 1; Tmp < chWidth; Tmp++)
				{
#ifdef FLASH_DEVICE
					Temp = pgm_read_byte(&CharTable6x8[Tmp + CharPtr]);
#else
					Temp = CharTable6x8[Tmp + CharPtr];
#endif
					if (Temp == 0)
						break;
				}
				Tmp++;
			}
			else
			{
				Tmp = chWidth;
			}
			if (Cursor_X + Tmp >= LcdStruct->sClipRegion.sXMin
					&& Cursor_X < LcdStruct->sClipRegion.sXMax + Tmp
					&& Cursor_Y + chHeight >= LcdStruct->sClipRegion.sYMin
					&& Cursor_Y < LcdStruct->sClipRegion.sYMax + chHeight)
			{
				if (ulVisible)
				{
					gfx_s32 XX = 0;
					gfx_s32 YY = 0;
					for (XX = 0; XX < Tmp; XX++)
					{
#ifdef FLASH_DEVICE
						Temp = pgm_read_byte(&CharTable6x8[XX + CharPtr]);
#else
						Temp = CharTable6x8[XX + CharPtr];
#endif
						for (YY = 0; YY < chHeight; YY++)
						{
							if (Temp & 0x1)
							{
								LcdStruct->drawPixel(XX + Cursor_X,
										YY + Cursor_Y, foreground_color);
							}
							else
							{
								if (ulOpaque)
									LcdStruct->drawPixel(XX + Cursor_X,
											YY + Cursor_Y, background_color);
							}
							Temp = Temp >> 1;
						}
					}
				}
			}
		}
		//if(Tmp < chWidth) Tmp++;
		switch (Char)
		{
		case '\r':
			Cursor_X = lX;
			pcString++;
			break;
		case '\n':
			Cursor_Y += chHeight;
			pcString++;
			break;
			//case 9:
			//case 11:
			////signed short TabCursor = (ScreenStruct->Cursor_X/((chWidth_Tmp>>1)*4)*(chWidth_Tmp>>1))*4;
			//Tmp = (((Cursor_X/(chWidth_Tmp<<2))*(chWidth_Tmp<<2))-Cursor_X) + (chWidth_Tmp<<2);
		default:
			Cursor_X += Tmp;
			/*if((ScreenStruct->Cursor_X + chWidth > ScreenStruct->Width) && ScreenStruct->WorldWrap == True)
			 {
			 ScreenStruct->CharWidth = Tmp;
			 ScreenStruct->CharHeight = chHeight;
			 return EOF;
			 }*/
			if ((Cursor_X + chWidth > LcdStruct->sClipRegion.sXMax)
					&& WordWrap == true)
			{
				Cursor_Y += chHeight;
				Cursor_X = lX;
			}
			pcString++;
		}
		CharCnt++;
	} while (1);
}

gfx_s32 GI::Screen::String::getStrPropTiny()
{
	//GScreen::Window *pDisplay = pDisplay;
	char *pcString = buff;
	//bool WordWrap = wordWrap;
	int lLength = -1;
	//tDisplay* LcdStruct = (tDisplay *) pDisplay;
	gfx_s8 chWidth = 0;
	//gfx_s8 chWidth_Tmp = 0;
	gfx_s32 CharPtr;
	bool CompactWriting = true;
	gfx_s32 Cursor_X = 0;
	gfx_s32 CharCnt = 0;
#ifdef FLASH_DEVICE
	chWidth = pgm_read_byte(&CharTable6x8[2]);
#else
	chWidth = CharTable6x8[2];
#endif
	do
	{
		gfx_s8 Tmp = 0;
		gfx_u8 Char = *pcString;
		if (lLength >= 0 && CharCnt > lLength)
			return Cursor_X;
		if (Char == 0)
			return Cursor_X;
#ifdef FLASH_DEVICE
		CharPtr = ((Char - pgm_read_byte(&CharTable6x8[4])) * chWidth) + pgm_read_byte(&CharTable6x8[0]);
		if(Char < pgm_read_byte(&CharTable6x8[4]) || Char > pgm_read_byte(&CharTable6x8[5]))
#else
		CharPtr = ((Char - CharTable6x8[4]) * chWidth) + CharTable6x8[0];
		if (Char < CharTable6x8[4] || Char > CharTable6x8[5])
#endif
		{
			//chWidth_Tmp = chWidth;
			chWidth = 0;
		}
		else
		{
			gfx_u8 Temp;
			if (CompactWriting)
			{
				for (Tmp = 1; Tmp < chWidth; Tmp++)
				{
#ifdef FLASH_DEVICE
					Temp = pgm_read_byte(&CharTable6x8[Tmp + CharPtr]);
#else
					Temp = CharTable6x8[Tmp + CharPtr];
#endif
					if (Temp == 0)
						break;
				}
				Tmp++;
			}
			else
			{
				Tmp = chWidth;
			}
		}
		Cursor_X += Tmp;
		pcString++;
		CharCnt++;
	} while (1);
}
#if 0
/*#####################################################*/
/*
 * Copyright Patrick Powell 1995 & modified by Iulian Gheorghiu
 * This code is based on code written by Patrick Powell (papowell@astart.com)
 * It may be used for any purpose as long as this notice remains intact
 * on all source code distributions
 */

/**************************************************************
 * Original:
 * Patrick Powell Tue Apr 11 09:48:21 PDT 1995
 * A bombproof version of doprnt (dopr) included.
 * Sigh.  This sort of thing is always nasty do deal with.  Note that
 * the version here does not include floating point...
 *
 * snprintf() is used instead of sprintf() as it does limit checks
 * for string length.  This covers a nasty loophole.
 *
 * The other functions are there to prevent NULL pointers from
 * causing nast effects.
 *
 * More Recently:
 *  Brandon Long <blong@fiction.net> 9/15/96 for mutt 0.43
 *  This was ugly.  It is still ugly.  I opted out of floating point
 *  numbers, but the formatter understands just about everything
 *  from the normal C string format, at least as far as I can tell from
 *  the Solaris 2.5 printf(3S) man page.
 *
 *  Brandon Long <blong@fiction.net> 10/22/97 for mutt 0.87.1
 *    Ok, added some minimal floating point support, which means this
 *    probably requires libm on most operating systems.  Don't yet
 *    support the exponent (e,E) and sigfig (g,G).  Also, fmtint()
 *    was pretty badly broken, it just wasn't being exercised in ways
 *    which showed it, so that's been fixed.  Also, formated the code
 *    to mutt conventions, and removed dead code left over from the
 *    original.  Also, there is now a builtin-test, just compile with:
 *           gcc -DTEST_SNPRINTF -o snprintf snprintf.c -lm
 *    and run snprintf for results.
 *
 *  Thomas Roessler <roessler@guug.de> 01/27/98 for mutt 0.89i
 *    The PGP code was using unsigned hexadecimal formats.
 *    Unfortunately, unsigned formats simply didn't work.
 *
 *  Michael Elkins <me@cs.hmc.edu> 03/05/98 for mutt 0.90.8
 *    The original code assumed that both snprintf() and vsnprintf() were
 *    missing.  Some systems only have snprintf() but not vsnprintf(), so
 *    the code is now broken down under HAVE_SNPRINTF and HAVE_VSNPRINTF.
 *
 *  Andrew Tridgell (tridge@samba.org) Oct 1998
 *    fixed handling of %.0f
 *    added test for HAVE_LONG_DOUBLE
 *
 **************************************************************/

/* Version number of package */
#undef VERSION//#include "config.h"

#if !defined(HAVE_SNPRINTF) || !defined(HAVE_VSNPRINTF)

#include <string.h>
#include <ctype.h>
#ifndef USE_AVR_STUDIO
#include <sys/types.h>
#endif


/* Define this as a fall through, HAVE_STDARG_H is probably already set */

#define HAVE_VARARGS_H


/* varargs declarations: */
#define HAVE_STDARG_H

#if defined(HAVE_STDARG_H)
# include "stdarg.h"
# define HAVE_STDARGS    /* let's hope that works everywhere (mj) */
# define VA_LOCAL_DECL   va_list ap
# define VA_START(f)     va_start(ap, f)
# define VA_SHIFT(v,t)  ;   /* no-op for ANSI */
# define VA_END          va_end(ap)
#else
# if defined(HAVE_VARARGS_H)
#  include "varargs.h"
#  undef HAVE_STDARGS
#  define VA_LOCAL_DECL   va_list ap
#  define VA_START(f)     va_start(ap)      /* f is ignored! */
#  define VA_SHIFT(v,t) v = va_arg(ap,t)
#  define VA_END        va_end(ap)
# else
/*XX ** NO VARARGS ** XX*/
# endif
#endif

/*
 * dopr(): poor man's version of doprintf
 */

/* format read states */
#define DP_S_DEFAULT 0
#define DP_S_FLAGS   1
#define DP_S_MIN     2
#define DP_S_DOT     3
#define DP_S_MAX     4
#define DP_S_MOD     5
#define DP_S_CONV    6
#define DP_S_DONE    7

/* format flags - Bits */
#define DP_F_MINUS 	(1 << 0)
#define DP_F_PLUS  	(1 << 1)
#define DP_F_SPACE 	(1 << 2)
#define DP_F_NUM   	(1 << 3)
#define DP_F_ZERO  	(1 << 4)
#define DP_F_UP    	(1 << 5)
#define DP_F_UNSIGNED 	(1 << 6)

/* Conversion Flags */
#define DP_C_SHORT   1
#define DP_C_LONG    2
#define DP_C_LDOUBLE 3

#define char_to_int(p) (p - '0')
#define MAX(p,q) ((p >= q) ? p : q)

void GI::Screen::String::dopr (char *buffer, size_t maxlen, const char *format, va_list args)
{
  int ch;
  long value;
  LDOUBLE fvalue;
  char *strvalue;
  int min;
  int max;
  int state;
  int flags;
  int cflags;
  size_t currlen;

  state = DP_S_DEFAULT;
  currlen = flags = cflags = min = 0;
  max = -1;
  ch = *format++;

  while (state != DP_S_DONE)
  {
    if ((ch == '\0') || (currlen >= maxlen))
      state = DP_S_DONE;

    switch(state)
    {
    case DP_S_DEFAULT:
      if (ch == '%')
	state = DP_S_FLAGS;
      else
	dopr_outch (buffer, &currlen, maxlen, ch);
      ch = *format++;
      break;
    case DP_S_FLAGS:
      switch (ch)
      {
      case '-':
	flags |= DP_F_MINUS;
        ch = *format++;
	break;
      case '+':
	flags |= DP_F_PLUS;
        ch = *format++;
	break;
      case ' ':
	flags |= DP_F_SPACE;
        ch = *format++;
	break;
      case '#':
	flags |= DP_F_NUM;
        ch = *format++;
	break;
      case '0':
	flags |= DP_F_ZERO;
        ch = *format++;
	break;
      default:
	state = DP_S_MIN;
	break;
      }
      break;
    case DP_S_MIN:
      if (isdigit(ch))
      {
	min = 10*min + char_to_int (ch);
	ch = *format++;
      }
      else if (ch == '*')
      {
	min = va_arg (args, int);
	ch = *format++;
	state = DP_S_DOT;
      }
      else
	state = DP_S_DOT;
      break;
    case DP_S_DOT:
      if (ch == '.')
      {
	state = DP_S_MAX;
	ch = *format++;
      }
      else
	state = DP_S_MOD;
      break;
    case DP_S_MAX:
      if (isdigit(ch))
      {
	if (max < 0)
	  max = 0;
	max = 10*max + char_to_int (ch);
	ch = *format++;
      }
      else if (ch == '*')
      {
	max = va_arg (args, int);
	ch = *format++;
	state = DP_S_MOD;
      }
      else
	state = DP_S_MOD;
      break;
    case DP_S_MOD:
      /* Currently, we don't support Long Long, bummer */
      switch (ch)
      {
      case 'h':
	cflags = DP_C_SHORT;
	ch = *format++;
	break;
      case 'l':
	cflags = DP_C_LONG;
	ch = *format++;
	break;
      case 'L':
	cflags = DP_C_LDOUBLE;
	ch = *format++;
	break;
      default:
	break;
      }
      state = DP_S_CONV;
      break;
    case DP_S_CONV:
      switch (ch)
      {
      case 'd':
      case 'i':
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, short);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, long);
	else
	  value = va_arg (args, int);
	fmtint (buffer, &currlen, maxlen, value, 10, min, max, flags);
	break;
      case 'o':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, u16);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, unsigned long);
	else
	  value = va_arg (args, gfx_u32);
	fmtint (buffer, &currlen, maxlen, value, 8, min, max, flags);
	break;
      case 'u':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, u16);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, unsigned long);
	else
	  value = va_arg (args, gfx_u32);
	fmtint (buffer, &currlen, maxlen, value, 10, min, max, flags);
	break;
      case 'X':
	flags |= DP_F_UP;
      case 'x':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, u16);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, unsigned long);
	else
	  value = va_arg (args, gfx_u32);
	fmtint (buffer, &currlen, maxlen, value, 16, min, max, flags);
	break;
      case 'f':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	/* um, floating point? */
	fmtfp (buffer, &currlen, maxlen, fvalue, min, max, flags);
	break;
      case 'E':
	flags |= DP_F_UP;
      case 'e':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	break;
      case 'G':
	flags |= DP_F_UP;
      case 'g':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	break;
      case 'c':
	dopr_outch (buffer, &currlen, maxlen, va_arg (args, int));
	break;
      case 's':
	strvalue = va_arg (args, char *);
	if (max < 0)
	  max = maxlen; /* ie, no max */
	fmtstr (buffer, &currlen, maxlen, strvalue, flags, min, max);
	break;
      case 'p':
	strvalue = (char *)va_arg (args, void *);
	fmtint (buffer, &currlen, maxlen, (int)strvalue, 16, min, max, flags);
	break;
      case 'n':
	if (cflags == DP_C_SHORT)
	{
	  short int *num;
	  num = va_arg (args, short *);
	  *num = currlen;
        }
	else if (cflags == DP_C_LONG)
	{
	  long int *num;
	  num = va_arg (args, long *);
	  *num = currlen;
        }
	else
	{
	  int *num;
	  num = va_arg (args, int *);
	  *num = currlen;
        }
	break;
      case '%':
	dopr_outch (buffer, &currlen, maxlen, ch);
	break;
      case 'w':
	/* not supported yet, treat as next char */
	ch = *format++;
	break;
      default:
	/* Unknown, skip */
	break;
      }
      ch = *format++;
      state = DP_S_DEFAULT;
      flags = cflags = min = 0;
      max = -1;
      break;
    case DP_S_DONE:
      break;
    default:
      /* hmm? */
      break; /* some picky compilers need this */
    }
  }
  if (currlen < maxlen - 1)
    buffer[currlen] = '\0';
  else
    buffer[maxlen - 1] = '\0';
}

void GI::Screen::String::fmtstr (char *buffer, size_t *currlen, size_t maxlen,
		    char *value, int flags, int min, int max)
{
  int padlen, strln;     /* amount to pad */
  int cnt = 0;

  if (value == 0)
  {
    value = (char *)"<NULL>";
  }

  for (strln = 0; value[strln]; ++strln); /* strlen */
  padlen = min - strln;
  if (padlen < 0)
    padlen = 0;
  if (flags & DP_F_MINUS)
    padlen = -padlen; /* Left Justify */

  while ((padlen > 0) && (cnt < max))
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    --padlen;
    ++cnt;
  }
  while (*value && (cnt < max))
  {
    dopr_outch (buffer, currlen, maxlen, *value++);
    ++cnt;
  }
  while ((padlen < 0) && (cnt < max))
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    ++padlen;
    ++cnt;
  }
}

/* Have to handle DP_F_NUM (ie 0x and 0 alternates) */

void GI::Screen::String::fmtint (char *buffer, size_t *currlen, size_t maxlen,
		    long value, int base, int min, int max, int flags)
{
  int signvalue = 0;
  unsigned long uvalue;
  char convert[20];
  int place = 0;
  int spadlen = 0; /* amount to space pad */
  int zpadlen = 0; /* amount to zero pad */
  int caps = 0;

  if (max < 0)
    max = 0;

  uvalue = value;

  if(!(flags & DP_F_UNSIGNED))
  {
    if( value < 0 ) {
      signvalue = '-';
      uvalue = -value;
    }
    else
      if (flags & DP_F_PLUS)  /* Do a sign (+/i) */
	signvalue = '+';
    else
      if (flags & DP_F_SPACE)
	signvalue = ' ';
  }

  if (flags & DP_F_UP) caps = 1; /* Should characters be upper case? */

  do {
    convert[place++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")
      [uvalue % (unsigned)base  ];
    uvalue = (uvalue / (unsigned)base );
  } while(uvalue && (place < 20));
  if (place == 20) place--;
  convert[place] = 0;

  zpadlen = max - place;
  spadlen = min - MAX (max, place) - (signvalue ? 1 : 0);
  if (zpadlen < 0) zpadlen = 0;
  if (spadlen < 0) spadlen = 0;
  if (flags & DP_F_ZERO)
  {
    zpadlen = MAX(zpadlen, spadlen);
    spadlen = 0;
  }
  if (flags & DP_F_MINUS)
    spadlen = -spadlen; /* Left Justifty */

#ifdef DEBUG_SNPRINTF
  dprint (1, (debugfile, "zpad: %d, spad: %d, min: %d, max: %d, place: %d\n",
      zpadlen, spadlen, min, max, place));
#endif

  /* Spaces */
  while (spadlen > 0)
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    --spadlen;
  }

  /* Sign */
  if (signvalue)
    dopr_outch (buffer, currlen, maxlen, signvalue);

  /* Zeros */
  if (zpadlen > 0)
  {
    while (zpadlen > 0)
    {
      dopr_outch (buffer, currlen, maxlen, '0');
      --zpadlen;
    }
  }

  /* Digits */
  while (place > 0)
    dopr_outch (buffer, currlen, maxlen, convert[--place]);

  /* Left Justified spaces */
  while (spadlen < 0) {
    dopr_outch (buffer, currlen, maxlen, ' ');
    ++spadlen;
  }
}

static LDOUBLE abs_val (LDOUBLE value)
{
  LDOUBLE result = value;

  if (value < 0)
    result = -value;

  return result;
}

static LDOUBLE _pow10_ (int exp)
{
  LDOUBLE result = 1;

  while (exp)
  {
    result *= 10;
    exp--;
  }

  return result;
}

static long _round_ (LDOUBLE value)
{
  long intpart;

  intpart = value;
  value = value - intpart;
  if (value >= 0.5)
    intpart++;

  return intpart;
}

void GI::Screen::String::fmtfp (char *buffer, size_t *currlen, size_t maxlen,
		   LDOUBLE fvalue, int min, int max, int flags)
{
  int signvalue = 0;
  LDOUBLE ufvalue;
  char iconvert[20];
  char fconvert[20];
  int iplace = 0;
  int fplace = 0;
  int padlen = 0; /* amount to pad */
  int zpadlen = 0;
  int caps = 0;
  long intpart;
  long fracpart;

  /*
   * AIX manpage says the default is 0, but Solaris says the default
   * is 6, and sprintf on AIX defaults to 6
   */
  if (max < 0)
    max = 6;

  ufvalue = abs_val (fvalue);

  if (fvalue < 0)
    signvalue = '-';
  else
    if (flags & DP_F_PLUS)  /* Do a sign (+/i) */
      signvalue = '+';
    else
      if (flags & DP_F_SPACE)
	signvalue = ' ';

#if 0
  if (flags & DP_F_UP) caps = 1; /* Should characters be upper case? */
#endif

  intpart = ufvalue;

  /*
   * Sorry, we only support 9 digits past the decimal because of our
   * conversion method
   */
  if (max > 9)
    max = 9;

  /* We "cheat" by converting the fractional part to integer by
   * multiplying by a factor of 10
   */
  fracpart = _round_ ((_pow10_ (max)) * (ufvalue - intpart));

  if (fracpart >= _pow10_ (max))
  {
    intpart++;
    fracpart -= _pow10_ (max);
  }

#ifdef DEBUG_SNPRINTF
  dprint (1, (debugfile, "fmtfp: %f =? %d.%d\n", fvalue, intpart, fracpart));
#endif

  /* Convert integer part */
  do {
    iconvert[iplace++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")[intpart % 10];
    intpart = (intpart / 10);
  } while(intpart && (iplace < 20));
  if (iplace == 20) iplace--;
  iconvert[iplace] = 0;

  /* Convert fractional part */
  do {
    fconvert[fplace++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")[fracpart % 10];
    fracpart = (fracpart / 10);
  } while(fracpart && (fplace < 20));
  if (fplace == 20) fplace--;
  fconvert[fplace] = 0;

  /* -1 for decimal point, another -1 if we are printing a sign */
  padlen = min - iplace - max - 1 - ((signvalue) ? 1 : 0);
  zpadlen = max - fplace;
  if (zpadlen < 0)
    zpadlen = 0;
  if (padlen < 0)
    padlen = 0;
  if (flags & DP_F_MINUS)
    padlen = -padlen; /* Left Justifty */

  if ((flags & DP_F_ZERO) && (padlen > 0))
  {
    if (signvalue)
    {
      dopr_outch (buffer, currlen, maxlen, signvalue);
      --padlen;
      signvalue = 0;
    }
    while (padlen > 0)
    {
      dopr_outch (buffer, currlen, maxlen, '0');
      --padlen;
    }
  }
  while (padlen > 0)
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    --padlen;
  }
  if (signvalue)
    dopr_outch (buffer, currlen, maxlen, signvalue);

  while (iplace > 0)
    dopr_outch (buffer, currlen, maxlen, iconvert[--iplace]);

  /*
   * Decimal point.  This should probably use locale to find the correct
   * char to print out.
   */
  if (max > 0)
  {
    dopr_outch (buffer, currlen, maxlen, '.');

    while (fplace > 0)
      dopr_outch (buffer, currlen, maxlen, fconvert[--fplace]);
  }

  while (zpadlen > 0)
  {
    dopr_outch (buffer, currlen, maxlen, '0');
    --zpadlen;
  }

  while (padlen < 0)
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    ++padlen;
  }
}

void GI::Screen::String::dopr_outch (char *buffer, size_t *currlen, size_t maxlen, char c)
{
  /*if (*currlen < maxlen)
    buffer[(*currlen)++] = c;*/
	append((gfx_u8)c);
}

int GI::Screen::String::vsnprintf (char *str, size_t count, const char *fmt, va_list args)
{
  str[0] = 0;
  dopr(str, count, fmt, args);
  return(strlen(str));
}
/* VARARGS3 */
/*
 * Source of this description is : http://www.cplusplus.com/reference/cstdio/printf/
 * specifier	output
 *
 * d or i		Signed decimal integer																392
 * u			Unsigned decimal integer															7235
 * o			Unsigned octal																		610
 * x			Unsigned hexadecimal integer														7fa
 * X			Unsigned hexadecimal integer (uppercase)											7FA
 * f			Decimal floating point, lowercase													392.65
 * F			Decimal floating point, uppercase													392.65
 * e			Scientific notation (mantissa/exponent), lowercase									3.9265e+2
 * E			Scientific notation (mantissa/exponent), uppercase									3.9265E+2
 * g			Use the shortest representation: %e or %f											392.65
 * G			Use the shortest representation: %E or %F											392.65
 * a			Hexadecimal floating point, lowercase												-0xc.90fep-2
 * A			Hexadecimal floating point, uppercase												-0XC.90FEP-2
 * c			Character																			a
 * s			String of characters																sample
 * p			Pointer address																		b8000000
 * n			Nothing printed.
 * 					The corresponding argument must be a pointer to a gfx_s32.
 * 					The number of characters written so far is stored in the pointed location.
 * %			A % followed by another % character will write a single % to the stream.			%
 *
 *
 *
 *
 *The format specifier can also contain sub-specifiers: flags, width, .precision and modifiers (in that order), which are optional and follow these specifications:

 * flags		description
 * -			Left-justify within the given field width; Right justification is the default (see width sub-specifier).
 * +			Forces to preceed the result with a plus or minus sign (+ or -) even for positive numbers. By default, only negative numbers are preceded with a - sign.
 * (space)		If no sign is going to be written, a blank space is inserted before the value.
 * #			Used with o, x or X specifiers the value is preceeded with 0, 0x or 0X respectively for values different than zero.
 * 					Used with a, A, e, E, f, F, g or G it forces the written output to contain a decimal point even if no more digits follow. By default, if no digits follow, no decimal point is written.
 * 0			Left-pads the number with zeroes (0) instead of spaces when padding is specified (see width sub-specifier).
 *
 * width		description
 * (number)		Minimum number of characters to be printed. If the value to be printed is shorter than this number, the result is padded with blank spaces. The value is not truncated even if the result is larger.
 * *			The width is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.
 *
 * .precision	description
 * .number		For integer specifiers (d, i, o, u, x, X): precision specifies the minimum number of digits to be written. If the value to be written is shorter than this number, the result is padded with leading zeros. The value is not truncated even if the result is longer. A precision of 0 means that no character is written for the value 0.
 * 					For a, A, e, E, f and F specifiers: this is the number of digits to be printed after the decimal point (by default, this is 6).
 * 					For g and G specifiers: This is the maximum number of significant digits to be printed.
 * 					For s: this is the maximum number of characters to be printed. By default all characters are printed until the ending null character is encountered.
 * 					If the period is specified without an explicit value for precision, 0 is assumed.
 * .*			The precision is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.
 *
 *
 *
 *
 * The length sub-specifier modifies the length of the data type. This is a chart showing the types used to interpret the corresponding arguments with and without length specifier (if a different type is used, the proper type promotion or conversion is performed, if allowed):
 * 											specifiers
 * length		d i				u o x X						f F e E g G a A				c			s			p			n
 * (none)		int				gfx_u32				double						int			char*		void*		int*
 * hh			gfx_s8		gfx_u8																				gfx_s8*
 * h			short int		u16 int																			short int*
 * l			long int		unsigned long int										wint_t		wchar_t*				long int*
 * ll			long long int	unsigned long long int																		long long int*
 * j			intmax_t		uintmax_t																					intmax_t*
 * z			size_t			size_t																						size_t*
 * t			ptrdiff_t		ptrdiff_t																					ptrdiff_t*
 * L														long double
 *
 *
 * Note regarding the c specifier: it takes an int (or wint_t) as argument, but performs the proper conversion to a char value (or a wchar_t) before formatting it for output.
 *
 * Example
 *
 *  uart_printf example
 *    uart_printf ("Characters: %c %c \n", 'a', 65);
 *    uart_printf ("Decimals: %d %ld\n", 1977, 650000L);
 *    uart_printf ("Preceding with blanks: %10d \n", 1977);
 *    uart_printf ("Preceding with zeros: %010d \n", 1977);
 *    uart_printf ("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
 *    uart_printf ("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
 *    uart_printf ("Width trick: %*d \n", 5, 10);
 *    uart_printf ("%s \n", "A string");
 *
 * Output:
 *
 *
 * Characters: a A
 * Decimals: 1977 650000
 * Preceding with blanks:       1977
 * Preceding with zeros: 0000001977
 * Some different radices: 100 64 144 0x64 0144
 * floats: 3.14 +3e+000 3.141600E+000
 * Width trick:    10
 * A string
 *
 */
#endif /* !HAVE_SNPRINTF */

#endif

void GI::Screen::String::appendF(const char *pcString, ...)
{
	va_list args;
	va_start (args, pcString);
	char buff[1];
	s32 len = vsnprintf(buff, 0, pcString, args);
	va_end (args);
	if(len > 0)
	{
		char *tmp_str = (char *)malloc(len + 1);
		if(tmp_str)
		{
			va_start (args, pcString);
			vsnprintf(tmp_str, len + 1, pcString, args);
			va_end (args);
			append(tmp_str);
			free(tmp_str);
		}
	}
}

void GI::Screen::String::appendF(GI::String *string, ...)
{
	va_list args;
	char *pcString = string->buff;
	va_start (args, pcString);
	char buff[1];
	s32 len = vsnprintf(buff, 0, pcString, args);
	va_end (args);
	if(len > 0)
	{
		char *tmp_str = (char *)malloc(len + 1);
		if(tmp_str)
		{
			va_start (args, pcString);
			vsnprintf(tmp_str, len + 1, pcString, args);
			va_end (args);
			append(tmp_str);
			free(tmp_str);
		}
	}
}

void GI::Screen::String::appendF(GI::Screen::String *string, ...)
{
	va_list args;
	char *pcString = string->buff;
	va_start (args, pcString);
	char buff[1];
	s32 len = vsnprintf(buff, 0, pcString, args);
	va_end (args);
	if(len > 0)
	{
		char *tmp_str = (char *)malloc(len + 1);
		if(tmp_str)
		{
			va_start (args, pcString);
			vsnprintf(tmp_str, len + 1, pcString, args);
			va_end (args);
			append(tmp_str);
			free(tmp_str);
		}
	}
}


void GI::Screen::String::setTextF(const char *pcString, ...)
{
	clear();
	va_list args;
	va_start (args, pcString);
	char buff[1];
	s32 len = vsnprintf(buff, 0, pcString, args);
	va_end (args);
	if(len > 0)
	{
		char *tmp_str = (char *)malloc(len + 1);
		if(tmp_str)
		{
			va_start (args, pcString);
			vsnprintf(tmp_str, len + 1, pcString, args);
			va_end (args);
			setText(tmp_str);
			free(tmp_str);
		}
	}
}

void GI::Screen::String::setTextF(GI::String *string, ...)
{
	clear();
	va_list args;
	char *pcString = string->buff;
	va_start (args, pcString);
	char buff[1];
	s32 len = vsnprintf(buff, 0, pcString, args);
	va_end (args);
	if(len > 0)
	{
		char *tmp_str = (char *)malloc(len + 1);
		if(tmp_str)
		{
			va_start (args, pcString);
			vsnprintf(tmp_str, len + 1, pcString, args);
			va_end (args);
			setText(tmp_str);
			free(tmp_str);
		}
	}
}

void GI::Screen::String::setTextF(GI::Screen::String *string, ...)
{
	clear();
	va_list args;
	char *pcString = string->buff;
	va_start (args, pcString);
	char buff[1];
	s32 len = vsnprintf(buff, 0, pcString, args);
	va_end (args);
	if(len > 0)
	{
		char *tmp_str = (char *)malloc(len + 1);
		if(tmp_str)
		{
			va_start (args, pcString);
			vsnprintf(tmp_str, len + 1, pcString, args);
			va_end (args);
			setText(tmp_str);
			free(tmp_str);
		}
	}
}
