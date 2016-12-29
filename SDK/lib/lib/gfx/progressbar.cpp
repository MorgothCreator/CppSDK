/*
 *  lib/gfx/progressbar.c
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

#include <stdbool.h>
#include <stdlib.h>
#include "progressbar.h"
#include "window.h"
#include "controls_definition.h"
#include "api/lcd_def.h"
#include "util.h"
#include "graphic_string.h"
#include <lib/util/converters.h>
#include "sys/plat_properties.h"


void GI::Screen::Gfx::ProgressBar::paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand)
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
	color = controls_color.Control_Color_Enabled_Border_Pull;
	if((!Enabled || !parentWindowHandler->Internals.OldStateEnabled) && Internals.Control.Initiated == true) {
		color = Color.Disabled.Border;
		LcdStruct->drawHLine(x_start, x_len, y_start, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
		LcdStruct->drawHLine(x_start + 1, x_len - 2, y_start + 1, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));

		LcdStruct->drawHLine(x_start + 1, x_len - 2, y_start + y_len - 2, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));
		LcdStruct->drawHLine(x_start, x_len, (y_start + y_len) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));

		LcdStruct->drawVLine(y_start, y_len, x_start, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
		LcdStruct->drawVLine(y_start + 1, y_len - 2, x_start + 1, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));

		LcdStruct->drawVLine(y_start + 1, y_len - 2, x_start + x_len - 2, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));
		LcdStruct->drawVLine(y_start, y_len, (x_start + x_len) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
	}
	else {
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
	color = controls_color.Control_Color_Enabled_Buton_Pull;
	if(!Enabled || !parentWindowHandler->Internals.OldStateEnabled)
		color = Color.Disabled.Buton;
	LcdStruct->drawRectangle(x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, color);
	gfx_s32 Position = (gfx_s32)GUtil::Converters::toPercent((s32)MinimValue, (s32)MaximValue, (s32)(Size.X - 8), (s32)Value);
	LcdStruct->drawRectangle(x_start + 4, y_start + 4, Position, y_len - 8, true, controlls_change_color(color, -2));
	if(Caption || Caption->length)
	{
		LcdStruct->sClipRegion.sXMin = x_start + 4;
		LcdStruct->sClipRegion.sYMin = y_start + 4;
		LcdStruct->sClipRegion.sXMax = ((x_start + x_len) - 4);
		LcdStruct->sClipRegion.sYMax = ((y_start + y_len) - 4);
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		gfx_s32 x_str_location;
		gfx_s32 y_str_location;
		if(Caption->wordWrap)
		{
			StringProperties_t str_properties = Caption->getStrProp();
			x_str_location = x_start + ((x_len>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
			y_str_location = y_start + ((y_len>>1)-(str_properties.StringColsHeight_Pixels>>1));
		}else
		{
			x_str_location = x_start + 4;
			gfx_u8 CharHeight = Caption->getFontHeight();
			y_str_location = y_start + ((Internals.Size.Y>>1)-(CharHeight>>1));
		}
		Caption->ulOpaque = false;
		Caption->ulVisible = true;
		Caption->lX = x_str_location;
		Caption->lY = y_str_location;
		Caption->_SelStart = 0;
		Caption->_SelLen = 0;
		if(Enabled == true) {
			Caption->foreground_color = Color.Enabled.Ink.Pull;
			Caption->background_color = Color.Enabled.Buton.Pull;
		}
		else {
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
	LcdStruct->cacheClean(x_start, y_start, x_len, y_len);
	LcdStruct->sClipRegion = back_up_clip;
	control_comand->WindowRefresh |= true;
}
//#######################################################################################
void GI::Screen::Gfx::ProgressBar::idle(tControlCommandData* control_comand)
{
	if(this == NULL)
		return;
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window*)Internals.parentWindowHandler;
	if(control_comand->Comand != Control_Nop)
	{
		/* Parse commands */
#ifdef NO_ENUM_ON_SWITCH
		switch((gfx_u8)control_comand->Comand)
#else
		switch((gfx_s32)control_comand->Comand)
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
	/* Verify if position on size has been modified */
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
		Internals.OldMinimValue = MinimValue;
		Internals.OldMaximValue = MaximValue;
		Internals.OldValue = Value;
	}

	//gfx_s32 Position = to_percentage(settings->MinimValue, settings->MaximValue, settings->Size.X - 4, settings->Value);
	//gfx_s32 OldPosition = to_percentage(settings->MinimValue, settings->MaximValue, settings->Size.X - 4, settings->Internals.OldValue);
	//if(Position != OldPosition)
	if(Value != Internals.OldValue)
	{
		Internals.OldValue = Value;
		Internals.NeedEntireRepaint = true;
		if((control_comand->Cursor == Cursor_Move && StateChangedOn == Cursor_Move) ||
			(control_comand->Cursor == Cursor_Up && StateChangedOn == Cursor_Up) ||
				(control_comand->Cursor == Cursor_Down && StateChangedOn == Cursor_Down))
		{
			Events.ValueChanged = true;
			if(Events.OnValueChange.CallBack && Enabled && Visible)
				Events.OnValueChange.CallbackReturnData = Events.OnValueChange.CallBack(Events.OnValueChange.CallbackData);
		}
		//control_comand->CursorCoordonateUsed = true;
	}

	if(parentWindowHandler)
	{
		if((Position.X + parentWindowHandler->Internals.Position.X + Internals.PositionOffset.X) != Internals.Position.X ||
				(Position.Y + parentWindowHandler->Internals.Position.Y + Internals.PositionOffset.Y) != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							Internals.Caption->equal(Caption) == false ||
								Internals.OldMinimValue != MinimValue ||
									Internals.OldMaximValue != MaximValue ||
										Internals.OldValue != Value ||
											Internals.OldStateEnabled != Enabled ||
											parentWindowHandler->Internals.OldStateEnabled != Internals.parentWindowStateEnabled)
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
								Internals.OldMinimValue != MinimValue ||
									Internals.OldMaximValue != MaximValue ||
										Internals.OldValue != Value ||
											Internals.OldStateEnabled != Enabled)
		{
			Internals.NeedEntireRefresh = true;
			Internals.Caption->clone(Caption);
		}
	}

	gfx_s32 X_StartBox = Internals.Position.X;
	gfx_s32 Y_StartBox = Internals.Position.Y;
	gfx_s32 X_LenBox = Internals.Size.X;
	gfx_s32 Y_LenBox = Internals.Size.Y;
	void *pDisplay = Internals.pDisplay;
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
			LcdStruct->drawRectangle(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, true, Color.Scren);
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
		Internals.OldMinimValue = MinimValue;
		Internals.OldMaximValue = MaximValue;
		Internals.OldValue = Value;
		if(Size.X == 0 || Size.Y == 0)
			return;

		X_StartBox = Internals.Position.X;
		Y_StartBox = Internals.Position.Y;
		X_LenBox = Internals.Size.X;
		Y_LenBox = Internals.Size.Y;
		//settings->Internals.Caption.Text = gfx_change_str(settings->Internals.Caption.Text, settings->Caption.Text);
		//settings->Caption.Text = settings->Internals.Caption.Text;

		Internals.Caption->clone(Caption);
		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = X_StartBox;
		LcdStruct->sClipRegion.sYMin = Y_StartBox;
		LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
		LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		CursorState Cursor = control_comand->Cursor;
		control_comand->Cursor = Cursor_Up;
		paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		control_comand->Cursor = Cursor;
		LcdStruct->sClipRegion = back_up_clip;
		control_comand->WindowRefresh = true;

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

	if(control_comand->Cursor != Cursor_NoAction &&
			/*settings->Internals.OldStateCursor != control_comand->Cursor &&*/
				(inside_window == true || Internals.CursorDownInsideBox == true) &&
					Enabled == true &&
						Visible == true &&
							control_comand->CursorCoordonateUsed == false)
	{
		if(control_comand->Cursor == Cursor_Down && inside_window == true)
			Internals.CursorDownInsideBox = true;
		//settings->Internals.OldStateCursor = control_comand->Cursor;
		if(control_comand->Cursor == Cursor_Down && inside_window)
			Events.CursorDown = true;
		if(control_comand->Cursor == Cursor_Up && inside_window == true)
			Events.CursorUp = true;
		if(control_comand->Cursor == Cursor_Move && inside_window == true)
			Events.CursorMove = true;
		if(Events.OnDown.CallBack && control_comand->Cursor == Cursor_Down)
		{
			Events.OnDown.CallbackReturnData = Events.OnDown.CallBack(Events.OnDown.CallbackData);
		}
		if(Events.OnUp.CallBack && control_comand->Cursor == Cursor_Up && inside_window == true)
		{
			Events.OnUp.CallbackReturnData = Events.OnUp.CallBack(Events.OnUp.CallbackData);
		}
		if(Events.OnMove.CallBack && control_comand->Cursor == Cursor_Move && inside_window == true && Visible && Enabled == true)
		{
			Events.OnMove.CallbackReturnData = Events.OnMove.CallBack(Events.OnMove.CallbackData);
		}
		if((control_comand->Cursor == Cursor_Move/* && settings->StateChangedOn == Cursor_Move) ||
				(control_comand->Cursor == Cursor_Up && settings->StateChangedOn == Cursor_Up) ||
					(control_comand->Cursor == Cursor_Down && settings->StateChangedOn == Cursor_Down*/))
		{
			if(Internals.CursorDownInsideBox)
			{
				Value = GUtil::Converters::percentTo((s32)MinimValue, MaximValue, Size.X - 8, control_comand->X - (X_StartBox + 4));
				if(Value > MaximValue)
					Value = MaximValue;
				if(Value < MinimValue)
					Value = MinimValue;
			}
		}
		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = X_StartBox;
		LcdStruct->sClipRegion.sYMin = Y_StartBox;
		LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
		LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		if(Internals.CursorDownInsideBox)
			paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		LcdStruct->sClipRegion = back_up_clip;
	}
	if(control_comand->Cursor && Internals.CursorDownInsideBox)
		control_comand->CursorCoordonateUsed |= true;
	if(Internals.CursorDownInsideBox == true && (control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction))
		Internals.CursorDownInsideBox = false;
	//control_comand->CursorCoordonateUsed |= settings->Internals.CursorDownInsideBox;
	//control_comand->WindowRefresh |= true;
	return;
}

void GI::Screen::Gfx::ProgressBar::idle()
{
	tControlCommandData control_comand;
	memset(&control_comand, 0, sizeof(tControlCommandData));
	idle(&control_comand);
}

GI::Screen::Gfx::ProgressBar::ProgressBar(void *parentWindowHandler)
{
	if(!parentWindowHandler)
		return;
	memset(this, 0, sizeof(*this));
	Internals.parentWindowHandler = parentWindowHandler;

	GI::Screen::Gfx::Window *_parentWindowHandler = (GI::Screen::Gfx::Window *)parentWindowHandler;
	Internals.pDisplay = _parentWindowHandler->Internals.pDisplay;

	GI::Screen::String *tmp_text = new GI::Screen::String(Internals.pDisplay);
	tmp_text->textAlign = alignLeft;
	tmp_text->wordWrap = true;
	tmp_text->pFont = controls_color.DefaultFont;
	tmp_text->setText((char *)"Progressbar");
	Caption = tmp_text;

	tmp_text = new GI::Screen::String(Internals.pDisplay);
	Internals.Caption = tmp_text;
//settings->Caption.Text = malloc(sizeof("Textbox") + 1);
	//strcpy(settings->Caption.Text, "Textbox");

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
	Size.X = 100;
	Size.Y = 40;
	StateChangedOn = Cursor_Move;
	Visible = true;
	//settings->Value = 0;
	//settings->MinimValue = 0;
	MaximValue = 100;
}
//#######################################################################################
GI::Screen::Gfx::ProgressBar::~ProgressBar()
{
	if(!this)
		return;
	Visible = false;
	idle();
	if(Caption)
		delete Caption;
}
