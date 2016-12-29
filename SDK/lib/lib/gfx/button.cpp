/*
 *  lib/gfx/buton.c
 *
 *  Copyright (C) 2016  Iulian Gheorghiu <morgoth.creator@gmail.com>
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

#include <stdbool.h>
#include <stdlib.h>
#include "button.h"
#include "window.h"
#include "api/lcd_def.h"
#include "util.h"
#include "api/timer.h"
#include "controls_definition.h"
#include "gfx_gui_paint.h"


void GI::Screen::Gfx::Button::paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand)
{
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window*)Internals.parentWindowHandler;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion.sXMin = x_start;
	LcdStruct->sClipRegion.sYMin = y_start;
	LcdStruct->sClipRegion.sXMax = x_start + x_len;
	LcdStruct->sClipRegion.sYMax = y_start + y_len;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	CursorState cursor = control_comand->Cursor;
	if((!Enabled || !parentWindowHandler->Internals.OldStateEnabled) && Internals.Control.Initiated == true)
		gui_put_item(pDisplay, x_start, y_start, x_len, y_len, Color.Disabled.Border, Color.Disabled.Border, cursor,PAINT_STYLE_ROUNDED_CORNERS , false);
	else {
		if(cursor == Cursor_Down || cursor == Cursor_Move)
			gui_put_item(pDisplay, x_start, y_start, x_len, y_len, controls_color.Control_Color_Enabled_Buton_Push, controls_color.Control_Color_Enabled_Border_Push, cursor,PAINT_STYLE_ROUNDED_CORNERS , true);
		else
			gui_put_item(pDisplay, x_start, y_start, x_len, y_len, controls_color.Control_Color_Enabled_Buton_Pull, controls_color.Control_Color_Enabled_Border_Pull, cursor,PAINT_STYLE_ROUNDED_CORNERS , true);
	}

	if(Caption || Caption->length)
	{
		LcdStruct->sClipRegion.sXMin = x_start + 2;
		LcdStruct->sClipRegion.sYMin = y_start + 2;
		LcdStruct->sClipRegion.sXMax = ((x_start + x_len) - 2);
		LcdStruct->sClipRegion.sYMax = ((y_start + y_len) - 2);
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);

		gfx_s32 x_str_location = LcdStruct->sClipRegion.sXMin;
		gfx_s32 y_str_location = LcdStruct->sClipRegion.sYMin;
		if(Caption->textAlign == alignCenter){
			StringProperties_t str_properties = Caption->getStrProp();
			x_str_location = x_start + ((x_len>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
			y_str_location = y_start + ((y_len>>1)-(str_properties.StringColsHeight_Pixels>>1));
		}
		Caption->ulOpaque = false;
		Caption->ulVisible = true;
		Caption->lX = x_str_location;
		Caption->lY = y_str_location;
		Caption->_SelStart = 0;
		Caption->_SelLen = 0;
		if(Enabled == true) {
			if(cursor == Cursor_Down || cursor == Cursor_Move) {
				Caption->foreground_color = Color.Enabled.Ink.Push;
				Caption->background_color = Color.Enabled.Buton.Push;
			}
			/*else if(cursor == Cursor_Up) {
				properties.foreground_color = settings->Color.Enabled.Ink.Pull;
				properties.background_color = settings->Color.Enabled.Buton.Pull;
			}*/
			else {
				Caption->foreground_color = Color.Enabled.Ink.Pull;
				Caption->background_color = Color.Enabled.Buton.Pull;
			}
		} else {
			Caption->foreground_color = Color.Disabled.Ink;
			Caption->background_color = Color.Disabled.Buton;
		}
		Caption->drawString();
	}
	LcdStruct->sClipRegion.sXMin = x_start;
	LcdStruct->sClipRegion.sYMin = y_start;
	LcdStruct->sClipRegion.sXMax = x_start + x_len;
	LcdStruct->sClipRegion.sYMax = y_start + y_len;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	LcdStruct->cacheClean( x_start, y_start, x_len, y_len);
	LcdStruct->sClipRegion = back_up_clip;
	control_comand->WindowRefresh |= true;
}

void GI::Screen::Gfx::Button::idle(tControlCommandData* control_comand)
{
	if(this == NULL)
		return;
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window *)Internals.parentWindowHandler;
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
							Internals.Caption->equal(Caption) == false ||
								Internals.OldStateEnabled != Enabled ||
									Internals.OldStateVisible != Visible ||
									parentWindowHandler->Internals.OldStateEnabled != Internals.parentWindowStateEnabled/* ||
														ParentWindow->Internals.OldStateVisible != settings->Internals.ParentWindowStateEnabled*/)
		{
			Internals.NeedEntireRefresh = true;
			Internals.Caption->clone(Caption);
		}
	}
	else
	{
		if(Position.X != Internals.Position.X ||
				Position.Y != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							Internals.Caption->equal(Caption) == false ||
								Internals.OldStateEnabled != Enabled ||
									Internals.OldStateVisible != Visible)
		{
			Internals.NeedEntireRefresh = true;
			Internals.Caption->clone(Caption);
		}
	}

	//CursorState cursor = control_comand->Cursor;
	//if(settings->Internals.NeedEntireRefresh) control_comand->Cursor = Cursor_Up;
	//if(settings->Internals.Caption.Text != NULL && settings->Caption.Text != NULL && strcmp(settings->Internals.Caption.Text, settings->Caption.Text) == NULL)
		//settings->Internals.NeedEntireRefresh = true;

	gfx_s32 X_StartBox = Internals.Position.X;
	gfx_s32 Y_StartBox = Internals.Position.Y;
	gfx_s32 X_LenBox = Internals.Size.X;
	gfx_s32 Y_LenBox = Internals.Size.Y;
	void *pDisplay = parentWindowHandler->Internals.pDisplay;
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;

	/*Clear background of box with actual painted dimensions and positions if they been changed*/
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
			LcdStruct->drawRectangle(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, true, Color.Enabled.BackGround);
			LcdStruct->cacheClean(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
			LcdStruct->sClipRegion = back_up_clip;
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
		if(Size.X == 0 || Size.Y == 0) return;
		X_StartBox = Internals.Position.X;
		Y_StartBox = Internals.Position.Y;
		X_LenBox = Internals.Size.X;
		Y_LenBox = Internals.Size.Y;
		Internals.Caption->clone(Caption);
		//gfx_change_str(settings->Internals.Caption.Text, settings->Caption.Text);
		//settings->Caption.Text = settings->Internals.Caption.Text;
		paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		Internals.parentWindowStateEnabled = parentWindowHandler->Internals.OldStateEnabled;
		Internals.OldStateVisible = Visible;
		Internals.OldStateEnabled = Enabled;
		Internals.Control.Initiated = true;
		Internals.NeedEntireRefresh = false;
		Internals.NeedEntireRepaint = false;
		control_comand->WindowRefresh |= true;
		return;
	}
	/* Check if inside window */
	bool inside_window = GI::Screen::Util::insideBox(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y);
	bool _inside_window = GI::Screen::Util::insideBox(LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMin, LcdStruct->sClipRegion.sXMax - LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMax - LcdStruct->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window)
		inside_window = false;
	if((control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_Down) &&
			Internals.OldStateCursor != control_comand->Cursor &&
				(inside_window == true || Internals.CursorDownInsideBox == true) &&
					Enabled == true &&
						Visible == true &&
							control_comand->CursorCoordonateUsed == false)
	{
		Internals.OldStateCursor = control_comand->Cursor;
		if(control_comand->Cursor == Cursor_Down && inside_window == true)
		{
			Internals.ContinuouslyPushTimer.interval(500);
			Internals.ContinuouslyPushTimer.enable(true);
			Events.CursorDown = true;
			Internals.CursorDownInsideBox = true;
			if(Events.OnDown.CallBack)
			{
				Events.OnDown.CallbackReturnData = Events.OnDown.CallBack(Events.OnDown.CallbackData);
			}
		}
		if(control_comand->Cursor == Cursor_Up && Internals.CursorDownInsideBox == true)
		{
			Internals.ContinuouslyPushTimer.enable(false);
			Events.CursorUp = true;
			if(Events.OnUp.CallBack)
			{
				Events.OnUp.CallbackReturnData = Events.OnUp.CallBack(Events.OnUp.CallbackData);
			}
		}
		paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
	}
	if(control_comand->Cursor == Cursor_Move && inside_window == true && Internals.CursorDownInsideBox == true && (Internals.ContinuouslyPushTimer.tick() || Internals.ContinuouslyPushTimerDisabled))
	{
		Internals.ContinuouslyPushTimer.interval(50);
		Internals.ContinuouslyPushTimer.enable(true);
		Events.CursorMove = true;
		if(Events.OnMove.CallBack)
		{
			Events.OnMove.CallbackReturnData = Events.OnMove.CallBack(Events.OnMove.CallbackData);
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

void GI::Screen::Gfx::Button::idle()
{
	tControlCommandData control_comand;
	memset(&control_comand, 0, sizeof(tControlCommandData));
	idle(&control_comand);
}

GI::Screen::Gfx::Button::Button(void *parentWindowHandler)
{
	if(!parentWindowHandler)
		return;
	memset(this, 0, sizeof(*this));

	Internals.parentWindowHandler = parentWindowHandler;

	GI::Screen::Gfx::Window *_parentWindowHandler = (GI::Screen::Gfx::Window *)parentWindowHandler;
	Internals.pDisplay = _parentWindowHandler->Internals.pDisplay;

	GI::Screen::String *tmp_text = new GI::Screen::String(Internals.pDisplay);
	tmp_text->textAlign =alignCenter;
	tmp_text->wordWrap = true;
	tmp_text->pFont = controls_color.DefaultFont;
	tmp_text->setText((char *)"Button");
	Caption = tmp_text;


	tmp_text = new GI::Screen::String(Internals.pDisplay);
	//tmp_text->clone(settings->Caption);
	Internals.Caption = tmp_text;

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
	StateChangedOn = Cursor_Up;
	Visible = true;
}

GI::Screen::Gfx::Button::~Button()
{
	if(!this)
		return;
	Visible = false;
	idle();
	if(Caption)
	{
		delete Caption;
	}
}
