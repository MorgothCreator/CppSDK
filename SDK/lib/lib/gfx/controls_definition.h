/*
 *  lib/gfx/controls_definition.h
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

#ifndef CONTROLS_DEFINITION_H_
#define CONTROLS_DEFINITION_H_
/*#####################################################*/
#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#endif
#include <stdbool.h>
#include <string.h>
#include "sys/cache.h"
#include "interface/screen.h"
#include "api/lcd_def.h"
#include <api/timer_api.h>
/*#####################################################*/
#define FONT_FMT_UNCOMPRESSED   0x00
#define ABSENT_CHAR_REPLACEMENT '.'
/*#####################################################*/
#ifndef INT_SIZE_IS_2_BYTES
//#if 4 == sizeof(unsigned int)
#define ReturnCommand_gp  (unsigned)(29)
#define ReturnCommand_gm  (unsigned)(0x07 << ReturnCommand_gp)
#define ReturnCommand_NoCommand  (unsigned)(0x00<<ReturnCommand_gp)
#define ReturnCommand_GoUp  (unsigned)(0x01<<ReturnCommand_gp)
#define ReturnCommand_GoDn  (unsigned)(0x02<<ReturnCommand_gp)
#define ReturnCommand_GoLeft  (unsigned)(0x03<<ReturnCommand_gp)
#define ReturnCommand_GoRight  (unsigned)(0x04<<ReturnCommand_gp)
#define ReturnCommand_MaxValue  (unsigned)(0x08)
#else
#define ReturnCommand_gp  (unsigned)(13)
#define ReturnCommand_gm  (unsigned)(0x07 << ReturnCommand_gp)
#define ReturnCommand_NoCommand  (unsigned)(0x00<<ReturnCommand_gp)
#define ReturnCommand_GoUp  (unsigned)(0x01<<ReturnCommand_gp)
#define ReturnCommand_GoDn  (unsigned)(0x02<<ReturnCommand_gp)
#define ReturnCommand_GoLeft  (unsigned)(0x03<<ReturnCommand_gp)
#define ReturnCommand_GoRight  (unsigned)(0x04<<ReturnCommand_gp)
#define ReturnCommand_MaxValue  (unsigned)(0x08)
#endif
/*#####################################################*/
#define BORDER_LINE_ONE_LIGHT		2
#define BORDER_LINE_TWO_LIGHT		1.5
#define BORDER_LINE_ONE_DARK		3
#define BORDER_LINE_TWO_DARK		2
/*#####################################################*/
typedef unsigned int colors;

typedef struct
{
	unsigned int Control_Color_Enabled_Ink_Pull;
	unsigned int Control_Color_Enabled_Ink_Push;
	unsigned int Control_Color_Enabled_Buton_Pull;
	unsigned int Control_Color_Enabled_Buton_Push;
	unsigned int Control_Color_Enabled_Border_Pull;
	unsigned int Control_Color_Enabled_Border_Push;
	unsigned int Control_Color_Enabled_BackGround;
	unsigned int Control_Color_Enabled_TextBackGround;
	unsigned int Control_Color_Enabled_WindowBorder;
	unsigned int Control_Color_Enabled_WindowHeader;

	unsigned int Control_Color_Disabled_Ink_Pull;
	//unsigned int Control_Color_Disabled_Ink_Push;
	unsigned int Control_Color_Disabled_Buton_Pull;
	//unsigned int Control_Color_Disabled_Buton_Push;
	unsigned int Control_Color_Disabled_Border_Pull;
	//unsigned int Control_Color_Disabled_Border_Push;
	unsigned int Control_Color_Disabled_BackGround;
	unsigned int Control_Color_Disabled_WindowBorder;
	unsigned int Control_Color_Disabled_WindowHeader;
	unsigned int Scren;
	tFont* DefaultFont;
}controlscolor;
/*#####################################################*/
typedef enum
{
	Control_Nop,
	Control_Refresh,
	Control_Entire_Refresh,
	Control_Entire_Repaint,
	Control_Add_Children,
	Control_Init_Childrens,

	Control_Set_Cursor_Data,
	Control_Set_Position_X,
	Control_Set_Position_Y,
	Control_Set_Size_X,
	Control_Set_Size_Y,
	Control_Set_Caption_Text,
	Control_Set_Caption_Align,
	Control_Set_Font,
	Control_Set_WordWrap,
	Control_Set_Color_Enabled_Ink_Pull,
	Control_Set_Color_Enabled_Ink_Push,
	Control_Set_Color_Enabled_Buton_Pull,
	Control_Set_Color_Enabled_Buton_Push,
	Control_Set_Color_Enabled_Border_Pull,
	Control_Set_Color_Enabled_Border_Push,
	Control_Set_Color_Enabled_BackGround,

	Control_Set_Color_Disabled_Ink_Pull,
	Control_Set_Color_Disabled_Ink_Push,
	Control_Set_Color_Disabled_Buton_Pull,
	Control_Set_Color_Disabled_Buton_Push,
	Control_Set_Color_Disabled_Border,
	Control_Set_Color_Disabled_BackGround,
	Control_Set_Color_Disabled_ScalerOnDisabled,
	Control_Set_Color_Screen,
	Control_Set_Enabled,
	Control_Set_Visible,
	Control_Set_StateChangedOn,
	Control_Set_Value,
	Control_Set_MinimValue,
	Control_Set_MaximValue,

	Control_Set_CallBackOnDownData,
	Control_Set_CallBackOnDownFunction,
	Control_Set_CallBackOnUpData,
	Control_Set_CallBackOnUpFunction,
	Control_Set_CallBackOnMoveData,
	Control_Set_CallBackOnMoveFunction,
	Control_Set_CallBackOnValueChangedData,
	Control_Set_CallBackOnValueChangedFunction,
	Control_Set_CallBackOnCheckedData,
	Control_Set_CallBackOnCheckedFunction,
	Control_Set_CallBackOnUnCheckedData,
	Control_Set_CallBackOnUnCheckedFunction
}ControlCommands;
/*#####################################################*/
typedef enum
{
	Cursor_NoAction,
	Cursor_Up,
	Cursor_Down,
	Cursor_Move
}CursorState;
/*#####################################################*/
typedef struct
{
	signed int X;
	signed int Y;
	unsigned int Data;
	void* Settings;
	bool CursorCoordonateUsed;
	bool WindowRefresh;
	ControlCommands Comand;
	CursorState Cursor;
}tControlCommandData;
/*#####################################################*/
typedef struct
{
	struct
	{
		unsigned int Ink;
		unsigned int Buton;
		unsigned int Border;
		unsigned int BackGround;
		double ScalerOnDisabled;
	}Disabled;
	struct
	{
		struct
		{
			unsigned int Push;
			unsigned int Pull;
		}Ink;
		struct
		{
			unsigned int Push;
			unsigned int Pull;
		}Buton;
		struct
		{
			unsigned int Push;
			unsigned int Pull;
		}Border;
		unsigned int TextBackGround;
		unsigned int BackGround;
		double ScalerOnDisabled;
	}Enabled;
	unsigned int Scren;
}controls_color_struct_t;
/*#####################################################*/
/*typedef struct
{
	Align TextAlign;
	String_t *Text;
	bool WordWrap;
	tFont* Font;
}controls_caption_t;*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
extern controlscolor controls_color;
/*#####################################################*/
typedef struct
{
	struct
	{
		signed int X;
		signed int Y;
	}Position;
	struct
	{
		signed int X;
		signed int Y;
	}Size;
	struct
	{
		unsigned int Background;
		unsigned int Scren;
	}Color;
	bool Visible;
	bool Enabled;
	struct
	{
		unsigned int ItemsNumber;
		void** List;
	}Children;
	struct
	{
		struct
		{
			bool Initiated;
		}Control;
	}Internals;
}tPanel;
/*#####################################################*/
unsigned int controlls_change_color(unsigned int color, double value);
bool screen_copy(void *pDisplayTo, void *pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color);
void TouchPaintPoint(void *pDisplay, signed int X, signed int Y, unsigned int color);
void put_circle(void *pDisplay, signed int x, signed int y, signed int _radius, unsigned char fill, unsigned int color);
void put_line(void *pDisplay, signed int X1, signed int Y1, signed int X2, signed int Y2, unsigned char width, unsigned int color);
void put_elipse(void *pDisplay, signed int xc,signed int yc,signed int _rx,signed int _ry, unsigned char Fill, unsigned int color);
void put_triangle(void *pDisplay, signed int  Ax,signed int  Ay,signed int  Bx,signed int  By,signed int  Cx,signed int  Cy, unsigned char Fill, unsigned int color);


/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "controls_definition.c"
#endif
/*#####################################################*/
#endif /* CONTROLS_DEFINITION_H_ */
/*#####################################################*/
