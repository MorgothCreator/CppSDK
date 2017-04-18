/*
 *  lib/gfx/scrollbar.cpp
 */

#include <stdbool.h>
#include <stdlib.h>
#include "scrollbar.h"
#include "button.h"
#include "window.h"
#include "api/lcd_def.h"
#include "util.h"
#include <lib/util/converters.h>
#include "controls_definition.h"
#include "gfx_gui_paint.h"


void GI::Screen::Gfx::ScrollBar::paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand)
{
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window*)Internals.parentWindowHandler;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion.sXMin = x_start;
	LcdStruct->sClipRegion.sYMin = y_start;
	LcdStruct->sClipRegion.sXMax = x_start + x_len;
	LcdStruct->sClipRegion.sYMax = y_start + y_len;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	if(Internals.NeedEntireRefresh == true || Internals.NeedEntireRepaint == true || Internals.Control.Initiated == false)
	{
		if(Enabled)
			gui_put_item(pDisplay, x_start, y_start, x_len, y_len, Color.Enabled.BackGround, controls_color.Control_Color_Enabled_Border_Push, Cursor_Down,PAINT_STYLE_ROUNDED_CORNERS , true);
		else
			gui_put_item(pDisplay, x_start, y_start, x_len, y_len, Color.Disabled.BackGround, Color.Disabled.Border, Cursor_NoAction,PAINT_STYLE_ROUNDED_CORNERS , true);
		control_comand->WindowRefresh |= true;
	}

	gfx_s32 ValueBetweenMinAndMax = (Internals.OldMaximum - Internals.OldMinimum);


	GI::Screen::Gfx::Button* ButtonSettings = Internals.BtnSettings;
	GI::Screen::Gfx::Button* BtnUpSettings = Internals.BtnUpSettings;
	GI::Screen::Gfx::Button* BtnDnSettings = Internals.BtnDnSettings;

	if(Size.X < Size.Y)
	{
		BtnUpSettings->Position.X = 2;
		BtnUpSettings->Position.Y = 2;
		BtnUpSettings->Internals.PositionOffset.X = x_start - parentWindowHandler->Internals.Position.X;
		BtnUpSettings->Internals.PositionOffset.Y = y_start - parentWindowHandler->Internals.Position.Y;
		BtnUpSettings->Size.X = Internals.Size.X - 4;
		BtnUpSettings->Size.Y = Internals.Size.X - 4;

		BtnDnSettings->Position.X = 2;
		BtnDnSettings->Position.Y = Internals.Size.Y - (Internals.Size.X - 2);
		BtnDnSettings->Internals.PositionOffset.X = x_start - parentWindowHandler->Internals.Position.X;
		BtnDnSettings->Internals.PositionOffset.Y = y_start - parentWindowHandler->Internals.Position.Y;
		BtnDnSettings->Size.X = Internals.Size.X - 4;
		BtnDnSettings->Size.Y = Internals.Size.X - 4;
	}
	else
	{
		/* Left */
		BtnUpSettings->Position.X = 2;
		BtnUpSettings->Position.Y = 2;
		BtnUpSettings->Internals.PositionOffset.X = x_start - parentWindowHandler->Internals.Position.X;
		BtnUpSettings->Internals.PositionOffset.Y = y_start - parentWindowHandler->Internals.Position.Y;
		BtnUpSettings->Size.X = Internals.Size.Y - 4;
		BtnUpSettings->Size.Y = Internals.Size.Y - 4;
		/* Right */
		BtnDnSettings->Position.X = Internals.Size.X - (Internals.Size.Y - 2);
		BtnDnSettings->Position.Y = 2;
		BtnDnSettings->Internals.PositionOffset.X = x_start - parentWindowHandler->Internals.Position.X;
		BtnDnSettings->Internals.PositionOffset.Y = y_start - parentWindowHandler->Internals.Position.Y;
		BtnDnSettings->Size.X = Internals.Size.Y - 4;
		BtnDnSettings->Size.Y = Internals.Size.Y - 4;
	}

	if(control_comand->Cursor != Cursor_NoAction || Internals.NeedEntireRefresh == true || Internals.NeedEntireRepaint == true)
	{
		BtnUpSettings->Internals.NeedEntireRefresh = Internals.NeedEntireRefresh;
		BtnUpSettings->Enabled = Enabled;
		CursorState back = control_comand->Cursor;
		if(BtnUpSettings->Internals.NeedEntireRefresh) control_comand->Cursor = Cursor_Up;
		BtnUpSettings->idle(control_comand);
		//control_comand->Cursor = back;

		BtnDnSettings->Internals.NeedEntireRefresh = Internals.NeedEntireRefresh;
		BtnDnSettings->Enabled = Enabled;
		//CursorState back = control_comand->Cursor;
		//if(BtnDnSettings->Internals.NeedEntireRefresh) control_comand->Cursor = Cursor_Up;
		BtnDnSettings->idle(control_comand);
		control_comand->Cursor = back;
	}

	gfx_s32 BtnSize = 0;

	if(Size.X < Size.Y)
		BtnSize = Size.Y - ((Size.X - 2)<<1) - 4 - ValueBetweenMinAndMax;
	else
		BtnSize = Size.X - ((Size.Y - 2)<<1) - 4 - ValueBetweenMinAndMax;

	if(BtnSize < Size.MinBtnSize)
		BtnSize = Size.MinBtnSize;

	bool scroll_internal_modified = false;
	if(BtnUpSettings->Events.CursorDown || BtnUpSettings->Events.CursorMove || Internals.OldValue != Value)
	{
		BtnUpSettings->Events.CursorDown = false;
		BtnUpSettings->Events.CursorMove = false;
		if(Value > Minimum)
		{
			if(Internals.OldValue == Value)
			{
				Value--;
				scroll_internal_modified = true;
				//control_comand->WindowRefresh |= true;
				//settings->Internals.NeedEntireRefresh = true;
			}
			else
			{
				Internals.NeedEntireRefresh = false;
				//control_comand->CursorCoordonateUsed = true;
				//control_comand->WindowRefresh |= true;
			}
		}
	}
	if(BtnDnSettings->Events.CursorDown || BtnDnSettings->Events.CursorMove || Internals.OldValue != Value)
	{
		BtnDnSettings->Events.CursorDown = false;
		BtnDnSettings->Events.CursorMove = false;
		if(Value < Maximum)
		{
			if(Internals.OldValue == Value)
			{
				Value++;
				scroll_internal_modified = true;
				//control_comand->WindowRefresh |= true;
				//settings->Internals.NeedEntireRefresh = true;
			}
			else
			{
				Internals.NeedEntireRefresh = false;
				//control_comand->CursorCoordonateUsed = true;
				//control_comand->WindowRefresh |= true;
			}
		}
	}

	bool CursorBtnSelfModified = false;
	if(ValueBetweenMinAndMax < 1)
	{
		ValueBetweenMinAndMax = 1;
		if(Size.X < Size.Y)
		{
			ButtonSettings->Position.X = 2;
			ButtonSettings->Position.Y = Size.X;
			//ButtonSettings->Position.X = settings->Position.X + 2;
			//ButtonSettings->Position.Y = settings->Position.Y + 2 + (settings->Size.X - 2);
			ButtonSettings->Size.X = Size.X - 4;
			ButtonSettings->Size.Y = Size.Y - 4 - ((Size.X - 2)<<1);
		}
		else
		{
			ButtonSettings->Position.X = Size.Y;
			ButtonSettings->Position.Y = 2;
			//ButtonSettings->Position.X = settings->Position.X + 2 + (settings->Size.Y - 2);
			//ButtonSettings->Position.Y = settings->Position.Y + 2;
			ButtonSettings->Size.X = Size.X - 4 - ((Size.Y - 2)<<1);
			ButtonSettings->Size.Y = Size.Y - 4;
		}
	}
	else
	{
		if(control_comand->CursorCoordonateUsed == false&&
				Enabled == true &&
					Visible == true)
		{
			if(Size.X < Size.Y)
			{
				ButtonSettings->Size.Y = BtnSize;
				////gfx_s32 Steps = settings->Size.Y - 4 - BtnSize;
				//ButtonSettings->Position.X = settings->Position.X + 2;
				////ButtonSettings->Position.Y = settings->Position.Y + 2;
				ButtonSettings->Size.X = Size.X - 4;
				//ButtonSettings->Size.Y = settings->Size.Y - 4;
				if(ButtonSettings->Events.CursorDown == true)
				{
					Internals.CoordonateOfTouchDown = control_comand->Y;
					Internals.CoordonateOfButtonDown = ButtonSettings->Internals.Position.Y - Internals.PositionOffset.Y;
					ButtonSettings->Events.CursorDown = false;
				}
				else if(control_comand->Cursor == Cursor_Move && Internals.CursorDownInsideBox == true && ButtonSettings->Internals.CursorDownInsideBox == true)
				{
					ButtonSettings->Position.Y = Internals.CoordonateOfButtonDown + ((control_comand->Y - (parentWindowHandler->Internals.Position.Y)) - Internals.CoordonateOfTouchDown);

					if(ButtonSettings->Position.Y < Position.Y + (Size.X - 2) + 2)
						ButtonSettings->Position.Y = Position.Y + 2 + (Size.X - 2);
					else if((ButtonSettings->Position.Y + ButtonSettings->Size.Y) > (Position.Y + Size.Y) - 2  - (Size.X - 2))
						ButtonSettings->Position.Y = ((Position.Y + Size.Y) - 2) - ButtonSettings->Size.Y   - (Size.X - 2);

					Value = (gfx_s32)GUtil::Converters::percentTo((s32)Minimum, (s32)Maximum, (s32)(Size.Y - 4 - BtnSize - ((Size.X - 2)<<1)), (s32)(ButtonSettings->Position.Y - (Position.Y + 2) - (Size.X - 2)));

					CursorBtnSelfModified = true;
				}
			}
			else
			{
				ButtonSettings->Size.X = BtnSize;
				////gfx_s32 Steps = settings->Size.X - 4 - BtnSize;
				//ButtonSettings->Position.Y = settings->Position.Y + 2;
				////ButtonSettings->Position.X = settings->Position.X + 2;
				ButtonSettings->Size.Y = Size.Y - 4;
				//ButtonSettings->Size.X = settings->Size.X - 4;
				if(ButtonSettings->Events.CursorDown == true)
				{
					Internals.CoordonateOfTouchDown = control_comand->X;
					Internals.CoordonateOfButtonDown = ButtonSettings->Internals.Position.X;
					ButtonSettings->Events.CursorDown = false;
				}
				else if(control_comand->Cursor == Cursor_Move && Internals.CursorDownInsideBox == true && ButtonSettings->Internals.CursorDownInsideBox == true)
				{
					ButtonSettings->Position.X = Internals.CoordonateOfButtonDown + ((control_comand->X - (parentWindowHandler->Internals.Position.X)) - Internals.CoordonateOfTouchDown);

					if(ButtonSettings->Position.X < Position.X + (Size.Y - 2) + 2)
						ButtonSettings->Position.X = Position.X + 2 + (Size.Y - 2);
					else if((ButtonSettings->Position.X + ButtonSettings->Size.X) > (Position.X + Size.X) - 2  - (Size.Y - 2))
						ButtonSettings->Position.X = ((Position.X + Size.X) - 2) - ButtonSettings->Size.X   - (Size.Y - 2);

					Value = (gfx_s32)GUtil::Converters::percentTo((s32)Minimum, (s32)Maximum, (s32)(Size.X - 4 - BtnSize - ((Size.Y - 2)<<1)), (s32)(ButtonSettings->Position.X - (Position.X + 2) - (Size.Y - 2)));

					CursorBtnSelfModified = true;
				}
			}
		}
	}

	bool ValueIsChangedExternaly = false;
	if(Internals.OldValue != Value)
	{
		Internals.OldValue = Value;
		//settings->Events.ValueChanged = true;
		ValueIsChangedExternaly = true;
		Events.ValueChanged = true;
		if(Events.OnValueChanged.CallBack)
		{
			Events.OnValueChanged.CallbackReturnData = Events.OnValueChanged.CallBack(Events.OnValueChanged.CallbackData);
		}
	}

	//if(settings->Enabled) ButtonSettings->Color.Scren = settings->Color.Enabled.BackGround;
	//else  ButtonSettings->Color.Scren = settings->Color.Disabled.BackGround;
	ButtonSettings->Color.Scren = Color.Scren;
	if(BtnSize > Size.MinBtnSize)
	{
		if(Size.X < Size.Y)
			ButtonSettings->Position.Y = (Value + 2 + (Size.X - 2)) - Minimum;
		else
			ButtonSettings->Position.X = (Value + 2 + (Size.Y - 2)) - Minimum;
	}
	else
	{
		if(Size.X < Size.Y)
			ButtonSettings->Position.Y = 2 + (Size.X - 2) + (s32)GUtil::Converters::toPercent((s32)Minimum, (s32)Maximum, (s32)(Size.Y - ((Size.X - 2)<<1) - 4 - Size.MinBtnSize), (s32)Value);
		else
			ButtonSettings->Position.X = 2 + (Size.Y - 2) + (s32)GUtil::Converters::toPercent((s32)Minimum, (s32)Maximum, (s32)(Size.X - ((Size.Y - 2)<<1) - 4 - Size.MinBtnSize), (s32)Value);
	}

	if(Internals.NeedEntireRefresh == true || Internals.NeedEntireRepaint == true)
	{
		ButtonSettings->Internals.NoPaintBackGround = true;
		ButtonSettings->Internals.NeedEntireRefresh = true;
		ButtonSettings->Enabled = Enabled;
		CursorState back = control_comand->Cursor;
		control_comand->Cursor = Cursor_Up;
		ButtonSettings->idle(control_comand);
		control_comand->Cursor = back;
		ButtonSettings->Internals.NoPaintBackGround = false;
	}
	else if(ButtonSettings->Position.X + parentWindowHandler->Internals.Position.X + ButtonSettings->Internals.PositionOffset.X != ButtonSettings->Internals.Position.X ||
			ButtonSettings->Position.Y + parentWindowHandler->Internals.Position.Y + ButtonSettings->Internals.PositionOffset.Y != ButtonSettings->Internals.Position.Y)
	{
		ButtonSettings->Internals.NeedEntireRefresh = true;
		CursorState back = control_comand->Cursor;
		if(scroll_internal_modified)
			control_comand->Cursor = Cursor_Up;
		ButtonSettings->idle(control_comand);
		control_comand->Cursor = back;
		ButtonSettings->Internals.NoPaintBackGround = false;
	}
	else
	{
		CursorState back = control_comand->Cursor;
		if(CursorBtnSelfModified)
			control_comand->Cursor = Cursor_Down;
		if(ValueIsChangedExternaly)
		{
			ButtonSettings->Internals.NeedEntireRefresh = true;
			control_comand->Cursor = Cursor_Up;
		}
		ButtonSettings->idle(control_comand);
		control_comand->Cursor = back;
	}

	LcdStruct->sClipRegion.sXMin = x_start;
	LcdStruct->sClipRegion.sYMin = y_start;
	LcdStruct->sClipRegion.sXMax = x_start + x_len;
	LcdStruct->sClipRegion.sYMax = y_start + y_len;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	LcdStruct->cacheClean(x_start, y_start, x_len, y_len);
	LcdStruct->sClipRegion = back_up_clip;
	Internals.NeedEntireRefresh = false;
}
//#######################################################################################
void GI::Screen::Gfx::ScrollBar::idle(tControlCommandData* control_comand)
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
		Internals.OldMinimum = Minimum;
		Internals.OldMaximum = Maximum;
		Internals.OldValue = Value;

		Internals.BtnSettings = new GI::Screen::Gfx::Button(Internals.parentWindowHandler);
		GI::Screen::Gfx::Button* ButtonSettings = Internals.BtnSettings;
		ButtonSettings->Internals.NoPaintBackGround = false;
		ButtonSettings->Internals.ContinuouslyPushTimerDisabled = true;
		ButtonSettings->Enabled = Enabled;
		ButtonSettings->Color.Scren = Color.Scren;
		ButtonSettings->Color.Enabled.BackGround = Color.Enabled.BackGround;
		ButtonSettings->Caption->clear();

		Internals.BtnUpSettings = new GI::Screen::Gfx::Button(Internals.parentWindowHandler);
		GI::Screen::Gfx::Button* BtnUpSettings = Internals.BtnUpSettings;
		BtnUpSettings->Internals.NoPaintBackGround = true;
		BtnUpSettings->Enabled = Enabled;
		BtnUpSettings->Color.Scren = Color.Scren;
		BtnUpSettings->Color.Enabled.BackGround = Color.Enabled.BackGround;
		BtnUpSettings->Caption->clear();

		Internals.BtnDnSettings = new GI::Screen::Gfx::Button(Internals.parentWindowHandler);
		GI::Screen::Gfx::Button* BtnDnSettings = Internals.BtnDnSettings;
		BtnDnSettings->Internals.NoPaintBackGround = true;
		BtnDnSettings->Enabled = Enabled;
		BtnDnSettings->Color.Scren = Color.Scren;
		BtnDnSettings->Color.Enabled.BackGround = Color.Enabled.BackGround;
		BtnDnSettings->Caption->clear();

		if(Size.X < Size.Y)
		{
			Internals.BtnSettings->Position.X = 2;
			Internals.BtnSettings->Position.Y = Size.X;
			Internals.BtnSettings->Internals.PositionOffset.Y = Internals.Position.Y - parentWindowHandler->Internals.Position.Y;
			Internals.BtnSettings->Internals.PositionOffset.X = Internals.Position.X - parentWindowHandler->Internals.Position.X;
			Internals.BtnSettings->Size.X = Internals.Size.X - 4;
			//settings->Internals.BtnSettings->Size.Y = settings->Internals.Size.X - 4;

			BtnUpSettings->Position.X = 2;
			BtnUpSettings->Position.Y = 2;
			BtnUpSettings->Internals.PositionOffset.X = Internals.Position.X;
			BtnUpSettings->Internals.PositionOffset.Y = Internals.Position.Y;
			BtnUpSettings->Size.X = Internals.Size.X - 4;
			BtnUpSettings->Size.Y = Internals.Size.X - 4;

			BtnDnSettings->Position.X = 2;
			BtnDnSettings->Position.Y = Internals.Size.Y - (Internals.Size.X - 2);
			BtnDnSettings->Internals.PositionOffset.X = Internals.Position.X;
			BtnDnSettings->Internals.PositionOffset.Y = Internals.Position.Y;
			BtnDnSettings->Size.X = Internals.Size.X - 4;
			BtnDnSettings->Size.Y = Internals.Size.X - 4;
		}
		else
		{
			Internals.BtnSettings->Position.X = Size.Y;
			Internals.BtnSettings->Position.Y = 2;
			Internals.BtnSettings->Internals.PositionOffset.X = Internals.Position.X - parentWindowHandler->Internals.Position.X;
			Internals.BtnSettings->Internals.PositionOffset.Y = Internals.Position.Y - parentWindowHandler->Internals.Position.Y;
			//settings->Internals.BtnSettings->Size.X = settings->Internals.Size.Y - 4;
			Internals.BtnSettings->Size.Y = Internals.Size.Y - 4;
			/* Left */
			BtnUpSettings->Position.X = 2;
			BtnUpSettings->Position.Y = 2;
			BtnUpSettings->Internals.PositionOffset.X = Internals.Position.X;
			BtnUpSettings->Internals.PositionOffset.Y = Internals.Position.Y;
			BtnUpSettings->Size.X = Internals.Size.Y - 4;
			BtnUpSettings->Size.Y = Internals.Size.Y - 4;
			/* Right */
			BtnDnSettings->Position.X = Internals.Size.X - (Internals.Size.Y - 2);
			BtnDnSettings->Position.Y = 2;
			BtnDnSettings->Internals.PositionOffset.X = Internals.Position.X;
			BtnDnSettings->Internals.PositionOffset.Y = Internals.Position.Y;
			BtnDnSettings->Size.X = Internals.Size.Y - 4;
			BtnDnSettings->Size.Y = Internals.Size.Y - 4;
		}
		//settings->Internals.BtnSettings->Color.Scren = settings->Color.Enabled.Buton.Pull;
	}
	if(Value > Maximum)
		Value = Maximum;
	if(Value < Minimum)
		Value = Minimum;
	if(	Size.MinBtnSize < 14)
		Size.MinBtnSize = 14;

	/* Verify if position on size has been modified */
	if(parentWindowHandler)
	{
		if(((Position.X + parentWindowHandler->Internals.Position.X + Internals.PositionOffset.X) != Internals.Position.X ||
				(Position.Y + parentWindowHandler->Internals.Position.Y + Internals.PositionOffset.Y) != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							Size.MinBtnSize != Internals.Size.MinBtnSize ||
								Minimum != Internals.OldMinimum||
									Maximum != Internals.OldMaximum ||
										Value != Internals.OldValue ||
											Internals.OldStateEnabled != Enabled ||
											parentWindowHandler->Internals.OldStateEnabled != Internals.parentWindowStateEnabled) &&
													Visible == true)
														Internals.NeedEntireRefresh = true;
	}
	else
	{
		if((Position.X != Internals.Position.X ||
			Position.Y != Internals.Position.Y ||
				Size.X != Internals.Size.X ||
					Size.Y != Internals.Size.Y ||
						Size.MinBtnSize != Internals.Size.MinBtnSize ||
							Minimum != Internals.OldMinimum||
								Maximum != Internals.OldMaximum ||
									Internals.OldStateEnabled != Enabled ||
										Value != Internals.OldValue) &&
											Visible == true)
												Internals.NeedEntireRefresh = true;
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
			if(!Visible) return;
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
		Internals.Size.MinBtnSize = Size.MinBtnSize;
		Internals.OldMinimum = Minimum;
		Internals.OldMaximum = Maximum;
		if((Size.X == 0 || Size.Y == 0) && Internals.Control.Initiated == true )
			return;
		if(Value > Maximum)
			Value = Internals.OldMaximum;
		if(Value < Minimum)
			Value = Internals.OldMinimum;
		X_StartBox = Internals.Position.X;
		Y_StartBox = Internals.Position.Y;
		X_LenBox = Internals.Size.X;
		Y_LenBox = Internals.Size.Y;
		//CursorState _cursor = control_comand->Cursor;
		//control_comand->Cursor = Cursor_Up;
		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = X_StartBox;
		LcdStruct->sClipRegion.sYMin = Y_StartBox;
		LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
		LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);

		if(Size.X < Size.Y)
		{
			Internals.BtnSettings->Position.X = 2;
			Internals.BtnSettings->Position.Y = Size.X;
			Internals.BtnSettings->Internals.PositionOffset.Y = Internals.Position.Y - parentWindowHandler->Internals.Position.Y;
			Internals.BtnSettings->Internals.PositionOffset.X = Internals.Position.X - parentWindowHandler->Internals.Position.X;
		}
		else
		{
			Internals.BtnSettings->Position.X = Size.Y;
			Internals.BtnSettings->Position.Y = 2;
			Internals.BtnSettings->Internals.PositionOffset.X = Internals.Position.X - parentWindowHandler->Internals.Position.X;
			Internals.BtnSettings->Internals.PositionOffset.Y = Internals.Position.Y - parentWindowHandler->Internals.Position.Y;
		}
		if(Size.Y < Size.X)
		{
			if(	Size.X >= 60 && Size.Y >= 20)
				paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		}
		else
		{
			if(	Size.X >= 20 && Size.Y >= 60)
				paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		}

		LcdStruct->sClipRegion = back_up_clip;
		//control_comand->Cursor = _cursor;
		Internals.parentWindowStateEnabled = parentWindowHandler->Internals.OldStateEnabled;
		Internals.OldStateVisible = Visible;
		Internals.OldStateEnabled = Enabled;
		Internals.Control.Initiated = true;
		Internals.NeedEntireRefresh = false;
		Internals.NeedEntireRepaint = false;
		control_comand->WindowRefresh |= true;
	}
	if(Internals.OldStateCursor != control_comand->Cursor &&
					Enabled == true &&
						Visible == true)
	{
		Internals.OldStateCursor = control_comand->Cursor;
	}
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion.sXMin = X_StartBox;
	LcdStruct->sClipRegion.sYMin = Y_StartBox;
	LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
	LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	if(control_comand->Cursor)
	{
		if(Size.Y < Size.X)
		{
			if(	Size.X >= 60 && Size.Y >= 20)
				paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		}
		else
		{
			if(	Size.X >= 20 && Size.Y >= 60)
				paint(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		}
	}
	LcdStruct->sClipRegion = back_up_clip;
	bool inside_window = GI::Screen::Util::insideBox(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y);
	bool _inside_window = GI::Screen::Util::insideBox(LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMin, LcdStruct->sClipRegion.sXMax - LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMax - LcdStruct->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window)
		inside_window = false;
	if(inside_window == true && control_comand->Cursor == Cursor_Down)
		Internals.CursorDownInsideBox = true;
	if(Internals.CursorDownInsideBox == true && (control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction))
		Internals.CursorDownInsideBox = false;
	//control_comand->WindowRefresh |= true;
	//control_comand->CursorCoordonateUsed |= settings->Internals.CursorDownInsideBox;
}
//#######################################################################################
GI::Screen::Gfx::ScrollBar::ScrollBar(void *parentWindowHandler)
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

	Color.Disabled.BackGround = controls_color.Control_Color_Disabled_BackGround;
	Color.Disabled.Border = controls_color.Control_Color_Disabled_Border_Pull;
	Color.Disabled.Buton = controls_color.Control_Color_Disabled_Buton_Pull;

	Enabled = true;
	Position.X = 30;
	Position.Y = 100;
	Size.X = 20;
	Size.Y = 60;
	Size.MinBtnSize = 14;
	StateChangedOn = Cursor_Up;
	Visible = true;
}

void GI::Screen::Gfx::ScrollBar::idle()
{
	tControlCommandData control_comand;
	memset(&control_comand, 0, sizeof(tControlCommandData));
	idle(&control_comand);
}

GI::Screen::Gfx::ScrollBar::~ScrollBar()
{
	if(!this)
		return;

	Internals.BtnDnSettings->~Button();
	Internals.BtnSettings->~Button();
	Internals.BtnUpSettings->~Button();

	Visible = false;
	idle();
}
