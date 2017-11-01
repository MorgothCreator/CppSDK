/*
 *  lib/gfx/keyboard.cpp
 */
//#######################################################################################
#include <stdbool.h>
#include <stdlib.h>
#include "button.h"
#include "keyboard.h"
#include "window.h"
#include "util.h"
#include "api/timer.h"
#include "graphic_string.h"
#include "controls_definition.h"
//#######################################################################################
#ifdef __AVR_XMEGA__
const gfx_u32 kbd_qwerty_keys_little_return[]  PROGMEM =
#else
static const gfx_u32 kbd_qwerty_keys_little_return[] =
#endif
{
		kbd_qwerty_ctrl,'q','w','e','r','t','y','u','i' ,'o','p'            ,','          ,8               ,
		'#'           ,'a','s','d','f','g','h','j','k' ,'l','.'            ,kbd_qwerty_up,13              ,
		0            ,'z','x','c','v','b','n','m',32      ,kbd_qwerty_left,kbd_qwerty_dn,kbd_qwerty_right
};
//#######################################################################################
#ifdef __AVR_XMEGA__
const gfx_u32 kbd_qwerty_keys_big_return[]  PROGMEM =
#else
static const gfx_u32 kbd_qwerty_keys_big_return[] =
#endif
{
		kbd_qwerty_ctrl,'Q','W','E','R','T','Y','U','I','O','P'            ,';'          ,8               ,
		'#'           ,'A','S','D','F','G','H','J','K','L',':'            ,kbd_qwerty_up,13              ,
		0              ,'Z','X','C','V','B','N','M',32     ,kbd_qwerty_left,kbd_qwerty_dn,kbd_qwerty_right
};
//#######################################################################################
#ifdef __AVR_XMEGA__
const gfx_u32 kbd_qwerty_keys_numeric_return[]  PROGMEM =
#else
static const gfx_u32 kbd_qwerty_keys_numeric_return[] =
#endif
{
		kbd_qwerty_ctrl,'1','2','3','4','5','6','7','8','9','0'            ,'='          ,8               ,
		'#'           ,'*','+','#','-','_','(',')','&','!','?'            ,kbd_qwerty_up,13              ,
		0              ,0  ,'&',0  ,47 ,92 ,34 ,96 ,32     ,kbd_qwerty_left,kbd_qwerty_dn,kbd_qwerty_right
};
//#######################################################################################
#ifdef __AVR_XMEGA__
const gfx_u8 kbd_qwerty_keys_little[][6]  PROGMEM =
#else
static const gfx_u8 kbd_qwerty_keys_little[][6] =
#endif
{
		"Ctrl","q","w","e","r","t","y","u","i" ,"o","p"  ,"," ,"BkS",
		"Nr"  ,"a","s","d","f","g","h","j","k" ,"l","."  ,"Up","Ent",
		"^"   ,"z","x","c","v","b","n","m","space" ,"Lef","Dn","Rig"
};
//#######################################################################################
#ifdef __AVR_XMEGA__
const gfx_u8 kbd_qwerty_keys_big[][6]  PROGMEM =
#else
static const gfx_u8 kbd_qwerty_keys_big[][6] =
#endif
{
		"Ctrl","Q","W","E","R","T","Y","U","I" ,"O","P"  ,";" ,"BkS",
		"Nr"  ,"A","S","D","F","G","H","J","K" ,"L",":"  ,"Up","Ent",
		"^"   ,"Z","X","C","V","B","N","M","space" ,"Lef","Dn","Rig"
};
//#######################################################################################
#ifdef __AVR_XMEGA__
const gfx_u8 kbd_qwerty_keys_numeric[][6]  PROGMEM =
#else
static const gfx_u8 kbd_qwerty_keys_numeric[][6] =
#endif
{
		"Ctrl","1","2","3","4","5","6","7","8" ,"9"     ,"0"  ,"=" ,"BkS",
		"Nr"  ,"*","+","#","-","_","(",")","&" ,"!"     ,"?"  ,"Up","Ent",
		"^"   ," ","&"," ","/","sl","quot","'" ,"space" ,"Lef","Dn","Rig"
};
//#######################################################################################
void GI::Screen::Gfx::KbdQwerty::paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand, bool refrash)
{
	gfx_u32 color = 0;
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window*)Internals.parentWindowHandler;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion.sXMin = x_start;
	LcdStruct->sClipRegion.sYMin = y_start;
	LcdStruct->sClipRegion.sXMax = x_start + x_len;
	LcdStruct->sClipRegion.sYMax = y_start + y_len;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);

	if(refrash)
	{
		color = controls_color.Control_Color_Enabled_Border_Pull;
		LcdStruct->drawRectangle(x_start, y_start, x_len, y_len, false, controlls_change_color(color, -3));
		LcdStruct->drawRectangle(x_start + 1, y_start + 1, x_len - 2, y_len - 2, false, controlls_change_color(color, -2));
		color = controls_color.Control_Color_Enabled_Buton_Pull;
		LcdStruct->drawRectangle(x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, color);
		gfx_s32 key_size_x = ((Size.X - 4 - (kbd_border_size << 1))/13) - key_space_size;
		gfx_s32 key_size_y = ((Size.Y - 4 - (kbd_border_size << 1))/3) - key_space_size;

		gfx_s32 KeyboardLocationX = ((/*settings->Internals.Position.X + */kbd_border_size) + ((Internals.Size.X>>1) - kbd_border_size)) - (((key_size_x + key_space_size) * 13)>>1);
		gfx_s32 KeyLocationY =      ((/*settings->Internals.Position.Y + */kbd_border_size) + ((Internals.Size.Y>>1) - kbd_border_size)) - (((key_size_y + key_space_size) *  3)>>1);
		gfx_s32 KeyLocationX = KeyboardLocationX + 2;

		gfx_u32 CntInitKeys = 0;
		for(; CntInitKeys < sizeof(Internals.Keys)/sizeof(Internals.Keys[0]); CntInitKeys++)
		{
			GI::Screen::Gfx::Button* KeyTmpPtr =  Internals.Keys[CntInitKeys];
			if(KeyTmpPtr == 0) break;
			switch (CntInitKeys)
			{
				case 0:
				KeyTmpPtr->Size.X = key_size_x;
				KeyTmpPtr->Size.Y = key_size_y;
				KeyLocationX = KeyboardLocationX;
				//KeyLocationY += key_size_y + settings->key_space_size;
				break;
				case 13:
				case 26:
				KeyTmpPtr->Size.X = key_size_x;
				KeyTmpPtr->Size.Y = key_size_y;
				KeyLocationX = KeyboardLocationX;
				KeyLocationY += key_size_y + key_space_size;
				break;
				//case 25:
				//KeyTmpPtr->Size.X = (key_size_x << 1) + settings->key_space_size;
				//KeyTmpPtr->Size.Y = key_size_y;
				//KeyLocationX += key_size_x + settings->key_space_size;
				//break;
				case 34:
				KeyLocationX += key_size_x + key_space_size;
				KeyTmpPtr->Size.X = (key_size_x << 1) + key_space_size;
				KeyTmpPtr->Size.Y = key_size_y;
				break;
				case 35:
				KeyTmpPtr->Size.X = key_size_x;
				KeyTmpPtr->Size.Y = key_size_y;
				KeyLocationX += ((key_size_x << 1) + (key_space_size << 1));
				break;
				default:
				KeyTmpPtr->Size.X = key_size_x;
				KeyTmpPtr->Size.Y = key_size_y;
				KeyLocationX += key_size_x + key_space_size;
			}
			KeyTmpPtr->Internals.PositionOffset.X = x_start - parentWindowHandler->Internals.Position.X;
			KeyTmpPtr->Internals.PositionOffset.Y = y_start - parentWindowHandler->Internals.Position.Y;
			KeyTmpPtr->Position.X = KeyLocationX;
			KeyTmpPtr->Position.Y = KeyLocationY;
			KeyTmpPtr->Caption->pFont = pFont;
			KeyTmpPtr->Internals.NoPaintBackGround = true;
			KeyTmpPtr->Visible = Visible;
			KeyTmpPtr->idle(control_comand);
		}
		control_comand->WindowRefresh |= true;
	}
	else
	{
		gfx_u32 CntInitKeys = 0;
		for(; CntInitKeys < sizeof(Internals.Keys)/sizeof(Internals.Keys[0]); CntInitKeys++)
		{
			GI::Screen::Gfx::Button* KeyTmpPtr =  Internals.Keys[CntInitKeys];
			KeyTmpPtr->Visible = Visible;
			KeyTmpPtr->idle(control_comand);
			if(CntInitKeys == 13 && KeyTmpPtr->Events.CursorUp == true)
			{
				KeyTmpPtr->Events.CursorUp = false;
				Internals.keboardType = Kbd_Qwerty_Numeric;
			}
			else if(CntInitKeys == 26 && KeyTmpPtr->Events.CursorUp == true)
			{
				KeyTmpPtr->Events.CursorUp = false;
				if(Internals.keboardType == Kbd_Qwerty_Numeric)
					Internals.keboardType = Kbd_Qwerty_LittleLetters;
				else if(Internals.keboardType == Kbd_Qwerty_BigLetters)
					Internals.keboardType = Kbd_Qwerty_LittleLetters;
				else if(Internals.keboardType == Kbd_Qwerty_LittleLetters)
					Internals.keboardType = Kbd_Qwerty_BigLetters;
			}
			else if(KeyTmpPtr->Events.CursorUp == true)
			{

				KeyTmpPtr->Events.CursorUp = false;
				if(Internals.keboardType == Kbd_Qwerty_Numeric)
					Return = (gfx_u32)kbd_qwerty_keys_numeric_return[CntInitKeys];
				else if(Internals.keboardType == Kbd_Qwerty_BigLetters)
					Return = (gfx_u32)kbd_qwerty_keys_big_return[CntInitKeys];
				else if(Internals.keboardType == Kbd_Qwerty_LittleLetters)
					Return = (gfx_u32)kbd_qwerty_keys_little_return[CntInitKeys];
				Events.CursorUp = true;
				if(Events.OnUp.CallBack)
					Events.OnUp.CallbackReturnData = Events.OnUp.CallBack(Events.OnUp.CallbackData, Return);
			}
			else if(KeyTmpPtr->Events.CursorDown == true)
			{
				KeyTmpPtr->Events.CursorDown = false;
				if(Internals.keboardType == Kbd_Qwerty_Numeric)
					Return = (gfx_u32)kbd_qwerty_keys_numeric[CntInitKeys];
				else if(Internals.keboardType == Kbd_Qwerty_BigLetters)
					Return = (gfx_u32)kbd_qwerty_keys_big_return[CntInitKeys];
				else if(Internals.keboardType == Kbd_Qwerty_LittleLetters)
					Return = (gfx_u32)kbd_qwerty_keys_little_return[CntInitKeys];
				Events.CursorDown = true;
				if(Events.OnDown.CallBack)
					Events.OnDown.CallbackReturnData = Events.OnDown.CallBack(Events.OnDown.CallbackData, Return);
			}
			else if(KeyTmpPtr->Events.CursorMove == true)
			{
				KeyTmpPtr->Events.CursorMove = false;
				if(Internals.keboardType == Kbd_Qwerty_Numeric)
					Return = (gfx_u32)kbd_qwerty_keys_numeric_return[CntInitKeys];
				else if(Internals.keboardType == Kbd_Qwerty_BigLetters)
					Return = (gfx_u32)kbd_qwerty_keys_big_return[CntInitKeys];
				else if(Internals.keboardType == Kbd_Qwerty_LittleLetters)
					Return = (gfx_u32)kbd_qwerty_keys_little_return[CntInitKeys];
				Events.CursorMove = true;
				if(Events.OnMove.CallBack)
					Events.OnMove.CallbackReturnData = Events.OnMove.CallBack(Events.OnMove.CallbackData, Return);
			}
		}
	}

	LcdStruct->sClipRegion.sXMin = x_start;
	LcdStruct->sClipRegion.sYMin = y_start;
	LcdStruct->sClipRegion.sXMax = x_start + x_len;
	LcdStruct->sClipRegion.sYMax = y_start + y_len;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	//LcdStruct->cacheClean(x_start, y_start, x_len, y_len);
	LcdStruct->refresh();
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::KbdQwerty::idle(tControlCommandData* control_comand)
{
	if(this == NULL)
		return;
	tControlCommandData _control_comand_;
	if(!control_comand)
		control_comand = &_control_comand_;
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window*)Internals.parentWindowHandler;
	if(control_comand->Comand != Control_Nop)
	{
		/* Parse commands */
#ifdef NO_ENUM_ON_SWITCH
		switch((gfx_u8)control_comand->Comand)
#else
		switch((int)control_comand->Comand)
#endif
		{
		case Control_Entire_Repaint:
			Internals.NeedEntireRepaint = true;
			return;
		case Control_Entire_Refresh:
			Internals.NeedEntireRefresh = true;
			return;
		case Control_Refresh:
			Internals.NeedEntireRefresh = true;
			break;
		}
	}
	if(Internals.Control.Initiated == false)
	{
		Internals.Position.X = Position.X;
		Internals.Position.Y = Position.Y;
		Internals.Size.X = Size.X;
		Internals.Size.Y = Size.Y;
	}
	/* Verify if position on size has been modified */
	if(parentWindowHandler) {
		if(Position.X + parentWindowHandler->Internals.Position.X + Internals.PositionOffset.X != Internals.Position.X ||
				Position.Y + parentWindowHandler->Internals.Position.Y + Internals.PositionOffset.Y != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							Internals.keboardType != Internals.oldKeboardType)
			Internals.NeedEntireRefresh = true;
	}
	else {
		if(Position.X != Internals.Position.X ||
				Position.Y != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							Internals.keboardType != Internals.oldKeboardType)
			Internals.NeedEntireRefresh = true;
	}

	gfx_s32 X_StartBox = Internals.Position.X;
	gfx_s32 Y_StartBox = Internals.Position.Y;
	gfx_s32 X_LenBox = Internals.Size.X;
	gfx_s32 Y_LenBox = Internals.Size.Y;
	void *pDisplay = Internals.pDisplay;
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;

	/*Clear background of box with actual painted dimensions and positions if they been changed*/
	if(Internals.NeedEntireRefresh == true || (Internals.OldStateVisible != Visible && Visible == false))
	{
		if(!Internals.NoPaintBackGround)
		{
			Internals.OldStateVisible = Visible;
			tRectangle back_up_clip = LcdStruct->sClipRegion;
			LcdStruct->sClipRegion.sXMin = X_StartBox;
			LcdStruct->sClipRegion.sYMin = Y_StartBox;
			LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
			LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
			LcdStruct->drawRectangle(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, true, Color.Scren);
			//LcdStruct->cacheClean(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
			LcdStruct->refresh();
			LcdStruct->sClipRegion = back_up_clip;
		}
	}

	/* Verify if is Entire refresh, entire repaint, or state changed */
	if((Internals.NeedEntireRefresh == true ||
			Internals.NeedEntireRepaint == true ||
				Internals.Control.Initiated == false ||
					//Enabled == true ||
						Internals.OldStateVisible != Visible) &&
							Visible == true)
	{
		/* Copy new locations and dimensions to actual locations and dimensions */
		if(parentWindowHandler)
		{
			Internals.Position.X = Position.X + parentWindowHandler->Internals.Position.X + Internals.PositionOffset.X;
			Internals.Position.Y = Position.Y + parentWindowHandler->Internals.Position.Y + Internals.PositionOffset.Y;
		}
		else
		{
			Internals.Position.X = Position.X;
			Internals.Position.Y = Position.Y;
		}
		Internals.Size.X = Size.X;
		Internals.Size.Y = Size.Y;
		Internals.oldKeboardType = Internals.keboardType;
		X_StartBox = Internals.Position.X;
		Y_StartBox = Internals.Position.Y;
		X_LenBox = Internals.Size.X;
		Y_LenBox = Internals.Size.Y;

		gfx_u8 CntInitKeys = 0;
		for(; CntInitKeys < sizeof(Internals.Keys)/sizeof(Internals.Keys[0]); CntInitKeys++)
		{
			if(Internals.keboardType == Kbd_Qwerty_LittleLetters)
				Internals.Keys[CntInitKeys]->Caption->buff = (char *)kbd_qwerty_keys_little[CntInitKeys];
			else if(Internals.keboardType == Kbd_Qwerty_BigLetters)
				Internals.Keys[CntInitKeys]->Caption->buff = (char *)kbd_qwerty_keys_big[CntInitKeys];
			else if(Internals.keboardType == Kbd_Qwerty_Numeric)
				Internals.Keys[CntInitKeys]->Caption->buff = (char *)kbd_qwerty_keys_numeric[CntInitKeys];
			Internals.Keys[CntInitKeys]->Internals.NeedEntireRefresh = true;
		}
		//CursorState back = control_comand->Cursor;
		//control_comand->Cursor = Cursor_Up;
		paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand, true);
		//control_comand->Cursor = back;
		Internals.OldStateVisible = Visible;
		Internals.Control.Initiated = true;
		Internals.NeedEntireRefresh = false;
		Internals.NeedEntireRepaint = false;
		//control_comand->Cursor = cursor;
		//control_comand->WindowRefresh |= true;
		return;
	}
	paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand, false);
	if(control_comand->Cursor && Internals.CursorDownInsideBox)
		control_comand->CursorCoordonateUsed |= true;
	//control_comand->CursorCoordonateUsed |= true;
	//control_comand->WindowRefresh |= true;
	return;
}
//#######################################################################################
GI::Screen::Gfx::KbdQwerty::KbdQwerty(void *parentWindowHandler)
{
	if(!parentWindowHandler)
		return;
	memset(this, 0, sizeof(*this));
	Internals.parentWindowHandler = parentWindowHandler;

	GI::Screen::Gfx::Window *_parentWindowHandler = (GI::Screen::Gfx::Window *)parentWindowHandler;
	Internals.pDisplay = _parentWindowHandler->Internals.pDisplay;

	pFont = controls_color.DefaultFont;

	Color.Scren = controls_color.Scren;
	Color.Enabled.BackGround = controls_color.Control_Color_Enabled_BackGround;
	Color.Enabled.Border.Pull = controls_color.Control_Color_Enabled_Border_Pull;
	Color.Enabled.Border.Push = controls_color.Control_Color_Enabled_Border_Push;
	Color.Enabled.Buton.Pull = controls_color.Control_Color_Enabled_Buton_Pull;
	Color.Enabled.Buton.Push = controls_color.Control_Color_Enabled_Buton_Push;
	Color.Enabled.Ink.Pull = controls_color.Control_Color_Enabled_Ink_Pull;
	Color.Enabled.Ink.Push = controls_color.Control_Color_Enabled_Ink_Push;

	Color.Disabled.BackGround = controls_color.Control_Color_Disabled_BackGround;
	Color.Disabled.Border = controls_color.Control_Color_Disabled_Border_Pull;
	Color.Disabled.Buton = controls_color.Control_Color_Disabled_Buton_Pull;
	Color.Disabled.Ink = controls_color.Control_Color_Disabled_Ink_Pull;

	Enabled = true;
	Position.X = 10;
	Position.Y = 10;
	Size.X = 60;
	Size.Y = 20;
	Visible = true;
	key_space_size = 0;

	gfx_u8 CntInitKeys = 0;
	for(; CntInitKeys < sizeof(Internals.Keys) / sizeof(Internals.Keys[0]); CntInitKeys++)
	{
		Internals.Keys[CntInitKeys] = new GI::Screen::Gfx::Button(Internals.parentWindowHandler);
		Internals.Keys[CntInitKeys]->Caption->wordWrap = false;
		//if(Internals.Keys[CntInitKeys]->Caption->buff)
			//free(Internals.Keys[CntInitKeys]->Caption->buff);
		Internals.Keys[CntInitKeys]->Caption->setText((char *)kbd_qwerty_keys_little[CntInitKeys]);
	}
}
//#######################################################################################
GI::Screen::Gfx::KbdQwerty::~KbdQwerty()
{
	if(!this)
		return;
	Visible = false;
	idle();
}
//#######################################################################################
