/*
 *  lib/gfx/textbox.c
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
#include "textbox.h"
#include "window.h"
#include "controls_definition.h"
#include "api/lcd_def.h"
#include "util.h"
#include "scrollbar.h"
#include "graphic_string.h"



void GI::Screen::Gfx::TextBox::paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand)
{
	//if(control_comand->CursorCoordonateUsed && !settings->Internals.NeedEntireRefresh && !settings->Internals.NeedEntireRepaint) return;

	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window*)Internals.parentWindowHandler;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion.sXMin = x_start;
	LcdStruct->sClipRegion.sYMin = y_start;
	LcdStruct->sClipRegion.sXMax = x_start + x_len;
	LcdStruct->sClipRegion.sYMax = y_start + y_len;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	gfx_u32 color = Color.Enabled.Buton.Pull;
	if(!Enabled || !parentWindowHandler->Internals.OldStateEnabled)
		color = Color.Disabled.Buton;
	if(Internals.NeedEntireRefresh)
	{
		if((!Enabled || !parentWindowHandler->Internals.OldStateEnabled) && Internals.Control.Initiated == true) {
			LcdStruct->drawHLine(x_start, x_len, y_start, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
			LcdStruct->drawHLine(x_start + 1, x_len - 2, y_start + 1, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));

			LcdStruct->drawHLine(x_start + 1, x_len - 2, y_start + y_len - 2, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));
			LcdStruct->drawHLine(x_start, x_len, (y_start + y_len) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));

			LcdStruct->drawVLine(y_start, y_len, x_start, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
			LcdStruct->drawVLine(y_start + 1, y_len - 2, x_start + 1, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));

			LcdStruct->drawVLine(y_start + 1, y_len - 2, x_start + x_len - 2, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));
			LcdStruct->drawVLine(y_start, y_len, (x_start + x_len) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));

			LcdStruct->drawRectangle(x_start + 2, y_start + 2, x_len - 4 - Internals.Size.ScrollSize, y_len - 4 - Internals.Size.ScrollSize, true, Color.Disabled.BackGround);
		} else {
			LcdStruct->drawHLine(x_start, x_len, y_start, 1, controlls_change_color(color, +BORDER_LINE_ONE_LIGHT));
			LcdStruct->drawHLine(x_start + 1, x_len - 2, y_start + 1, 1, controlls_change_color(color, +BORDER_LINE_TWO_LIGHT));

			LcdStruct->drawHLine(x_start + 1, x_len - 2, y_start + y_len - 2, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));
			LcdStruct->drawHLine(x_start, x_len, (y_start + y_len) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));

			LcdStruct->drawVLine(y_start, y_len, x_start, 1, controlls_change_color(color, +BORDER_LINE_ONE_LIGHT));
			LcdStruct->drawVLine(y_start + 1, y_len - 2, x_start + 1, 1, controlls_change_color(color, +BORDER_LINE_TWO_LIGHT));

			LcdStruct->drawVLine(y_start + 1, y_len - 2, x_start + x_len - 2, 1, controlls_change_color(color, -BORDER_LINE_TWO_DARK));
			LcdStruct->drawVLine(y_start, y_len, (x_start + x_len) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));

			LcdStruct->drawRectangle(x_start + 2, y_start + 2, x_len - 4 - Internals.Size.ScrollSize, y_len - 4 - Internals.Size.ScrollSize, true, Color.Enabled.TextBackGround);
		}
		//put_rectangle(pDisplay, x_start, y_start, x_len, y_len, false, controlls_change_color(color, -3));
		//put_rectangle(pDisplay, x_start + 1, y_start + 1, x_len - 2, y_len - 2, false, controlls_change_color(color, -2));
		control_comand->WindowRefresh |= true;
		LcdStruct->sClipRegion.sXMin = x_start;
		LcdStruct->sClipRegion.sYMin = y_start;
		LcdStruct->sClipRegion.sXMax = x_start + x_len;
		LcdStruct->sClipRegion.sYMax = y_start + y_len;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		LcdStruct->cacheClean(x_start, y_start, x_len, y_len);
		LcdStruct->sClipRegion = back_up_clip;
	}
	if(text || text->length)
	{
		LcdStruct->sClipRegion.sXMin = x_start + 4;
		LcdStruct->sClipRegion.sYMin = y_start + 4;
		LcdStruct->sClipRegion.sXMax = ((x_start + x_len) - 4 - Internals.Size.ScrollSize);
		LcdStruct->sClipRegion.sYMax = ((y_start + y_len) - 4 - Internals.Size.ScrollSize);
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);

		gfx_s32 x_str_location = x_start + 4;
		gfx_s32 y_str_location = y_start + 4;

		gfx_s32 Start = SelStart;
		gfx_s32 Len = SelLen;
		gfx_u32 command_return = 0;
		bool CommandReceived = false;

		if(Internals.CursorDownInsideTextBox || Internals.NeedEntireRefresh)
		{
			if(Internals.CursorDownInsideTextBox && Internals.NeedEntireRefresh == false)
				text->getStrSelect(&Internals.Start, &Start, &Len, Internals.PenPushX, Internals.PenPushY, control_comand->X, control_comand->Y, x_str_location - Internals.Hscrollbar->Value, y_str_location - Internals.Vscrollbar->Value, &command_return);

			if((command_return & ReturnCommand_gm) == ReturnCommand_GoLeft && (((command_return & (~ReturnCommand_gm)) >> 1) != 0))
			{
				if(Internals.Hscrollbar->Value > Internals.Hscrollbar->Minimum)
				{
					Internals.Hscrollbar->Value -= (command_return & (~ReturnCommand_gm)) >> 1;
					if(Internals.Hscrollbar->Value < Internals.Hscrollbar->Minimum)
					{
						Internals.Hscrollbar->Value = Internals.Hscrollbar->Minimum;
					}
					CommandReceived = true;
				}
			}

			if((command_return & ReturnCommand_gm) == ReturnCommand_GoRight && (((command_return & (~ReturnCommand_gm)) >> 1) != 0))
			{
				if(Internals.Hscrollbar->Value < Internals.Hscrollbar->Maximum)
				{
					Internals.Hscrollbar->Value += (command_return & (~ReturnCommand_gm)) >> 1;
					if(Internals.Hscrollbar->Value > Internals.Hscrollbar->Maximum)
					{
						Internals.Hscrollbar->Value = Internals.Hscrollbar->Maximum;
					}
					CommandReceived = true;
				}
			}

			if((command_return & ReturnCommand_gm) == ReturnCommand_GoUp && (((command_return & (~ReturnCommand_gm)) >> 1) != 0))
			{
				if(Internals.Vscrollbar->Value > Internals.Vscrollbar->Minimum)
				{
					Internals.Vscrollbar->Value -= (command_return & (~ReturnCommand_gm)) >> 1;
					if(Internals.Vscrollbar->Value < Internals.Vscrollbar->Minimum)
					{
						Internals.Vscrollbar->Value = Internals.Vscrollbar->Minimum;
					}
					CommandReceived = true;
				}
			}

			if((command_return & ReturnCommand_gm) == ReturnCommand_GoDn && (((command_return & (~ReturnCommand_gm)) >> 1) != 0))
			{
				if(Internals.Vscrollbar->Value < Internals.Vscrollbar->Maximum)
				{
					Internals.Vscrollbar->Value += (command_return & (~ReturnCommand_gm)) >> 1;
					if(Internals.Vscrollbar->Value > Internals.Vscrollbar->Maximum)
					{
						Internals.Vscrollbar->Value = Internals.Vscrollbar->Maximum;
					}
					CommandReceived = true;
				}
			}

			if(CommandReceived || Internals.NeedEntireRefresh)
			{
				LcdStruct->sClipRegion = back_up_clip;
				tRectangle _back_up_clip = LcdStruct->sClipRegion;
				LcdStruct->sClipRegion.sXMin = x_start;
				LcdStruct->sClipRegion.sYMin = y_start;
				LcdStruct->sClipRegion.sXMax = x_start + x_len;
				LcdStruct->sClipRegion.sYMax = y_start + y_len;
				GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
				ControlCommands ComandBeck = control_comand->Comand;
				control_comand->Comand = Control_Refresh;
				CursorState cursor = control_comand->Cursor;
				control_comand->Cursor = Cursor_Up;
				if(((command_return & ReturnCommand_gm) == ReturnCommand_GoUp || (command_return & ReturnCommand_gm) == ReturnCommand_GoDn) || Internals.NeedEntireRefresh)
				{
					Internals.Vscrollbar->Internals.NoPaintBackGround = true;
					Internals.Vscrollbar->Enabled = Enabled;
					Internals.Vscrollbar->idle(control_comand);
				}
				if(((command_return & ReturnCommand_gm) == ReturnCommand_GoLeft || (command_return & ReturnCommand_gm) == ReturnCommand_GoRight) || Internals.NeedEntireRefresh)
				{
					Internals.Hscrollbar->Internals.NoPaintBackGround = true;
					Internals.Hscrollbar->Enabled = Enabled;
					Internals.Hscrollbar->idle(control_comand);
				}
				control_comand->Comand = ComandBeck;
				control_comand->Cursor = cursor;

				LcdStruct->sClipRegion = _back_up_clip;
			}
		}

		if(Start != SelStart || Len != SelLen || Internals.Hscrollbar->Events.ValueChanged == true || Internals.Vscrollbar->Events.ValueChanged == true || Internals.NeedEntireRefresh == true|| Internals.NeedEntireRepaint == true || CommandReceived != Control_Nop)
		{
			LcdStruct->sClipRegion.sXMin = x_start + 4;
			LcdStruct->sClipRegion.sYMin = y_start + 4;
			LcdStruct->sClipRegion.sXMax = ((x_start + x_len) - 4 - Internals.Size.ScrollSize);
			LcdStruct->sClipRegion.sYMax = ((y_start + y_len) - 4 - Internals.Size.ScrollSize);
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
			Internals.Vscrollbar->Events.ValueChanged = false;
			Internals.Hscrollbar->Events.ValueChanged = false;
			SelStart = Start;
			SelLen = Len;
			LcdStruct->drawRectangle(x_start + 2, y_start + 2, x_len - 4 - Internals.Size.ScrollSize, y_len - 4 - Internals.Size.ScrollSize, true, Color.Enabled.TextBackGround);

			text->ulOpaque = false;
			text->ulVisible = true;
			text->lX = x_str_location - Internals.Hscrollbar->Value;
			text->lY = y_str_location - Internals.Vscrollbar->Value;
			text->_SelStart = SelStart;
			text->_SelLen = SelLen;
			if(Enabled == true) {
				text->foreground_color = Color.Enabled.Ink.Pull;
				text->background_color = controlls_change_color(color, -2);
			}
			else {
				text->foreground_color = Color.Disabled.Ink;
				text->background_color = controlls_change_color(color, -2);
			}
			text->drawString();
			text->modifyed = false;
			control_comand->WindowRefresh |= true;
			LcdStruct->sClipRegion.sXMin = x_start;
			LcdStruct->sClipRegion.sYMin = y_start;
			LcdStruct->sClipRegion.sXMax = x_start + x_len;
			LcdStruct->sClipRegion.sYMax = y_start + y_len;
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
			LcdStruct->cacheClean(x_start, y_start, x_len, y_len);
		}
	}
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::TextBox::idle(tControlCommandData* control_comand)
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

		Internals.Vscrollbar = new GI::Screen::Gfx::ScrollBar(Internals.parentWindowHandler);
		Internals.Hscrollbar = new GI::Screen::Gfx::ScrollBar(Internals.parentWindowHandler);
	}

	if(	Size.X < 60)
		Size.X = 60;
	if(	Size.Y < 60)
		Size.Y = 60;

	/* Verify if position on size has been modified */
	if(parentWindowHandler)
	{
		if((Position.X + parentWindowHandler->Internals.Position.X + Internals.PositionOffset.X) != Internals.Position.X ||
				(Position.Y + parentWindowHandler->Internals.Position.Y + Internals.PositionOffset.Y) != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							Internals.Size.ScrollSize != Size.ScrollSize ||
								//settings->Internals.Caption.Font != settings->Font ||
									text->modifyed ||
										//settings->Internals.Caption.WordWrap != settings->WordWrap ||
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
							Internals.Size.ScrollSize != Size.ScrollSize ||
								//settings->Internals.Caption.Font != settings->Font ||
									text->modifyed ||
										//settings->Internals.Caption.WordWrap != settings->WordWrap ||
											Internals.OldStateEnabled != Enabled)
												Internals.NeedEntireRefresh = true;
	}

	//if(settings->Internals.Caption.Text != NULL && settings->Text != NULL && strcmp(settings->Internals.Caption.Text, settings->Text) == NULL)
		//settings->Internals.NeedEntireRefresh = true;

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
		Internals.Size.ScrollSize = Size.ScrollSize;
		//settings->Internals.Caption.Font = settings->Font;
		//settings->Internals.Caption.Text = settings->Text;
		//settings->Internals.Caption.WordWrap = settings->WordWrap;
		if(Size.X == 0 || Size.Y == 0)
			return;

		X_StartBox = Internals.Position.X;
		Y_StartBox = Internals.Position.Y;
		X_LenBox = Internals.Size.X;
		Y_LenBox = Internals.Size.Y;


		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = Internals.Position.X + 4;
		LcdStruct->sClipRegion.sYMin = Internals.Position.Y + 4;
		LcdStruct->sClipRegion.sXMax = ((Internals.Position.X + Internals.Size.X) - 4 - Internals.Size.ScrollSize);
		LcdStruct->sClipRegion.sYMax = ((Internals.Position.Y + Internals.Size.Y) - 4 - Internals.Size.ScrollSize);
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		StringProperties_t StrProperties = text->getStrProp();
		LcdStruct->sClipRegion = back_up_clip;

		Internals.Vscrollbar->Maximum = StrProperties.StringColsHeight_Pixels - (Internals.Size.Y - 6 - Size.ScrollSize);
		if(Internals.Vscrollbar->Maximum < 0)
			Internals.Vscrollbar->Maximum = 0;

		Internals.Vscrollbar->Position.X = Internals.Size.X - 2 - Size.ScrollSize;
		Internals.Vscrollbar->Position.Y = 2;
		Internals.Vscrollbar->Internals.PositionOffset.X = Internals.Position.X - parentWindowHandler->Internals.Position.X;
		Internals.Vscrollbar->Internals.PositionOffset.Y = Internals.Position.Y - parentWindowHandler->Internals.Position.Y;


		//settings->Internals.Vscrollbar->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - 2 - settings->Size.ScrollSize;
		//settings->Internals.Vscrollbar->Position.Y = settings->Internals.Position.Y + 2;
		Internals.Vscrollbar->Size.X = Size.ScrollSize;
		Internals.Vscrollbar->Size.Y = Internals.Size.Y - 4 - Size.ScrollSize;

		Internals.Hscrollbar->Maximum = StrProperties.StringRowsMaxLength_Pixels - (Internals.Size.X - 6 - Size.ScrollSize);
		if(Internals.Hscrollbar->Maximum < 0)
			Internals.Hscrollbar->Maximum = 0;

		Internals.Hscrollbar->Position.X = 2;
		Internals.Hscrollbar->Position.Y = Internals.Size.Y - 2 - Size.ScrollSize;
		Internals.Hscrollbar->Internals.PositionOffset.X = Internals.Position.X - parentWindowHandler->Internals.Position.X;
		Internals.Hscrollbar->Internals.PositionOffset.Y = Internals.Position.Y - parentWindowHandler->Internals.Position.Y;

		//settings->Internals.Hscrollbar->Position.X = settings->Internals.Position.X + 2;
		//settings->Internals.Hscrollbar->Position.Y = (settings->Internals.Position.Y + settings->Internals.Size.Y) - 2 - settings->Size.ScrollSize;
		Internals.Hscrollbar->Size.X = Internals.Size.X - 4 - Size.ScrollSize;
		Internals.Hscrollbar->Size.Y = Size.ScrollSize;

		back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = X_StartBox;
		LcdStruct->sClipRegion.sYMin = Y_StartBox;
		LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
		LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		if(Internals.NeedEntireRepaint)
			control_comand->Comand = Control_Refresh;
		Internals.Vscrollbar->Internals.NoPaintBackGround = true;
		Internals.Vscrollbar->Enabled = Enabled;
		Internals.Vscrollbar->idle(control_comand);
		Internals.Hscrollbar->Internals.NoPaintBackGround = true;
		Internals.Hscrollbar->Enabled = Enabled;
		Internals.Hscrollbar->idle(control_comand);

		//settings->Internals.Caption.Text = gfx_change_str(settings->Internals.Caption.Text, settings->Text);
		//settings->Text = settings->Internals.Caption.Text;
		CursorState _back = control_comand->Cursor;
		if(control_comand->CursorCoordonateUsed)
			control_comand->Cursor = Cursor_NoAction;
		paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		control_comand->Cursor = _back;
		LcdStruct->sClipRegion = back_up_clip;
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
	bool _inside_window = GI::Screen::Util::insideBox(X_StartBox + 2, Y_StartBox + 2, X_LenBox - 2 - Internals.Size.ScrollSize, Y_LenBox - 2 - Internals.Size.ScrollSize, control_comand->X, control_comand->Y);
	bool __inside_window = GI::Screen::Util::insideBox(LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMin, LcdStruct->sClipRegion.sXMax - LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMax - LcdStruct->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!__inside_window)
		_inside_window = false;
	if(_inside_window == true && control_comand->Cursor == Cursor_Down)
		Internals.CursorDownInsideTextBox = true;

	bool inside_window = GI::Screen::Util::insideBox(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y);
	__inside_window = GI::Screen::Util::insideBox(LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMin, LcdStruct->sClipRegion.sXMax - LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMax - LcdStruct->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!__inside_window)
		inside_window = false;
	if((control_comand->Cursor != Cursor_NoAction) &&
				(inside_window == true || Internals.CursorDownInsideBox == true) &&
					Enabled == true &&
						Visible == true &&
							control_comand->CursorCoordonateUsed == false)
	{
		Internals.OldStateCursor = control_comand->Cursor;

		if(control_comand->Cursor == Cursor_Down && inside_window == true)
		{
			Events.CursorDown = true;
			Internals.CursorDownInsideBox = true;
			Internals.PenPushX = control_comand->X;
			Internals.PenPushY = control_comand->Y;
			Internals.Start = -1;
			if(Events.OnDown.CallBack)
			{
				Events.OnDown.CallbackReturnData = Events.OnDown.CallBack(Events.OnDown.CallbackData);
			}
		}
		if(control_comand->Cursor == Cursor_Up && inside_window == true && Internals.CursorDownInsideBox == true)
		{
			Events.CursorUp = true;
			if(Events.OnUp.CallBack)
			{
				Events.OnUp.CallbackReturnData = Events.OnUp.CallBack(Events.OnUp.CallbackData);
			}
		}
		if(control_comand->Cursor == Cursor_Move && inside_window == true && Internals.CursorDownInsideBox == true)
		{
			Events.CursorMove = true;
			if(Events.OnMove.CallBack)
			{
				Events.OnMove.CallbackReturnData = Events.OnMove.CallBack(Events.OnMove.CallbackData);
			}
		}
		CursorState cursor = control_comand->Cursor;
		if(Internals.CursorDownInsideTextBox)
			control_comand->Cursor = Cursor_Up;
		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = X_StartBox;
		LcdStruct->sClipRegion.sYMin = Y_StartBox;
		LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
		LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		Internals.Vscrollbar->idle(control_comand);
		Internals.Hscrollbar->idle(control_comand);
		control_comand->Cursor = cursor;
		paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		LcdStruct->sClipRegion = back_up_clip;
	}
	if(control_comand->Cursor != Cursor_NoAction && inside_window && (Internals.CursorDownInsideBox/* || settings->Internals.CursorDownInsideTextBox*/))
		control_comand->CursorCoordonateUsed |= true;
	if(Internals.CursorDownInsideBox == true && (control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction))
		Internals.CursorDownInsideBox = false;
	if(control_comand->Cursor == Cursor_Up)
		Internals.CursorDownInsideTextBox = false;
	return;
}

void GI::Screen::Gfx::TextBox::idle()
{
	tControlCommandData control_comand;
	memset(&control_comand, 0, sizeof(tControlCommandData));
	idle(&control_comand);
}

GI::Screen::Gfx::TextBox::TextBox(void *parentWindowHandler)
{
	if(!parentWindowHandler)
		return;
	memset(this, 0, sizeof(*this));

	Internals.parentWindowHandler = parentWindowHandler;

	GI::Screen::Gfx::Window *_parentWindowHandler = (GI::Screen::Gfx::Window *)parentWindowHandler;
	Internals.pDisplay = _parentWindowHandler->Internals.pDisplay;

	GI::Screen::String *tmp_text = new GI::Screen::String(Internals.pDisplay);
	tmp_text->wordWrap = true;
	tmp_text->pFont = controls_color.DefaultFont;
	text = tmp_text;

	Color.Scren = controls_color.Scren;
	Color.Enabled.BackGround = controls_color.Control_Color_Enabled_TextBackGround;
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
	Position.Y = 50;
	Size.X = 70;
	Size.Y = 70;
	Size.ScrollSize = 20;
	StateChangedOn = Cursor_Up;
	Visible = true;
}
//#######################################################################################
GI::Screen::Gfx::TextBox::~TextBox()
{
	if(!this)
		return;
	tControlCommandData comand;
	memset(&comand, 0x00, sizeof(tControlCommandData));

	Visible = false;
	idle();
	return;
}
//#######################################################################################
bool GI::Screen::Gfx::TextBox::setText(char* string)
{
	if(!this)
		return false;
	text->setText(string);//str_copy(string);
	return true;
}
//#######################################################################################
bool GI::Screen::Gfx::TextBox::append(char* string)
{
	if(!this)
		return false;
	text->append(string);
	return true;
}
//#######################################################################################
bool GI::Screen::Gfx::TextBox::insert(char* string, gfx_u32 location)
{
	if(!this)
		return false;
	text->insert(string, location);
	return true;
}
//#######################################################################################
bool GI::Screen::Gfx::TextBox::toUpper()
{
	if(!this)
		return false;
	text->toUper();
	return true;
}
//#######################################################################################
bool GI::Screen::Gfx::TextBox::toLower()
{
	if(!this)
		return false;
	text->toLower();
	return true;
}
//#######################################################################################
bool GI::Screen::Gfx::TextBox::setFont(tFont* font)
{
	if(!this)
		return false;
	text->pFont = font;
	text->modifyed++;
	return true;
}
//#######################################################################################
bool GI::Screen::Gfx::TextBox::setWordWrap(bool word_wrap)
{
	if(!this)
		return false;
	text->wordWrap = word_wrap;
	text->modifyed++;
	return true;
}
//#######################################################################################
bool GI::Screen::Gfx::TextBox::setAlign(allign Align)
{
	if(!this)
		return false;
	text->textAlign = Align;
	text->modifyed++;
	return true;
}
