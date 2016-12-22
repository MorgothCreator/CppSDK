/*
 *  lib/gfx/picturebox.c
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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
#include <stdbool.h>
#include "picturebox.h"
#include "window.h"
#include "api/lcd_def.h"
#include "util.h"
#include "api/timer_api.h"
#include "3d.h"
#include "controls_definition.h"
#include <interface/screen.h>
#if _USE_BITMAP_LIBRARY == 1
#include "bmp.h"
#endif


void GI::Screen::Gfx::Picturebox::paint(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, tControlCommandData* control_comand)
{
	unsigned int color = 0;
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;
	GI::Screen::Gfx::Window *ParentWindow = (GI::Screen::Gfx::Window*)Internals.parentWindowHandler;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion.sXMin = x_start;
	LcdStruct->sClipRegion.sYMin = y_start;
	LcdStruct->sClipRegion.sXMax = x_start + x_len;
	LcdStruct->sClipRegion.sYMax = y_start + y_len;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	color = controls_color.Control_Color_Enabled_Border_Pull;
	if((!Enabled || !ParentWindow->Internals.OldStateEnabled) && Internals.Control.Initiated == true) {
		color = Color.Disabled.Border;
		LcdStruct->drawHLine(x_start, x_len, y_start, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
		LcdStruct->drawHLine(x_start + 1, x_len - 2, y_start + 1, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));

		LcdStruct->drawHLine(x_start + 1, x_len - 2, y_start + y_len - 2, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));
		LcdStruct->drawHLine(x_start, x_len, (y_start + y_len) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));

		LcdStruct->drawVLine(y_start, y_len, x_start, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
		LcdStruct->drawVLine(y_start + 1, y_len - 2, x_start + 1, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));

		LcdStruct->drawVLine(y_start + 1, y_len - 2, x_start + x_len - 2, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));
		LcdStruct->drawVLine(y_start, y_len, (x_start + x_len) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
	} else {
		LcdStruct->drawHLine(x_start, x_len, y_start, 1, controlls_change_color(color, +BORDER_LINE_ONE_LIGHT));
		LcdStruct->drawHLine(x_start + 1, x_len - 2, y_start + 1, 1, controlls_change_color(color, +BORDER_LINE_TWO_LIGHT));

		LcdStruct->drawHLine(x_start + 1, x_len - 2, y_start + y_len - 2, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));
		LcdStruct->drawHLine(x_start, x_len, (y_start + y_len) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));

		LcdStruct->drawVLine(y_start, y_len, x_start, 1, controlls_change_color(color, +BORDER_LINE_ONE_LIGHT));
		LcdStruct->drawVLine(y_start + 1, y_len - 2, x_start + 1, 1, controlls_change_color(color, +BORDER_LINE_TWO_LIGHT));

		LcdStruct->drawVLine(y_start + 1, y_len - 2, x_start + x_len - 2, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));
		LcdStruct->drawVLine(y_start, y_len, (x_start + x_len) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));

	}

	//put_rectangle(pDisplay, x_start, y_start, x_len, y_len, false, controlls_change_color(color, -3));
	//put_rectangle(pDisplay, x_start + 1, y_start + 1, x_len - 2, y_len - 2, false, controlls_change_color(color, -2));
	color = BackgroundColor;
	if(!Enabled || !ParentWindow->Internals.OldStateEnabled)
		color = Color.Disabled.Buton;
	//color = controls_color.Control_Color_Enabled_Buton_Pull;
	if(PaintBackground)
		LcdStruct->drawRectangle(x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, color);
	if(Events.Refresh.CallBack)
	{
		tRectangle clip;
		clip.sXMin = x_start + 2;
		clip.sXMax = x_start + x_len - 2;
		clip.sYMin = y_start + 2;
		clip.sYMax = y_start + y_len - 2;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &clip);
		Internals.PictureWindowLimits = LcdStruct->sClipRegion;
		signed int X = control_comand->X;
		signed int Y = control_comand->Y;
		control_comand->X -= Internals.Position.X + 2;
		control_comand->Y -= Internals.Position.Y + 2;
		Events.Refresh.CallbackReturnData = Events.Refresh.CallBack(this, control_comand);
		control_comand->X = X;
		control_comand->Y = Y;
	}
	LcdStruct->sClipRegion.sXMin = x_start;
	LcdStruct->sClipRegion.sYMin = y_start;
	LcdStruct->sClipRegion.sXMax = x_start + x_len;
	LcdStruct->sClipRegion.sYMax = y_start + y_len;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	LcdStruct->cacheClean(x_start, y_start, x_len, y_len);
	LcdStruct->sClipRegion = back_up_clip;
	control_comand->WindowRefresh |= true;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::idle(tControlCommandData* control_comand)
{
	if(this == NULL)
		return;
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window*)Internals.parentWindowHandler;
	if(control_comand->Comand != Control_Nop)
	{
		/* Parse commands */
#ifdef NO_ENUM_ON_SWITCH
		switch((unsigned char)control_comand->Comand)
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
	}
	/* Verify if position on size has been modified */
	if(parentWindowHandler)
	{
		if((Position.X + parentWindowHandler->Internals.Position.X + Internals.PositionOffset.X) != Internals.Position.X ||
				(Position.Y + parentWindowHandler->Internals.Position.Y + Internals.PositionOffset.Y) != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							Internals.OldStateEnabled != Enabled ||
							parentWindowHandler->Internals.OldStateEnabled != Internals.parentWindowStateEnabled)
											Internals.NeedEntireRefresh = true;
	}
	else
	{
		if(Position.X != Internals.Position.X ||
				Position.Y != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							Internals.OldStateEnabled != Enabled)
											Internals.NeedEntireRefresh = true;
	}

	signed int X_StartBox = Internals.Position.X;
	signed int Y_StartBox = Internals.Position.Y;
	signed int X_LenBox = Internals.Size.X;
	signed int Y_LenBox = Internals.Size.Y;
	void *pDisplay = Internals.pDisplay;
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;

	/*Clear background of box with actual painted dimensions and positions if they been changed*/
	/*if(settings->Internals.NeedEntireRefresh == true || (settings->Internals.OldStateVisible != settings->Visible && settings->Visible == false))
	{
		settings->Internals.OldStateVisible = settings->Visible;
		if(settings->Events.Refresh.CallBack)
		{
			tRectangle back_up_clip = pDisplay->sClipRegion;
			tRectangle clip;
			clip.sXMin = X_StartBox + 2;
			clip.sXMax = (X_StartBox + X_LenBox) - 2;
			clip.sYMin = Y_StartBox + 2;
			clip.sYMax = (Y_StartBox + Y_LenBox) - 2;
			GScreen::Util::clipLimit(&pDisplay->sClipRegion, &clip);
			settings->Internals.PictureWindowLimits = pDisplay->sClipRegion;
			signed int X = control_comand->X;
			signed int Y = control_comand->Y;
			control_comand->X -= settings->Position.X + 2;
			control_comand->Y -= settings->Position.Y + 2;
			settings->Events.Refresh.CallbackReturnData = settings->Events.Refresh.CallBack(settings, control_comand);
			control_comand->X = X;
			control_comand->Y = Y;
			box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
			pDisplay->sClipRegion = back_up_clip;
		}
	}*/
	if(Internals.NeedEntireRefresh == true || Internals.OldStateVisible != Visible)
	{
		if(!Internals.NoPaintBackGround || !Visible)
		{
			Internals.OldStateVisible = Visible;
			tRectangle back_up_clip = LcdStruct->sClipRegion;
			LcdStruct->sClipRegion.sXMin = X_StartBox;
			LcdStruct->sClipRegion.sYMin = Y_StartBox;
			LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
			LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
			LcdStruct->drawRectangle(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, true, Color.Scren);
			LcdStruct->cacheClean(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
			LcdStruct->sClipRegion = back_up_clip;
			control_comand->WindowRefresh |= true;
			if(!Visible)
				return;
		}
		Internals.NeedEntireRefresh = true;
	}


	/* Verify if is Entire refresh, entire repaint, or state changed */
	if((Internals.NeedEntireRefresh == true ||
			Internals.NeedEntireRepaint == true ||
				Internals.Control.Initiated == false) &&
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
		X_StartBox = Internals.Position.X;
		Y_StartBox = Internals.Position.Y;
		X_LenBox = Internals.Size.X;
		Y_LenBox = Internals.Size.Y;
		paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		Internals.parentWindowStateEnabled = parentWindowHandler->Internals.OldStateEnabled;
		Internals.OldStateVisible = Visible;
		Internals.OldStateEnabled = Enabled;
		Internals.Control.Initiated = true;
		Internals.NeedEntireRefresh = false;
		Internals.NeedEntireRepaint = false;
		//control_comand->Cursor = cursor;
		//control_comand->WindowRefresh |= true;
		return;
	}
	/* Check if inside window */
	bool inside_window = GI::Screen::Util::insideBox(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y);
	bool _inside_window = GI::Screen::Util::insideBox(LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMin, LcdStruct->sClipRegion.sXMax - LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMax - LcdStruct->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window)
		inside_window = false;
	if((control_comand->Cursor) &&
			//settings->Internals.OldStateCursor != control_comand->Cursor &&
				(inside_window == true || Internals.CursorDownInsideBox == true) &&
					Enabled == true &&
						Visible == true &&
							control_comand->CursorCoordonateUsed == false)
	{
		//settings->Internals.OldStateCursor = control_comand->Cursor;
		if(control_comand->Cursor == Cursor_Down && inside_window == true)
		{
			Events.CursorDown = true;
			Internals.CursorDownInsideBox = true;
			if(Events.OnDown.CallBack)
			{
				tRectangle back_up_clip = LcdStruct->sClipRegion;
				tRectangle clip;
				clip.sXMin = X_StartBox + 2;
				clip.sXMax = (X_StartBox + X_LenBox) - 2;
				clip.sYMin = Y_StartBox + 2;
				clip.sYMax = (Y_StartBox + Y_LenBox) - 2;
				GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &clip);
				Internals.PictureWindowLimits = LcdStruct->sClipRegion;
				signed int X = control_comand->X;
				signed int Y = control_comand->Y;
				control_comand->X -= Internals.Position.X + 2;
				control_comand->Y -= Internals.Position.Y + 2;
				Events.OnDown.CallbackReturnData = Events.OnDown.CallBack(this, control_comand);
				control_comand->X = X;
				control_comand->Y = Y;
				LcdStruct->cacheClean(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
				LcdStruct->sClipRegion = back_up_clip;
				control_comand->WindowRefresh |= true;
			}
		}
		if(control_comand->Cursor == Cursor_Up && Internals.CursorDownInsideBox == true && inside_window == true)
		{
			Events.CursorUp = true;
			if(Events.OnUp.CallBack)
			{
				tRectangle back_up_clip = LcdStruct->sClipRegion;
				tRectangle clip;
				clip.sXMin = X_StartBox + 2;
				clip.sXMax = (X_StartBox + X_LenBox) - 2;
				clip.sYMin = Y_StartBox + 2;
				clip.sYMax = (Y_StartBox + Y_LenBox) - 2;
				GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &clip);
				Internals.PictureWindowLimits = LcdStruct->sClipRegion;
				signed int X = control_comand->X;
				signed int Y = control_comand->Y;
				control_comand->X -= Internals.Position.X + 2;
				control_comand->Y -= Internals.Position.Y + 2;
				Events.OnUp.CallbackReturnData = Events.OnUp.CallBack(this, control_comand);
				control_comand->X = X;
				control_comand->Y = Y;
				LcdStruct->cacheClean(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
				LcdStruct->sClipRegion = back_up_clip;
				control_comand->WindowRefresh |= true;
			}
		}
		//paint_picturebox(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		if(control_comand->Cursor == Cursor_Move && Internals.CursorDownInsideBox == true && inside_window == true)
		{
			Events.CursorMove = true;
			if(Events.OnMove.CallBack)
			{
				tRectangle back_up_clip = LcdStruct->sClipRegion;
				tRectangle clip;
				clip.sXMin = X_StartBox + 2;
				clip.sXMax = (X_StartBox + X_LenBox) - 2;
				clip.sYMin = Y_StartBox + 2;
				clip.sYMax = (Y_StartBox + Y_LenBox) - 2;
				GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &clip);
				Internals.PictureWindowLimits = LcdStruct->sClipRegion;
				signed int X = control_comand->X;
				signed int Y = control_comand->Y;
				control_comand->X -= Internals.Position.X + 2;
				control_comand->Y -= Internals.Position.Y + 2;
				Events.OnMove.CallbackReturnData = Events.OnMove.CallBack(this, control_comand);
				control_comand->X = X;
				control_comand->Y = Y;
				LcdStruct->cacheClean(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
				LcdStruct->sClipRegion = back_up_clip;
				control_comand->WindowRefresh |= true;
			}
		}
	}
	if(control_comand->Cursor && Internals.CursorDownInsideBox)
		control_comand->CursorCoordonateUsed |= true;
	if(Internals.CursorDownInsideBox == true && (control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction))
		Internals.CursorDownInsideBox = false;
	//control_comand->CursorCoordonateUsed |= settings->Internals.CursorDownInsideBox;
	//control_comand->WindowRefresh |= true;
	return;
}

void GI::Screen::Gfx::Picturebox::idle()
{
	tControlCommandData control_comand;
	memset(&control_comand, 0, sizeof(tControlCommandData));
	idle(&control_comand);
}

GI::Screen::Gfx::Picturebox::Picturebox(void *parentWindowHandler)
{
	if(!parentWindowHandler)
		return;
	memset(this, 0, sizeof(*this));
	Internals.parentWindowHandler = parentWindowHandler;

	GI::Screen::Gfx::Window *_parentWindowHandler = (GI::Screen::Gfx::Window *)parentWindowHandler;
	Internals.pDisplay = _parentWindowHandler->Internals.pDisplay;

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
	BackgroundColor = ClrWhite;
}
//#######################################################################################
GI::Screen::Gfx::Picturebox::~Picturebox()
{
	if(!this)
		return;
	Visible = false;
	idle();
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::clear()
{
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window*)Internals.parentWindowHandler;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	signed int X_StartBox = Internals.Position.X;
	signed int Y_StartBox = Internals.Position.Y;
	signed int X_LenBox = Internals.Size.X;
	signed int Y_LenBox = Internals.Size.Y;
	LcdStruct->sClipRegion.sXMin = X_StartBox;
	LcdStruct->sClipRegion.sYMin = Y_StartBox;
	LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
	LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	unsigned int color = BackgroundColor;
	if(!Enabled || !parentWindowHandler->Internals.OldStateEnabled)
		color = Color.Disabled.Buton;
	LcdStruct->drawRectangle(X_StartBox + 2, Y_StartBox + 2, X_LenBox - 4, Y_LenBox - 4, true, color);
	LcdStruct->cacheClean(X_StartBox + 2, Y_StartBox + 2, X_LenBox - 4, Y_LenBox - 4);
	LcdStruct->sClipRegion = back_up_clip;
}
/*//#######################################################################################
void picturebox_copy_rectangle(tPictureBox* settings, unsigned int *src_buff, signed int src_x_buff_size, signed int src_y_buff_size, signed int src_x_offset, signed int src_y_offset)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	//put_pixel(pDisplay, X + settings->Position.X + 2, Y + settings->Position.Y + 2, color);

	signed int Y_cnt = 0;
	signed int X_len = (pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin);
	if(X_len <= 0) return;
	for(; Y_cnt < src_y_offset + (pDisplay->sClipRegion.sYMax - pDisplay->sClipRegion.sYMin); Y_cnt++)
	{
		if(Y_cnt + pDisplay->sClipRegion.sYMin >= pDisplay->sClipRegion.sYMax) break;
		memcpy((void *)(pDisplay->DisplayData + 8 + ((Y_cnt + settings->Position.Y + 2) * pDisplay->Width) + settings->Position.X + 2), (void *)((char *)(src_buff + ((Y_cnt + src_y_offset) * src_x_buff_size)) - 1), X_len * sizeof(pDisplay->DisplayData[0]));
		CacheDataCleanInvalidateBuff((unsigned int)(void *)(pDisplay->DisplayData + 8 + ((Y_cnt + settings->Position.Y + 2) * pDisplay->Width) + settings->Position.X + 2), X_len * sizeof(pDisplay->DisplayData[0]) + 64);
	}
	pDisplay->sClipRegion = back_up_clip;
}*/
//#######################################################################################
void GI::Screen::Gfx::Picturebox::copyRectangle(unsigned int *src_buff, unsigned int src_buff_data_offset, tRectangle *_dest_rectangle, tRectangle *_src_rectangle, signed int src_width, signed int src_height)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* pDisplay = Internals.pDisplay;
	//tRectangle back_up_clip = pDisplay->sClipRegion;
	//pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	////put_pixel(pDisplay, X + settings->Position.X + 2, Y + settings->Position.Y + 2, color);

	tRectangle *dest_rectangle = _dest_rectangle;
	//tRectangle *src_rectangle = _src_rectangle;

	signed int dest_X_StartBox = Internals.Position.X;
	signed int dest_Y_StartBox = Internals.Position.Y;
	//signed int dest_X_EndBox = settings->Internals.Position.X + settings->Internals.Size.X;
	//signed int dest_Y_EndBox = settings->Internals.Position.Y + settings->Internals.Size.Y;

	//Calculate the destination locations on the screen.
	dest_rectangle->sXMin += dest_X_StartBox + 2;
	dest_rectangle->sXMax += dest_X_StartBox + 2;
	dest_rectangle->sYMin += dest_Y_StartBox + 2;
	dest_rectangle->sYMax += dest_Y_StartBox + 2;
	if(dest_rectangle->sXMax < Internals.Position.X + 2 ||
			dest_rectangle->sYMax < Internals.Position.Y + 2 ||
				dest_rectangle->sXMin >= Internals.Position.X + (Internals.Size.X - 4) ||
					dest_rectangle->sYMin >= Internals.Position.Y + (Internals.Size.Y - 4))
		return;
	pDisplay->copyRectangle32Bit((unsigned char *)src_buff, dest_rectangle->sXMin, dest_rectangle->sYMin, dest_rectangle->sXMax - dest_rectangle->sXMin, dest_rectangle->sYMax - dest_rectangle->sYMin);
	//Limit the destination area to the picture box size.
	/*GScreen::Util::clipLimit(dest_rectangle, &settings->Internals.PictureWindowLimits);

	signed int x_line_len = dest_rectangle->sXMax - dest_rectangle->sXMin;

	if(x_line_len <= 0) return;
	if(dest_X_StartBox <= dest_rectangle->sXMin)
		dest_X_StartBox = dest_rectangle->sXMin;

	signed int Y_cnt = dest_Y_StartBox;
	if(Y_cnt <= dest_rectangle->sYMin)
		Y_cnt = dest_rectangle->sYMin;

	signed int X_Start_Src_Buff = (settings->Internals.Position.X + 2) - settings->Internals.PictureWindowLimits.sXMin;
	if(X_Start_Src_Buff > 0)
		X_Start_Src_Buff = 0;
	else
		X_Start_Src_Buff = (~X_Start_Src_Buff) + 1;
	signed int Y_Start_Src_Buff = (settings->Internals.Position.Y + 2) - settings->Internals.PictureWindowLimits.sYMin;
	if(Y_Start_Src_Buff > 0)
		Y_Start_Src_Buff = 0;
	else
		Y_Start_Src_Buff = (~Y_Start_Src_Buff) + 1;

	for(; Y_cnt < dest_rectangle->sYMax; Y_cnt++)
	{
		//if(Y_cnt >= pDisplay->sClipRegion.sYMax) break;
		memcpy((void *)(pDisplay->DisplayData + pDisplay->LcdTimings->palete_len + (Y_cnt * pDisplay->LcdTimings->X) + dest_X_StartBox), (void *)((char *)(src_buff + src_buff_data_offset + (((Y_cnt - settings->Internals.PictureWindowLimits.sYMin + Y_Start_Src_Buff) + src_rectangle->sYMin) * src_width) + src_rectangle->sXMin + X_Start_Src_Buff) - 1), x_line_len * sizeof(pDisplay->DisplayData[0]));
		//CacheDataCleanInvalidateBuff((unsigned int)(void *)(pDisplay->DisplayData + 8 + ((Y_cnt + settings->Position.Y + 2) * pDisplay->Width) + settings->Position.X + 2), X_len * sizeof(pDisplay->DisplayData[0]) + 64);
	}*/
	//pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawPixel(signed int X, signed int Y, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawPixel(X + Internals.Position.X + 2, Y + Internals.Position.Y + 2, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawHLine(signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawHLine(X1 + Internals.Position.X + 2, X2 + Internals.Position.X + 2, Y + Internals.Position.Y + 2, width, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawVLine(signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawVLine(Y1 + Internals.Position.Y + 2, Y2 + Internals.Position.Y + 2, X + Internals.Position.X + 2, width, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawCircle(signed int x, signed int y, signed int radius, unsigned char fill, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawCircle(x + Internals.Position.X + 2, y + Internals.Position.Y + 2, radius, fill, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawLine(signed int X1, signed int Y1, signed int X2, signed int Y2, unsigned char width, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawLine(X1 + Internals.Position.X + 2, Y1 + Internals.Position.Y + 2, X2 + Internals.Position.X + 2, Y2 + Internals.Position.Y + 2, width, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawElipse(signed int xc,signed int yc,signed int rx,signed int ry, unsigned char fill, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawElipse(xc + Internals.Position.X + 2, yc + Internals.Position.Y + 2, rx + Internals.Position.X + 2, ry + Internals.Position.Y + 2, fill, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawTriangle(signed int  Ax,signed int  Ay,signed int  Bx,signed int  By,signed int  Cx,signed int  Cy, unsigned char fill, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawTriangle(Ax + Internals.Position.X + 2, Ay + Internals.Position.Y + 2, Bx + Internals.Position.X + 2, By + Internals.Position.Y + 2, Cx + Internals.Position.X + 2, Cy + Internals.Position.Y + 2, fill, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawString(GI::Screen::String *string)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;

	string->pDisplay = Internals.pDisplay;
	signed int lX = string->lX;
	signed int lY = string->lY;
	string->lX += Internals.Position.X + 2;
	string->lY += Internals.Position.Y + 2;
	string->drawString();
	string->lX = lX;
	string->lY = lY;
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::draw3dTriangle(_3d_points *Points, signed int X_offset, signed int Y_offset, double X_Angle, double Y_Angle, double Z_Angle, unsigned int Color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	_3d_points screenPoints;

	axisRotations cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 4;
	Transform3DPointsTo2DPoints(&screenPoints, Points, cubeAxisRotations);

	unsigned int color = Color;//(controlls_change_color(Color, (-triangle_median))<<8) & 0xFFFFFF00;

	signed int X_start = (signed int)screenPoints.x[1];
	signed int Y_start = (signed int)screenPoints.y[1];
	signed int X_end = (signed int)screenPoints.x[2];
	signed int Y_end = (signed int)screenPoints.y[2];
	drawLine(X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (signed int)screenPoints.x[2];
	Y_start = (signed int)screenPoints.y[2];
	X_end = (signed int)screenPoints.x[3];
	Y_end = (signed int)screenPoints.y[3];
	drawLine(X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (signed int)screenPoints.x[3];
	Y_start = (signed int)screenPoints.y[3];
	X_end = (signed int)screenPoints.x[1];
	Y_end = (signed int)screenPoints.y[1];
	drawLine(X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::draw3dCube(_3d_points *Points, signed int X_offset, signed int Y_offset, double X_Angle, double Y_Angle, double Z_Angle, unsigned int Color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	_3d_points screenPoints;

	axisRotations cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 8;
	Transform3DPointsTo2DPoints(&screenPoints, Points, cubeAxisRotations);
	drawLine((signed int)screenPoints.x[0] + X_offset, (signed int)screenPoints.y[0] + Y_offset, (signed int)screenPoints.x[1] + X_offset, (signed int)screenPoints.y[1] + Y_offset, 1, Color);
	drawLine((signed int)screenPoints.x[0] + X_offset, (signed int)screenPoints.y[0] + Y_offset, (signed int)screenPoints.x[3] + X_offset, (signed int)screenPoints.y[3] + Y_offset, 1, Color);
	drawLine((signed int)screenPoints.x[0] + X_offset, (signed int)screenPoints.y[0] + Y_offset, (signed int)screenPoints.x[4] + X_offset, (signed int)screenPoints.y[4] + Y_offset, 1, Color);

	drawLine((signed int)screenPoints.x[2] + X_offset, (signed int)screenPoints.y[2] + Y_offset, (signed int)screenPoints.x[1] + X_offset, (signed int)screenPoints.y[1] + Y_offset, 1, Color);
	drawLine((signed int)screenPoints.x[2] + X_offset, (signed int)screenPoints.y[2] + Y_offset, (signed int)screenPoints.x[3] + X_offset, (signed int)screenPoints.y[3] + Y_offset, 1, Color);
	drawLine((signed int)screenPoints.x[2] + X_offset, (signed int)screenPoints.y[2] + Y_offset, (signed int)screenPoints.x[6] + X_offset, (signed int)screenPoints.y[6] + Y_offset, 1, Color);

	drawLine((signed int)screenPoints.x[5] + X_offset, (signed int)screenPoints.y[5] + Y_offset, (signed int)screenPoints.x[1] + X_offset, (signed int)screenPoints.y[1] + Y_offset, 1, Color);
	drawLine((signed int)screenPoints.x[5] + X_offset, (signed int)screenPoints.y[5] + Y_offset, (signed int)screenPoints.x[4] + X_offset, (signed int)screenPoints.y[4] + Y_offset, 1, Color);
	drawLine((signed int)screenPoints.x[5] + X_offset, (signed int)screenPoints.y[5] + Y_offset, (signed int)screenPoints.x[6] + X_offset, (signed int)screenPoints.y[6] + Y_offset, 1, Color);

	drawLine((signed int)screenPoints.x[7] + X_offset, (signed int)screenPoints.y[7] + Y_offset, (signed int)screenPoints.x[3] + X_offset, (signed int)screenPoints.y[3] + Y_offset, 1, Color);
	drawLine((signed int)screenPoints.x[7] + X_offset, (signed int)screenPoints.y[7] + Y_offset, (signed int)screenPoints.x[4] + X_offset, (signed int)screenPoints.y[4] + Y_offset, 1, Color);
	drawLine((signed int)screenPoints.x[7] + X_offset, (signed int)screenPoints.y[7] + Y_offset, (signed int)screenPoints.x[6] + X_offset, (signed int)screenPoints.y[6] + Y_offset, 1, Color);
}
//#######################################################################################
#if _USE_BITMAP_LIBRARY == 1
bool picturebox_put_bitmap(tPictureBox* settings, unsigned char *file, signed int X, signed int Y, bool use_transparency, bool scale)
{
	tDisplay *pDisplay = settings->Internals.pDisplay;
	return put_bitmap(pDisplay, file, settings->Internals.Position.X + X + 2, settings->Internals.Position.Y + Y + 2, use_transparency);
}
//#######################################################################################
bool picturebox_put_fbitmap(tPictureBox* settings, char *path, signed int X, signed int Y, bool use_transparency, bool scale)
{
	tDisplay *pDisplay = settings->Internals.pDisplay;
	return put_fbitmap(pDisplay, path, settings->Internals.Position.X + X + 2, settings->Internals.Position.Y + Y + 2, use_transparency);
}
#endif
//#######################################################################################
