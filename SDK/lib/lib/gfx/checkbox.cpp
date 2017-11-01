/*
 *  lib/gfx/checkbox.cpp
 */

#include <stdbool.h>
#include <stdlib.h>
#include "checkbox.h"
#include "checkbox.h"
#include "window.h"
#include "api/lcd_def.h"
#include "util.h"
#include "graphic_string.h"
#include "controls_definition.h"
#include "gfx_gui_paint.h"


void GI::Screen::Gfx::CheckBox::paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand)
{
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;
	gfx_u32 color = 0;
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window *)Internals.parentWindowHandler;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	CursorState cursor = control_comand->Cursor;
	if(Style == normal)
	{
		LcdStruct->sClipRegion.sXMin = x_start;
		LcdStruct->sClipRegion.sYMin = y_start;
		LcdStruct->sClipRegion.sXMax = x_start + x_len;
		LcdStruct->sClipRegion.sYMax = y_start + y_len;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		if((!Enabled || !parentWindowHandler->Internals.OldStateEnabled) && Internals.Control.Initiated == true)
			gui_put_item(pDisplay, x_start, y_start, x_len, y_len, Color.Disabled.Border, Color.Disabled.Border, cursor,PAINT_STYLE_ROUNDED_CORNERS , false);
		else {
			if(cursor == Cursor_Down || cursor == Cursor_Move)
				gui_put_item(pDisplay, x_start, y_start, x_len, y_len, controls_color.Control_Color_Enabled_Buton_Push, controls_color.Control_Color_Enabled_Border_Push, cursor,PAINT_STYLE_ROUNDED_CORNERS , true);
			else
				gui_put_item(pDisplay, x_start, y_start, x_len, y_len, controls_color.Control_Color_Enabled_Buton_Pull, controls_color.Control_Color_Enabled_Border_Pull, cursor,PAINT_STYLE_ROUNDED_CORNERS , true);
		}
		if(Cheched == true)
			LcdStruct->drawRectangle(x_start + 4, y_start + 4, y_len - 8, y_len - 8, true, controlls_change_color(color, -3));
		else
			LcdStruct->drawRectangle(x_start + 4, y_start + 4, y_len - 8, y_len - 8, false, controlls_change_color(color, -3));
	}
	else
	{
		LcdStruct->sClipRegion.sXMin = x_start;
		LcdStruct->sClipRegion.sYMin = y_start;
		LcdStruct->sClipRegion.sXMax = x_start + x_len;
		LcdStruct->sClipRegion.sYMax = y_start + y_len;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		if(Cheched == true) {
			gui_put_item(pDisplay, x_start, y_start, x_len, y_len, controls_color.Control_Color_Enabled_Buton_Push, controls_color.Control_Color_Enabled_Border_Push, cursor,PAINT_STYLE_ROUNDED_CORNERS , true);
		}
		else {
			gui_put_item(pDisplay, x_start, y_start, x_len, y_len, controls_color.Control_Color_Enabled_Buton_Pull, controls_color.Control_Color_Enabled_Buton_Pull, cursor,PAINT_STYLE_ROUNDED_CORNERS , true);
		}
		//LcdStruct->drawRectangle(x_start, y_start, x_len, y_len, false, controlls_change_color(color, -3));
		//LcdStruct->drawRectangle(x_start + 1, y_start + 1, x_len - 2, y_len - 2, false, controlls_change_color(color, -2));
		//LcdStruct->drawRectangle(x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, color);
	}
	if(Caption || Caption->length)
	{
		gfx_s32 x_str_location;
		gfx_s32 y_str_location;

		if(Caption->wordWrap)
		{
			StringProperties_t str_properties = Caption->getStrProp();
			if(Style == normal) {
				LcdStruct->sClipRegion.sXMin = x_start + y_len + 4;
				LcdStruct->sClipRegion.sYMin = y_start + 4;
				LcdStruct->sClipRegion.sXMax = ((x_start + x_len) - 4);
				LcdStruct->sClipRegion.sYMax = ((y_start + y_len) - 4);
				GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
				x_str_location = x_start + (((x_len - y_len)>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
			}
			else {
				LcdStruct->sClipRegion.sXMin = x_start + 4;
				LcdStruct->sClipRegion.sYMin = y_start + 4;
				LcdStruct->sClipRegion.sXMax = ((x_start + x_len) - 4);
				LcdStruct->sClipRegion.sYMax = ((y_start + y_len) - 4);
				GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
				x_str_location = x_start + ((x_len>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
			}
			y_str_location = y_start + ((y_len>>1)-(str_properties.StringColsHeight_Pixels>>1));
		}else
		{
			if(Style == normal) {
				LcdStruct->sClipRegion.sXMin = x_start + y_len + 4;
				LcdStruct->sClipRegion.sYMin = y_start + 4;
				LcdStruct->sClipRegion.sXMax = ((x_start + x_len) - 4);
				LcdStruct->sClipRegion.sYMax = ((y_start + y_len) - 4);
				GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
				x_str_location = x_start + y_len + 4;
				gfx_u8 CharHeight = Caption->getFontHeight();
				y_str_location = y_start + ((Internals.Size.Y>>1)-(CharHeight>>1));
			}
			else {
				LcdStruct->sClipRegion.sXMin = x_start + 4;
				LcdStruct->sClipRegion.sYMin = y_start + 4;
				LcdStruct->sClipRegion.sXMax = ((x_start + x_len) - 4);
				LcdStruct->sClipRegion.sYMax = ((y_start + y_len) - 4);
				GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
				x_str_location = x_start + 4;
				y_str_location = y_start + 4;
			}
		}
		Caption->ulOpaque = false;
		Caption->ulVisible = true;
		Caption->lX = x_str_location;
		Caption->lY = y_str_location;
		Caption->_SelStart = 0;
		Caption->_SelLen = 0;
		if(Enabled == true)
		{
			if(cursor == Cursor_Down || cursor == Cursor_Move) {
				Caption->foreground_color = Color.Enabled.Ink.Push;
				Caption->background_color = Color.Enabled.Buton.Push;
			}
			else if(cursor == Cursor_Up) {
				Caption->foreground_color = Color.Enabled.Ink.Pull;
				Caption->background_color = Color.Enabled.Buton.Pull;
			}
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
	//LcdStruct->cacheClean(x_start, y_start, x_len, y_len);
	LcdStruct->refresh();
	LcdStruct->sClipRegion = back_up_clip;
	control_comand->WindowRefresh |= true;
}

void GI::Screen::Gfx::CheckBox::idle(tControlCommandData* control_comand)
{
	if(this == NULL)
		return;
	tControlCommandData _control_comand_;
	if(!control_comand)
		control_comand = &_control_comand_;
	/* Verify if position on size has been modified */
	GI::Screen::Gfx::Window *parentWindowHandle = (GI::Screen::Gfx::Window*)Internals.parentWindowHandler;
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
		if(parentWindowHandle)
		{
			Internals.Position.X = Position.X + parentWindowHandle->Internals.Position.X + Internals.PositionOffset.X;
			Internals.Position.Y = Position.Y + parentWindowHandle->Internals.Position.Y + Internals.PositionOffset.Y;
		}
		else
		{
			Internals.Position.X = Position.X;
			Internals.Position.Y = Position.Y;
		}
		Internals.Size.X = Size.X;
		Internals.Size.Y = Size.Y;
	}

	if(	Size.Y < 14)
		Size.Y = 14;
	if(	Size.X < 14)
		Size.X = 14;

	if(parentWindowHandle)
	{
		if((Position.X + parentWindowHandle->Internals.Position.X + Internals.PositionOffset.X) != Internals.Position.X ||
				(Position.Y + parentWindowHandle->Internals.Position.Y + Internals.PositionOffset.Y) != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							Internals.Caption->equal(Caption) == false ||
								Internals.OldStateEnabled != Enabled ||
									Internals.OldStateVisible != Visible ||
									parentWindowHandle->Internals.OldStateEnabled != Internals.parentWindowStateEnabled)
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

	int X_StartBox = Internals.Position.X;
	int Y_StartBox = Internals.Position.Y;
	int X_LenBox = Internals.Size.X;
	int Y_LenBox = Internals.Size.Y;
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
			//LcdStruct->cacheClean(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
			LcdStruct->refresh();
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
		if(parentWindowHandle)
		{
			Internals.Position.X = Position.X + parentWindowHandle->Internals.Position.X + Internals.PositionOffset.X;
			Internals.Position.Y = Position.Y + parentWindowHandle->Internals.Position.Y + Internals.PositionOffset.Y;
		}
		else
		{
			Internals.Position.X = Position.X;
			Internals.Position.Y = Position.Y;
		}
		Internals.Size.X = Size.X;
		Internals.Size.Y = Size.Y;
		if(Size.X == 0 || Size.Y == 0)
			return;
		X_StartBox = Internals.Position.X;
		Y_StartBox = Internals.Position.Y;
		X_LenBox = Internals.Size.X;
		Y_LenBox = Internals.Size.Y;
		//settings->Internals.Caption.Text = gfx_change_str(settings->Internals.Caption.Text.text, settings->Caption.Text);
		//settings->Caption.Text = settings->Internals.Caption.Text;
		Internals.Caption->clone(Caption);

		CursorState Cursor = control_comand->Cursor;
		control_comand->Cursor = Cursor_Up;
		paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		control_comand->Cursor = Cursor;

		Internals.parentWindowStateEnabled = parentWindowHandle->Internals.OldStateEnabled;
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

	/*if((control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_Down) &&
			settings->Internals.OldStateCursor != control_comand->Cursor &&
				(inside_window == true || settings->Internals.CursorDownInsideBox == true) &&
				settings->Internals.OldStateCheched != settings->Cheched &&
					settings->Enabled == true &&
						settings->Visible == true &&
							control_comand->CursorCoordonateUsed == false)*/

		if(((control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_Down) &&
			Internals.OldStateCursor != control_comand->Cursor &&
				(inside_window == true || Internals.CursorDownInsideBox == true) &&
					Visible == true && Enabled == true) ||
						(Internals.OldStateCheched != Cheched &&
								Visible == true && Enabled == true &&
									control_comand->CursorCoordonateUsed == false))
	{
		//if(settings->Internals.OldStateCheched != settings->Cheched) //control_comand->WindowRefresh |= true;
		if(control_comand->Cursor == Cursor_Down && inside_window == true)
			Internals.CursorDownInsideBox = true;
		Internals.OldStateCursor = control_comand->Cursor;
		if(control_comand->Cursor == Cursor_Down && inside_window == true && Internals.CursorDownInsideBox == true)
			Events.CursorDown = true;
		if(control_comand->Cursor == Cursor_Up && inside_window == true && Internals.CursorDownInsideBox == true)
			Events.CursorUp = true;
		if(control_comand->Cursor == Cursor_Move && inside_window == true && Internals.CursorDownInsideBox == true)
			Events.CursorMove = true;
		if(Events.OnDown.CallBack && control_comand->Cursor == Cursor_Down && inside_window == true && Internals.CursorDownInsideBox == true)
		{
			Events.CursorDown = true;
			Events.OnDown.CallbackReturnData = Events.OnDown.CallBack(Events.OnDown.CallbackData);
		}
		if(Events.OnUp.CallBack && control_comand->Cursor == Cursor_Up && inside_window == true && Internals.CursorDownInsideBox == true)
		{
			Events.CursorUp = true;
			Events.OnUp.CallbackReturnData = Events.OnUp.CallBack(Events.OnUp.CallbackData);
		}

		if(control_comand->Cursor == Cursor_Down && StateChangedOn == Cursor_Down && inside_window == true && Internals.CursorDownInsideBox == true)
		{
			if(Cheched == true)
			{
				if(Style == normal)
					Cheched = false;
				if(Events.OnUnCheched.CallBack)
					Events.OnUnCheched.CallbackReturnData = Events.OnUnCheched.CallBack(Events.OnUnCheched.CallbackData);
				Events.UnChecked = true;
			}
			else
			{
				Cheched = true;
				if(Events.OnCheched.CallBack)
					Events.OnCheched.CallbackReturnData = Events.OnCheched.CallBack(Events.OnCheched.CallbackData);
				Events.Checked = true;
			}
		}
		if(control_comand->Cursor == Cursor_Up && StateChangedOn == Cursor_Up && inside_window == true && Internals.CursorDownInsideBox == true)
		{
			if(Cheched == true)
			{
				if(Style == normal)
					Cheched = false;
				if(Events.OnUnCheched.CallBack)
					Events.OnUnCheched.CallbackReturnData = Events.OnUnCheched.CallBack(Events.OnUnCheched.CallbackData);
				Events.UnChecked = true;
			}
			else
			{
				Cheched = true;
				if(Events.OnCheched.CallBack)
					Events.OnCheched.CallbackReturnData = Events.OnCheched.CallBack(Events.OnCheched.CallbackData);
				Events.Checked = true;
			}
		}
		Internals.OldStateCheched = Cheched;
		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = X_StartBox;
		LcdStruct->sClipRegion.sYMin = Y_StartBox;
		LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
		LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		LcdStruct->sClipRegion = back_up_clip;
	}
	if(Events.OnMove.CallBack && control_comand->Cursor == Cursor_Move && inside_window == true && Internals.CursorDownInsideBox == true && Visible && Enabled == true)
	{
		Events.CursorMove = true;
		Events.OnMove.CallbackReturnData = Events.OnMove.CallBack(Events.OnMove.CallbackData);
	}
	if(control_comand->Cursor && Internals.CursorDownInsideBox)
		control_comand->CursorCoordonateUsed |= true;
	if(Internals.CursorDownInsideBox == true && (control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction))
		Internals.CursorDownInsideBox = false;
	return;
}

void GI::Screen::Gfx::CheckBox::idle()
{
	tControlCommandData control_comand;
	memset(&control_comand, 0, sizeof(tControlCommandData));
	idle(&control_comand);
}

GI::Screen::Gfx::CheckBox::CheckBox(void *parentWindowHandler)
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
	tmp_text->setText((char *)"CheckBox");
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
	StateChangedOn = Cursor_Up;
	Visible = true;
}

GI::Screen::Gfx::CheckBox::~CheckBox()
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
