/*
 *  lib/gfx/window.c
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
#include <stdlib.h>
#include "listbox.h"
#include "window.h"
#include "api/lcd_def.h"
#include "util.h"
#include "scrollbar.h"
#include "controls_definition.h"
#include "gfx_gui_paint.h"

bool GI::Screen::Gfx::ListBox::paint(listboxItem* settings, void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, tControlCommandData* control_comand, bool Pushed, bool Paint)
{
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window*)Internals.parentWindowHandler;
	bool inside_window = GI::Screen::Util::insideBox(x_start, y_start, settings->Size.X, settings->Size.Y, control_comand->X, control_comand->Y);
	bool _inside_window = GI::Screen::Util::insideBox(LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMin, LcdStruct->sClipRegion.sXMax - LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMax - LcdStruct->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window) inside_window = false;
	//if(!inside_window && !EntireRefresh && !settings->CursorDownInsideBox) return false;
	//if(inside_window == true/* && control_comand->Cursor == Cursor_Down*/) settings->CursorDownInsideBox = true;
	if(Paint)
	{
		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = x_start;
		LcdStruct->sClipRegion.sYMin = y_start;
		LcdStruct->sClipRegion.sXMax = x_start + settings->Size.X;
		LcdStruct->sClipRegion.sYMax = y_start + settings->Size.Y;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		if((!Enabled || !parentWindowHandler->Internals.OldStateEnabled) && Internals.Control.Initiated == true)
			gui_put_item(pDisplay, x_start, y_start, settings->Size.X, settings->Size.Y, settings->Color.Disabled.Buton, settings->Color.Disabled.Border, Cursor_NoAction,PAINT_STYLE_ROUNDED_CORNERS , false);
		else {
			if(Pushed == true)
				gui_put_item(pDisplay, x_start, y_start, settings->Size.X, settings->Size.Y, controls_color.Control_Color_Enabled_Buton_Push, controls_color.Control_Color_Enabled_Border_Push, Cursor_Down,PAINT_STYLE_ROUNDED_CORNERS , true);
			else
				gui_put_item(pDisplay, x_start, y_start, settings->Size.X, settings->Size.Y, controls_color.Control_Color_Enabled_Buton_Pull, controls_color.Control_Color_Enabled_Border_Pull, Cursor_Up,PAINT_STYLE_ROUNDED_CORNERS , true);
		}
		if(settings->Caption || settings->Caption->length)
		{
			LcdStruct->sClipRegion.sXMin = x_start + 4;
			LcdStruct->sClipRegion.sYMin = y_start + 4;
			LcdStruct->sClipRegion.sXMax = ((x_start + settings->Size.X) - 4);
			LcdStruct->sClipRegion.sYMax = ((y_start + settings->Size.Y) - 4);
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);

			gfx_s32 x_str_location = x_start + 4;
			gfx_s32 y_str_location = y_start + 4;
			if(settings->Caption->textAlign == alignCenter)
			{
				StringProperties_t str_properties = settings->Caption->getStrProp();
				x_str_location = x_start + ((settings->Size.X>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
				y_str_location = y_start + ((settings->Size.Y>>1)-(str_properties.StringColsHeight_Pixels>>1));
			}

			settings->Caption->ulOpaque = false;
			settings->Caption->ulVisible = true;
			settings->Caption->lX = x_str_location;
			settings->Caption->lY = y_str_location;
			settings->Caption->_SelStart = 0;
			settings->Caption->_SelLen = 0;
			if(Pushed == true) {
				settings->Caption->foreground_color = settings->Color.Enabled.Ink.Push;
				settings->Caption->background_color = settings->Color.Enabled.Buton.Push;
			}
			else {
				settings->Caption->foreground_color = settings->Color.Enabled.Ink.Pull;
				settings->Caption->background_color = settings->Color.Enabled.Buton.Pull;
			}
			settings->Caption->drawString();
		}
		LcdStruct->sClipRegion.sXMin = x_start;
		LcdStruct->sClipRegion.sYMin = y_start;
		LcdStruct->sClipRegion.sXMax = x_start + settings->Size.X;
		LcdStruct->sClipRegion.sYMax = y_start + settings->Size.Y;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		LcdStruct->cacheClean(x_start, y_start, settings->Size.X, settings->Size.Y);
		LcdStruct->sClipRegion = back_up_clip;
		control_comand->WindowRefresh |= true;
	}
	return inside_window;
}

void GI::Screen::Gfx::ListBox::idle(tControlCommandData* control_comand)
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
		Internals.ScrollBar = new GI::Screen::Gfx::ScrollBar(Internals.parentWindowHandler);
		GI::Screen::Gfx::ScrollBar *ScrollBar = Internals.ScrollBar;


		ScrollBar->Position.X = Internals.Size.X - 2 - Size.ScrollSize;
		ScrollBar->Position.Y = 2;
		ScrollBar->Internals.PositionOffset.X = Internals.Position.X - parentWindowHandler->Internals.Position.X;
		ScrollBar->Internals.PositionOffset.Y = Internals.Position.Y - parentWindowHandler->Internals.Position.Y;

		//ScrollBar->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - 2 - settings->Size.ScrollSize;
		//ScrollBar->Position.Y = settings->Internals.Position.Y + 2;
		ScrollBar->Size.X = Size.ScrollSize;
		ScrollBar->Size.Y = Internals.Size.Y - 4;
		ScrollBar->Maximum = ItemsCount - ((Size.Y - 4) / Size.ItemSizeY);
		ScrollBar->Internals.NoPaintBackGround = true;

	}
	/* Verify if position on size has been modified */
	if(parentWindowHandler)
	{
		if((Position.X + parentWindowHandler->Internals.Position.X + Internals.PositionOffset.X) != Internals.Position.X ||
				(Position.Y + parentWindowHandler->Internals.Position.Y + Internals.PositionOffset.Y) != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							Internals.Size.ItemSizeY != Size.ItemSizeY ||
								Internals.Size.ScrollSize != Size.ScrollSize ||
									Caption->modifyed ||
													Internals.OldStateEnabled != Enabled ||
														Internals.Size.MinScrollBtnSize != Size.MinScrollBtnSize ||
														parentWindowHandler->Internals.OldStateEnabled != Internals.parentWindowStateEnabled)
		{
			Internals.NeedEntireRefresh = true;
			Caption->modifyed = 0;
		}
	}
	else
	{
		if(Position.X != Internals.Position.X ||
				Position.Y != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							Internals.Size.ItemSizeY != Size.ItemSizeY ||
								Internals.Size.ScrollSize != Size.ScrollSize ||
									Caption->modifyed ||
													Internals.Size.MinScrollBtnSize != Size.MinScrollBtnSize ||
														Internals.OldStateEnabled != Enabled)
		{
			Internals.NeedEntireRefresh = true;
			Caption->modifyed = 0;
		}
	}
	//if(settings->Internals.Caption.Text != NULL && settings->Caption.Text != NULL && strcmp(settings->Internals.Caption.Text, settings->Caption.Text) == NULL)
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
	}

	/* Verify if is Entire refresh, entire repaint, or state changed */
	if((Internals.NeedEntireRefresh == true ||
			Internals.NeedEntireRepaint == true ||
				Internals.Control.Initiated == false ||
					//settings->Enabled == true ||
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
		Internals.Size.ItemSizeY = Size.ItemSizeY;
		Internals.Size.ScrollSize = Size.ScrollSize;

		//settings->Internals.ScrollBar->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - 2 - settings->Size.ScrollSize;
		//settings->Internals.ScrollBar->Position.Y = settings->Internals.Position.Y + 2;
		Internals.ScrollBar->Internals.PositionOffset.X = Internals.Position.X - parentWindowHandler->Internals.Position.X;
		Internals.ScrollBar->Internals.PositionOffset.Y = Internals.Position.Y - parentWindowHandler->Internals.Position.Y;
		Internals.ScrollBar->Size.X = Size.ScrollSize;
		Internals.ScrollBar->Size.Y = Internals.Size.Y - 4;
		Internals.ScrollBar->Maximum = ItemsCount - ((Size.Y - 4) / Size.ItemSizeY);
		Internals.ScrollBar->Enabled = Enabled;
		Internals.ScrollBar->Size.MinBtnSize = Size.MinScrollBtnSize;
		Internals.Size.MinScrollBtnSize = Size.MinScrollBtnSize;
		if(Size.X == 0 || Size.Y == 0)
			return;

		X_StartBox = Internals.Position.X;
		Y_StartBox = Internals.Position.Y;
		X_LenBox = Internals.Size.X;
		Y_LenBox = Internals.Size.Y;
		gfx_u32 CntDisplayItems;
		for(CntDisplayItems = 0; CntDisplayItems < ItemsCount ; CntDisplayItems++)
		{
			Items[CntDisplayItems]->Size.X = AlwaisShowScrollbar? Size.X - 4 : Size.X - Internals.Size.ScrollSize - 5;
			Items[CntDisplayItems]->Size.Y = Internals.Size.ItemSizeY;
		}

		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = X_StartBox;
		LcdStruct->sClipRegion.sYMin = Y_StartBox;
		LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
		LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		gfx_u32 color = controls_color.Control_Color_Enabled_Border_Pull;
		if((!Enabled || !parentWindowHandler->Internals.OldStateEnabled) && Internals.Control.Initiated == true) {
			color = Color.Disabled.Border;
			LcdStruct->drawHLine(X_StartBox, X_LenBox, Y_StartBox, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
			LcdStruct->drawHLine(X_StartBox, X_LenBox, (Y_StartBox + Y_LenBox) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));

			LcdStruct->drawVLine(Y_StartBox, Y_LenBox, X_StartBox, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
			LcdStruct->drawVLine(Y_StartBox, Y_LenBox, (X_StartBox + X_LenBox) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
		} else {
		//put_rectangle(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, false, controlls_change_color(color, -3));
			LcdStruct->drawHLine(X_StartBox, X_LenBox, Y_StartBox, 1, controlls_change_color(color, +BORDER_LINE_ONE_LIGHT));
			LcdStruct->drawHLine(X_StartBox, X_LenBox, (Y_StartBox + Y_LenBox) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));

			LcdStruct->drawVLine(Y_StartBox, Y_LenBox, X_StartBox, 1, controlls_change_color(color, +BORDER_LINE_ONE_LIGHT));
			LcdStruct->drawVLine(Y_StartBox, Y_LenBox, (X_StartBox + X_LenBox) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
		}


		if(Enabled)
			LcdStruct->drawRectangle(X_StartBox + 1, Y_StartBox + 1, X_LenBox - 2, Y_LenBox - 2, true, Color.Enabled.BackGround);
		else
			LcdStruct->drawRectangle(X_StartBox + 1, Y_StartBox + 1, X_LenBox - 2, Y_LenBox - 2, true, Color.Disabled.BackGround);

		LcdStruct->cacheClean(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);


		CursorState cursor = control_comand->Cursor;
		//if(!settings->Internals.IsChildren) control_comand->Cursor = Cursor_Up;
		for(CntDisplayItems = Internals.ItemStartOnBox; CntDisplayItems < ((Size.Y - 4) / Size.ItemSizeY) + Internals.ItemStartOnBox; CntDisplayItems++)
		{
			if(CntDisplayItems >= ItemsCount) break;
			bool Pushed = false;
			if(SelectedItem == CntDisplayItems)
				Pushed = true;
			else if(Internals.IsChildren == false || SelectedItem != CntDisplayItems)
				Pushed = false;
			paint(Items[CntDisplayItems], pDisplay, X_StartBox + 2, Y_StartBox + 2 + ((CntDisplayItems - Internals.ItemStartOnBox) * Size.ItemSizeY), control_comand, Pushed, true);
		}
		control_comand->Cursor = cursor;
		if(Enabled)
			Internals.ScrollBar->Color.Scren = Color.Enabled.BackGround;
		else
			Internals.ScrollBar->Color.Scren = Color.Disabled.BackGround;
		Internals.ScrollBar->Color.Enabled.BackGround = Color.Enabled.BackGround;;
		Internals.ScrollBar->Color.Disabled.BackGround = Color.Disabled.BackGround;;

		Internals.ScrollBar->Internals.NeedEntireRefresh = true;
		Internals.ScrollBar->idle(control_comand);

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

	Internals.ScrollBar->idle(control_comand);
	if(Internals.ScrollBar->Events.ValueChanged)
	{
		Internals.ScrollBar->Events.ValueChanged = false;
		Internals.ItemStartOnBox = Internals.ScrollBar->Value;
	}
	/* Check if inside window */
	bool inside_window = GI::Screen::Util::insideBox(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y);
	bool _inside_window = GI::Screen::Util::insideBox(LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMin, LcdStruct->sClipRegion.sXMax - LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMax - LcdStruct->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window)
		inside_window = false;
	if(control_comand->Cursor == Cursor_Down && inside_window == true)
		Internals.CursorDownInsideBox = true;

	if(((control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_Down) &&
			Internals.OldStateCursor != control_comand->Cursor &&
				Internals.CursorDownInsideBox == true &&
					Enabled == true &&
						Visible == true &&
							control_comand->CursorCoordonateUsed == false) ||
								Internals.ItemStartOnBox != Internals.OldItemStartOnBox)
	{
		Internals.OldStateCursor = control_comand->Cursor;

		gfx_u32 CntDisplayItems;
		bool items_position_has_changed = false;
		if(Internals.ItemStartOnBox != Internals.OldItemStartOnBox)
		{
			Internals.OldItemStartOnBox = Internals.ItemStartOnBox;
			items_position_has_changed = true;
		}
		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = X_StartBox;
		LcdStruct->sClipRegion.sYMin = Y_StartBox;
		LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
		LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		gfx_u32 EndDisplayedItems = ((Size.Y - 4) / Size.ItemSizeY) + Internals.ItemStartOnBox;
		for(CntDisplayItems = Internals.ItemStartOnBox; CntDisplayItems < EndDisplayedItems; CntDisplayItems++)
		{
			if(CntDisplayItems >= ItemsCount) break;
			if(items_position_has_changed)
			{
				bool Pushed = false;
				if(SelectedItem == CntDisplayItems)
					Pushed = true;
				else if(Internals.IsChildren == false || SelectedItem != CntDisplayItems)
					Pushed = false;
				paint(Items[CntDisplayItems], pDisplay, X_StartBox + 2, Y_StartBox + 2 + ((CntDisplayItems - Internals.ItemStartOnBox) * Size.ItemSizeY), control_comand, Pushed, true);
			}
			else
			{
				if( control_comand->Cursor == StateChangedOn && paint(Items[CntDisplayItems], pDisplay, X_StartBox + 2, Y_StartBox + 2 + ((CntDisplayItems - Internals.ItemStartOnBox) * Size.ItemSizeY), control_comand, false, false))
				{
					gfx_u32 _CntDisplayItems = CntDisplayItems;
					if(SelectedItem < EndDisplayedItems && SelectedItem >= Internals.ItemStartOnBox)
					{
						paint(Items[SelectedItem], pDisplay, X_StartBox + 2, Y_StartBox + 2 + ((SelectedItem - Internals.ItemStartOnBox) * Size.ItemSizeY), control_comand, false, true);
					}
					SelectedItem = _CntDisplayItems;
					paint(Items[_CntDisplayItems], pDisplay, X_StartBox + 2, Y_StartBox + 2 + ((_CntDisplayItems - Internals.ItemStartOnBox) * Size.ItemSizeY), control_comand, true, true);
					break;
				}
			}
		}
		LcdStruct->sClipRegion = back_up_clip;
	}
	if(control_comand->Cursor == Cursor_Down && inside_window == true && Internals.CursorDownInsideBox == true)
		Events.CursorDown = true;
	if(Events.OnDown.CallBack && control_comand->Cursor == Cursor_Down && inside_window == true && Internals.CursorDownInsideBox == true)
	{
		Events.OnDown.CallbackReturnData = Events.OnDown.CallBack(Events.OnDown.CallbackData);
	}
	if(control_comand->Cursor == Cursor_Up && inside_window == true && Internals.CursorDownInsideBox == true)
		Events.CursorUp = true;
	if(Events.OnUp.CallBack && control_comand->Cursor == Cursor_Up && inside_window == true && Internals.CursorDownInsideBox == true)
	{
		Events.OnUp.CallbackReturnData = Events.OnUp.CallBack(Events.OnUp.CallbackData);
	}
	if(control_comand->Cursor == Cursor_Move && inside_window == true)
		Events.CursorMove = true;
	if(Events.OnMove.CallBack && control_comand->Cursor == Cursor_Move && inside_window == true && Internals.CursorDownInsideBox == true)
	{
		Events.OnMove.CallbackReturnData = Events.OnMove.CallBack(Events.OnMove.CallbackData);
	}

	if(control_comand->Cursor && Internals.CursorDownInsideBox)
		control_comand->CursorCoordonateUsed |= true;
	if(control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction)
		Internals.CursorDownInsideBox = false;
	//control_comand->CursorCoordonateUsed = settings->Internals.CursorDownInsideBox;
	//control_comand->WindowRefresh |= true;
	return;
}
//#######################################################################################
bool GI::Screen::Gfx::ListBox::insert(char* text, gfx_u32 location)
{
	if(!this)
		return false;
	listboxItem* item_settings = (listboxItem*)calloc(1, sizeof(listboxItem));
	if(!item_settings)
		return false;

	memcpy((void*)&item_settings->Color, (void*)&Color, sizeof(item_settings->Color));

	//memcpy((void*)&item_settings->Caption, (void*)&settings->Caption, sizeof(item_settings->Caption));
	item_settings->Size.X = AlwaisShowScrollbar? Size.X - 4 : Size.X - Internals.Size.ScrollSize - 5;
	item_settings->Size.Y = Internals.Size.ItemSizeY;

	GI::Screen::String *tmp_text = new GI::Screen::String(Internals.pDisplay, text);
	tmp_text->textAlign = Caption->textAlign;
	//tmp_text->removeNewLine(item_settings->Caption);
	//tmp_text->clone(settings->Caption);
	//tmp_text->setText(text);
	item_settings->Caption = tmp_text;

	//item_settings->Caption.Font = settings->Internals.Caption.Font;
	//item_settings->Caption.TextAlign = settings->Internals.Caption.TextAlign;
	//item_settings->Caption.WordWrap = settings->Internals.Caption.WordWrap;

	//item_settings->Size.X = settings->Size.X;
	//item_settings->Size.Y = settings->Size.ItemSizeY;

	listboxItem **Tmp = (listboxItem **)realloc(Items, sizeof(void*) * (ItemsCount + 1));
	if(!Tmp)
	{
		//free(item_settings->Caption.Text.text);
		free(item_settings);
		return false;
	}
	gfx_u32 TmpCntItemsToMove = ItemsCount;
	for(; TmpCntItemsToMove > location; TmpCntItemsToMove --)
	{
		Tmp[TmpCntItemsToMove] = Tmp[TmpCntItemsToMove - 1];
	}
	Tmp[location] = item_settings;
	Items = Tmp;
	ItemsCount++;
	Internals.NeedEntireRefresh = true;
	return true;
}
//#######################################################################################
void GI::Screen::Gfx::ListBox::add(char* text)
{
	insert(text, ItemsCount);
}
//#######################################################################################
void GI::Screen::Gfx::ListBox::clear()
{
	if(!this)
		return;
	gfx_u32 TmpCntItemsToMove = ItemsCount;
	for(; TmpCntItemsToMove > 0; TmpCntItemsToMove--)
	{
		if(Items[TmpCntItemsToMove - 1]->Caption)
			delete Items[TmpCntItemsToMove - 1]->Caption;
		if(Items[TmpCntItemsToMove - 1])
			free(Items[TmpCntItemsToMove - 1]);
	}
	SelectedItem = 0;
	ItemsCount = 0;
	if(Items)
		free(Items);
	listboxItem ** TempPtr = (listboxItem **)calloc(1, sizeof(void*));
	if(TempPtr)
	{
		if(Items)
			free(Items);
		Items = TempPtr;
	}
	else
		Items[0] = NULL;
	Internals.ItemStartOnBox = 0;
	Internals.NeedEntireRefresh = true;
}
//#######################################################################################
bool GI::Screen::Gfx::ListBox::remove(gfx_u32 location)
{
	if(!this)
		return false;
	if(location >= ItemsCount)
		return false;
	if(Items[location]->Caption)
	{
		delete Items[location]->Caption;
	}
	if(Items[location])
		free(Items[location]);
	if(ItemsCount - 1 != 0)
	{
		gfx_u32 TmpCntItemsToMove = location +1;
		for(; TmpCntItemsToMove < ItemsCount; TmpCntItemsToMove++)
		{
			Items[TmpCntItemsToMove - 1] = Items[TmpCntItemsToMove];
		}
		listboxItem **Temp = (listboxItem **)realloc(Items, sizeof(void*) * (ItemsCount - 1));
		if(!Temp)
			return false;
		Items = Temp;
		ItemsCount--;
		if(SelectedItem > ItemsCount)
			SelectedItem = ItemsCount - 1;
		if(location < SelectedItem)
		{
			SelectedItem--;
			Internals.ScrollBar->Value = SelectedItem;
		}
		if(Internals.ItemStartOnBox)
			Internals.ItemStartOnBox--;
		Internals.NeedEntireRefresh = true;
	}
	else
		clear();
	return true;
}
//#######################################################################################
GI::Screen::Gfx::ListBox::ListBox(void *parentWindowHandler)
{
	if(!parentWindowHandler)
		return;
	memset(this, 0, sizeof(*this));
	Internals.parentWindowHandler = parentWindowHandler;

	GI::Screen::Gfx::Window *_parentWindowHandler = (GI::Screen::Gfx::Window *)parentWindowHandler;
	Internals.pDisplay = _parentWindowHandler->Internals.pDisplay;

	GI::Screen::String *tmp_txt = new GI::Screen::String(Internals.pDisplay);
	tmp_txt->textAlign = alignLeft;
	tmp_txt->wordWrap = true;
	tmp_txt->pFont = controls_color.DefaultFont;
	tmp_txt->setText((char *)"Item..");
	Caption = tmp_txt;
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

	Items = (listboxItem **)calloc(1, sizeof(void*));
	Enabled = true;
	Position.X = 50;
	Position.Y = 75;
	Size.X = 200;
	Size.Y = 60;
	Size.ItemSizeY = 20;
	StateChangedOn = Cursor_Up;
	Visible = true;
	Size.ScrollSize = 20;
}
//#######################################################################################
GI::Screen::Gfx::ListBox::~ListBox()
{
	if(!this)
		return;

	delete Internals.ScrollBar;

	tControlCommandData comand;
	memset(&comand, 0x00, sizeof(tControlCommandData));

	Visible = false;
	idle(&comand);

	gfx_u32 TmpCntItemsToMove = 0;
	for(; TmpCntItemsToMove > ItemsCount; TmpCntItemsToMove++)
	{
		if(Items[TmpCntItemsToMove]->Caption)
		{
			delete Items[TmpCntItemsToMove]->Caption;
		}
		if(Items[TmpCntItemsToMove])
			free(Items[TmpCntItemsToMove]);
	}

	if(Caption)
	{
		delete Caption;
	}
}
