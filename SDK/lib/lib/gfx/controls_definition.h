/*
 *  lib/gfx/controls_definition.h
 */

#ifndef CONTROLS_DEFINITION_H_
#define CONTROLS_DEFINITION_H_
/*#####################################################*/
#ifdef __AVR_XMEGA__
#include <avr/pgmspace.h>
#endif
#include <stdbool.h>
#include <string.h>
#include "sys/cache.h"
#include "api/lcd_def.h"
#include <api/timer.h>
#include "gfxdef.h"
/*#####################################################*/
#define FONT_FMT_UNCOMPRESSED   0x00
#define ABSENT_CHAR_REPLACEMENT '.'
/*#####################################################*/
/*#if (__SIZEOF_INT__ == 4)
//#if 4 == sizeof(gfx_u32)
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
#endif*/
/*#####################################################*/
#define BORDER_LINE_ONE_LIGHT		2
#define BORDER_LINE_TWO_LIGHT		1.5
#define BORDER_LINE_ONE_DARK		3
#define BORDER_LINE_TWO_DARK		2
/*#####################################################*/
typedef gfx_u32 colors;

typedef struct
{
	gfx_u32 Control_Color_Enabled_Ink_Pull;
	gfx_u32 Control_Color_Enabled_Ink_Push;
	gfx_u32 Control_Color_Enabled_Buton_Pull;
	gfx_u32 Control_Color_Enabled_Buton_Push;
	gfx_u32 Control_Color_Enabled_Border_Pull;
	gfx_u32 Control_Color_Enabled_Border_Push;
	gfx_u32 Control_Color_Enabled_BackGround;
	gfx_u32 Control_Color_Enabled_TextBackGround;
	gfx_u32 Control_Color_Enabled_WindowBorder;
	gfx_u32 Control_Color_Enabled_WindowHeader;

	gfx_u32 Control_Color_Disabled_Ink_Pull;
	//gfx_u32 Control_Color_Disabled_Ink_Push;
	gfx_u32 Control_Color_Disabled_Buton_Pull;
	//gfx_u32 Control_Color_Disabled_Buton_Push;
	gfx_u32 Control_Color_Disabled_Border_Pull;
	//gfx_u32 Control_Color_Disabled_Border_Push;
	gfx_u32 Control_Color_Disabled_BackGround;
	gfx_u32 Control_Color_Disabled_WindowBorder;
	gfx_u32 Control_Color_Disabled_WindowHeader;
	gfx_u32 Scren;
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
	gfx_s32 X;
	gfx_s32 Y;
	gfx_s32 X2;
	gfx_s32 Y2;
	gfx_s32 X3;
	gfx_s32 Y3;
	gfx_s32 X4;
	gfx_s32 Y4;
	gfx_s32 X5;
	gfx_s32 Y5;
	CursorState Cursor;
	CursorState Cursor2;
	CursorState Cursor3;
	CursorState Cursor4;
	CursorState Cursor5;
	gfx_u32 Data;
	void* Settings;
	bool CursorCoordonateUsed;
	bool WindowRefresh;
	ControlCommands Comand;
}tControlCommandData;
/*#####################################################*/
typedef struct
{
	struct
	{
		gfx_u32 Ink;
		gfx_u32 Buton;
		gfx_u32 Border;
		gfx_u32 BackGround;
		double ScalerOnDisabled;
	}Disabled;
	struct
	{
		struct
		{
			gfx_u32 Push;
			gfx_u32 Pull;
		}Ink;
		struct
		{
			gfx_u32 Push;
			gfx_u32 Pull;
		}Buton;
		struct
		{
			gfx_u32 Push;
			gfx_u32 Pull;
		}Border;
		gfx_u32 TextBackGround;
		gfx_u32 BackGround;
		double ScalerOnDisabled;
	}Enabled;
	gfx_u32 Scren;
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
		gfx_s32 X;
		gfx_s32 Y;
	}Position;
	struct
	{
		gfx_s32 X;
		gfx_s32 Y;
	}Size;
	struct
	{
		gfx_u32 Background;
		gfx_u32 Scren;
	}Color;
	bool Visible;
	bool Enabled;
	struct
	{
		gfx_u32 ItemsNumber;
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
gfx_u32 controlls_change_color(gfx_u32 color, double value);
bool screen_copy(void *pDisplayTo, void *pDisplayFrom, bool put_cursor, gfx_s32 X, gfx_s32 Y, gfx_u32 color);
void TouchPaintPoint(void *pDisplay, gfx_s32 X, gfx_s32 Y, gfx_u32 color);
void put_circle(void *pDisplay, gfx_s32 x, gfx_s32 y, gfx_s32 _radius, gfx_u8 fill, gfx_u32 color);
void put_line(void *pDisplay, gfx_s32 X1, gfx_s32 Y1, gfx_s32 X2, gfx_s32 Y2, gfx_u8 width, gfx_u32 color);
void put_elipse(void *pDisplay, gfx_s32 xc,gfx_s32 yc,gfx_s32 _rx,gfx_s32 _ry, gfx_u8 Fill, gfx_u32 color);
void put_triangle(void *pDisplay, gfx_s32  Ax,gfx_s32  Ay,gfx_s32  Bx,gfx_s32  By,gfx_s32  Cx,gfx_s32  Cy, gfx_u8 Fill, gfx_u32 color);


/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "controls_definition.cpp"
#endif
/*#####################################################*/
#endif /* CONTROLS_DEFINITION_H_ */
/*#####################################################*/
