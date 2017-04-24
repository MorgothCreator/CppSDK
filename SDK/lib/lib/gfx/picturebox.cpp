/*
 *  lib/gfx/picturebox.cpp
 */
#include <stdbool.h>
#include "picturebox.h"
#include "window.h"
#include "api/lcd_def.h"
#include "util.h"
#include "api/timer.h"
#include "3d.h"
#include "controls_definition.h"
#include <interface/screen.h>
#if _USE_BITMAP_LIBRARY == 1
#include "bmp.h"
#endif


void GI::Screen::Gfx::Picturebox::paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand)
{
	gfx_u32 color = 0;
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
		gfx_s32 X = control_comand->X;
		gfx_s32 Y = control_comand->Y;
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

	gfx_s32 X_StartBox = Internals.Position.X;
	gfx_s32 Y_StartBox = Internals.Position.Y;
	gfx_s32 X_LenBox = Internals.Size.X;
	gfx_s32 Y_LenBox = Internals.Size.Y;
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
			gfx_s32 X = control_comand->X;
			gfx_s32 Y = control_comand->Y;
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
				gfx_s32 X = control_comand->X;
				gfx_s32 Y = control_comand->Y;
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
				gfx_s32 X = control_comand->X;
				gfx_s32 Y = control_comand->Y;
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
				gfx_s32 X = control_comand->X;
				gfx_s32 Y = control_comand->Y;
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
	gfx_s32 X_StartBox = Internals.Position.X;
	gfx_s32 Y_StartBox = Internals.Position.Y;
	gfx_s32 X_LenBox = Internals.Size.X;
	gfx_s32 Y_LenBox = Internals.Size.Y;
	LcdStruct->sClipRegion.sXMin = X_StartBox;
	LcdStruct->sClipRegion.sYMin = Y_StartBox;
	LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
	LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	gfx_u32 color = BackgroundColor;
	if(!Enabled || !parentWindowHandler->Internals.OldStateEnabled)
		color = Color.Disabled.Buton;
	LcdStruct->drawRectangle(X_StartBox + 2, Y_StartBox + 2, X_LenBox - 4, Y_LenBox - 4, true, color);
	LcdStruct->cacheClean(X_StartBox + 2, Y_StartBox + 2, X_LenBox - 4, Y_LenBox - 4);
	LcdStruct->sClipRegion = back_up_clip;
}
/*//#######################################################################################
void picturebox_copy_rectangle(tPictureBox* settings, gfx_u32 *src_buff, gfx_s32 src_x_buff_size, gfx_s32 src_y_buff_size, gfx_s32 src_x_offset, gfx_s32 src_y_offset)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	//put_pixel(pDisplay, X + settings->Position.X + 2, Y + settings->Position.Y + 2, color);

	gfx_s32 Y_cnt = 0;
	gfx_s32 X_len = (pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin);
	if(X_len <= 0) return;
	for(; Y_cnt < src_y_offset + (pDisplay->sClipRegion.sYMax - pDisplay->sClipRegion.sYMin); Y_cnt++)
	{
		if(Y_cnt + pDisplay->sClipRegion.sYMin >= pDisplay->sClipRegion.sYMax) break;
		memcpy((void *)(pDisplay->DisplayData + 8 + ((Y_cnt + settings->Position.Y + 2) * pDisplay->Width) + settings->Position.X + 2), (void *)((char *)(src_buff + ((Y_cnt + src_y_offset) * src_x_buff_size)) - 1), X_len * sizeof(pDisplay->DisplayData[0]));
		CacheDataCleanInvalidateBuff((gfx_u32)(void *)(pDisplay->DisplayData + 8 + ((Y_cnt + settings->Position.Y + 2) * pDisplay->Width) + settings->Position.X + 2), X_len * sizeof(pDisplay->DisplayData[0]) + 64);
	}
	pDisplay->sClipRegion = back_up_clip;
}*/
//#######################################################################################
void GI::Screen::Gfx::Picturebox::copyRectangle(gfx_u32 *src_buff, gfx_u32 src_buff_data_offset, tRectangle *_dest_rectangle, tRectangle *_src_rectangle, gfx_s32 src_width, gfx_s32 src_height)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* pDisplay = Internals.pDisplay;
	//tRectangle back_up_clip = pDisplay->sClipRegion;
	//pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	////put_pixel(pDisplay, X + settings->Position.X + 2, Y + settings->Position.Y + 2, color);

	tRectangle *dest_rectangle = _dest_rectangle;
	//tRectangle *src_rectangle = _src_rectangle;

	gfx_s32 dest_X_StartBox = Internals.Position.X;
	gfx_s32 dest_Y_StartBox = Internals.Position.Y;
	//gfx_s32 dest_X_EndBox = settings->Internals.Position.X + settings->Internals.Size.X;
	//gfx_s32 dest_Y_EndBox = settings->Internals.Position.Y + settings->Internals.Size.Y;

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
	pDisplay->copyRectangle32Bit((gfx_u8 *)src_buff, dest_rectangle->sXMin, dest_rectangle->sYMin, dest_rectangle->sXMax - dest_rectangle->sXMin, dest_rectangle->sYMax - dest_rectangle->sYMin);
	//Limit the destination area to the picture box size.
	/*GScreen::Util::clipLimit(dest_rectangle, &settings->Internals.PictureWindowLimits);

	gfx_s32 x_line_len = dest_rectangle->sXMax - dest_rectangle->sXMin;

	if(x_line_len <= 0) return;
	if(dest_X_StartBox <= dest_rectangle->sXMin)
		dest_X_StartBox = dest_rectangle->sXMin;

	gfx_s32 Y_cnt = dest_Y_StartBox;
	if(Y_cnt <= dest_rectangle->sYMin)
		Y_cnt = dest_rectangle->sYMin;

	gfx_s32 X_Start_Src_Buff = (settings->Internals.Position.X + 2) - settings->Internals.PictureWindowLimits.sXMin;
	if(X_Start_Src_Buff > 0)
		X_Start_Src_Buff = 0;
	else
		X_Start_Src_Buff = (~X_Start_Src_Buff) + 1;
	gfx_s32 Y_Start_Src_Buff = (settings->Internals.Position.Y + 2) - settings->Internals.PictureWindowLimits.sYMin;
	if(Y_Start_Src_Buff > 0)
		Y_Start_Src_Buff = 0;
	else
		Y_Start_Src_Buff = (~Y_Start_Src_Buff) + 1;

	for(; Y_cnt < dest_rectangle->sYMax; Y_cnt++)
	{
		//if(Y_cnt >= pDisplay->sClipRegion.sYMax) break;
		memcpy((void *)(pDisplay->DisplayData + pDisplay->LcdTimings->palete_len + (Y_cnt * pDisplay->LcdTimings->X) + dest_X_StartBox), (void *)((char *)(src_buff + src_buff_data_offset + (((Y_cnt - settings->Internals.PictureWindowLimits.sYMin + Y_Start_Src_Buff) + src_rectangle->sYMin) * src_width) + src_rectangle->sXMin + X_Start_Src_Buff) - 1), x_line_len * sizeof(pDisplay->DisplayData[0]));
		//CacheDataCleanInvalidateBuff((gfx_u32)(void *)(pDisplay->DisplayData + 8 + ((Y_cnt + settings->Position.Y + 2) * pDisplay->Width) + settings->Position.X + 2), X_len * sizeof(pDisplay->DisplayData[0]) + 64);
	}*/
	//pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawPixel(gfx_s32 X, gfx_s32 Y, gfx_u32 color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawPixel(X + Internals.Position.X + 2, Y + Internals.Position.Y + 2, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawHLine(gfx_s32 X1, gfx_s32 X2, gfx_s32 Y, gfx_u8 width, gfx_u32 color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawHLine(X1 + Internals.Position.X + 2, X2 + Internals.Position.X + 2, Y + Internals.Position.Y + 2, width, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawVLine(gfx_s32 Y1, gfx_s32 Y2, gfx_s32 X, gfx_u8 width, gfx_u32 color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawVLine(Y1 + Internals.Position.Y + 2, Y2 + Internals.Position.Y + 2, X + Internals.Position.X + 2, width, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawCircle(gfx_s32 x, gfx_s32 y, gfx_s32 radius, gfx_u8 fill, gfx_u32 color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawCircle(x + Internals.Position.X + 2, y + Internals.Position.Y + 2, radius, fill, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawLine(gfx_s32 X1, gfx_s32 Y1, gfx_s32 X2, gfx_s32 Y2, gfx_u8 width, gfx_u32 color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawLine(X1 + Internals.Position.X + 2, Y1 + Internals.Position.Y + 2, X2 + Internals.Position.X + 2, Y2 + Internals.Position.Y + 2, width, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawElipse(gfx_s32 xc,gfx_s32 yc,gfx_s32 rx,gfx_s32 ry, gfx_u8 fill, gfx_u32 color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	GI::Dev::Screen* LcdStruct = Internals.pDisplay;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion = Internals.PictureWindowLimits;
	LcdStruct->drawElipse(xc + Internals.Position.X + 2, yc + Internals.Position.Y + 2, rx + Internals.Position.X + 2, ry + Internals.Position.Y + 2, fill, color);
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::drawTriangle(gfx_s32  Ax,gfx_s32  Ay,gfx_s32  Bx,gfx_s32  By,gfx_s32  Cx,gfx_s32  Cy, gfx_u8 fill, gfx_u32 color)
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
	gfx_s32 lX = string->lX;
	gfx_s32 lY = string->lY;
	string->lX += Internals.Position.X + 2;
	string->lY += Internals.Position.Y + 2;
	string->drawString();
	string->lX = lX;
	string->lY = lY;
	LcdStruct->sClipRegion = back_up_clip;
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::draw3dTriangle(_3d_points *Points, gfx_s32 X_offset, gfx_s32 Y_offset, double X_Angle, double Y_Angle, double Z_Angle, gfx_u32 Color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	_3d_points screenPoints;

	axisRotations cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 4;
	Transform3DPointsTo2DPoints(&screenPoints, Points, cubeAxisRotations);

	gfx_u32 color = Color;//(controlls_change_color(Color, (-triangle_median))<<8) & 0xFFFFFF00;

	gfx_s32 X_start = (gfx_s32)screenPoints.x[1];
	gfx_s32 Y_start = (gfx_s32)screenPoints.y[1];
	gfx_s32 X_end = (gfx_s32)screenPoints.x[2];
	gfx_s32 Y_end = (gfx_s32)screenPoints.y[2];
	drawLine(X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (gfx_s32)screenPoints.x[2];
	Y_start = (gfx_s32)screenPoints.y[2];
	X_end = (gfx_s32)screenPoints.x[3];
	Y_end = (gfx_s32)screenPoints.y[3];
	drawLine(X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (gfx_s32)screenPoints.x[3];
	Y_start = (gfx_s32)screenPoints.y[3];
	X_end = (gfx_s32)screenPoints.x[1];
	Y_end = (gfx_s32)screenPoints.y[1];
	drawLine(X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);
}
//#######################################################################################
void GI::Screen::Gfx::Picturebox::draw3dCube(_3d_points *Points, gfx_s32 X_offset, gfx_s32 Y_offset, double X_Angle, double Y_Angle, double Z_Angle, gfx_u32 Color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	_3d_points screenPoints;

	axisRotations cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 8;
	Transform3DPointsTo2DPoints(&screenPoints, Points, cubeAxisRotations);
	drawLine((gfx_s32)screenPoints.x[0] + X_offset, (gfx_s32)screenPoints.y[0] + Y_offset, (gfx_s32)screenPoints.x[1] + X_offset, (gfx_s32)screenPoints.y[1] + Y_offset, 1, Color);
	drawLine((gfx_s32)screenPoints.x[0] + X_offset, (gfx_s32)screenPoints.y[0] + Y_offset, (gfx_s32)screenPoints.x[3] + X_offset, (gfx_s32)screenPoints.y[3] + Y_offset, 1, Color);
	drawLine((gfx_s32)screenPoints.x[0] + X_offset, (gfx_s32)screenPoints.y[0] + Y_offset, (gfx_s32)screenPoints.x[4] + X_offset, (gfx_s32)screenPoints.y[4] + Y_offset, 1, Color);

	drawLine((gfx_s32)screenPoints.x[2] + X_offset, (gfx_s32)screenPoints.y[2] + Y_offset, (gfx_s32)screenPoints.x[1] + X_offset, (gfx_s32)screenPoints.y[1] + Y_offset, 1, Color);
	drawLine((gfx_s32)screenPoints.x[2] + X_offset, (gfx_s32)screenPoints.y[2] + Y_offset, (gfx_s32)screenPoints.x[3] + X_offset, (gfx_s32)screenPoints.y[3] + Y_offset, 1, Color);
	drawLine((gfx_s32)screenPoints.x[2] + X_offset, (gfx_s32)screenPoints.y[2] + Y_offset, (gfx_s32)screenPoints.x[6] + X_offset, (gfx_s32)screenPoints.y[6] + Y_offset, 1, Color);

	drawLine((gfx_s32)screenPoints.x[5] + X_offset, (gfx_s32)screenPoints.y[5] + Y_offset, (gfx_s32)screenPoints.x[1] + X_offset, (gfx_s32)screenPoints.y[1] + Y_offset, 1, Color);
	drawLine((gfx_s32)screenPoints.x[5] + X_offset, (gfx_s32)screenPoints.y[5] + Y_offset, (gfx_s32)screenPoints.x[4] + X_offset, (gfx_s32)screenPoints.y[4] + Y_offset, 1, Color);
	drawLine((gfx_s32)screenPoints.x[5] + X_offset, (gfx_s32)screenPoints.y[5] + Y_offset, (gfx_s32)screenPoints.x[6] + X_offset, (gfx_s32)screenPoints.y[6] + Y_offset, 1, Color);

	drawLine((gfx_s32)screenPoints.x[7] + X_offset, (gfx_s32)screenPoints.y[7] + Y_offset, (gfx_s32)screenPoints.x[3] + X_offset, (gfx_s32)screenPoints.y[3] + Y_offset, 1, Color);
	drawLine((gfx_s32)screenPoints.x[7] + X_offset, (gfx_s32)screenPoints.y[7] + Y_offset, (gfx_s32)screenPoints.x[4] + X_offset, (gfx_s32)screenPoints.y[4] + Y_offset, 1, Color);
	drawLine((gfx_s32)screenPoints.x[7] + X_offset, (gfx_s32)screenPoints.y[7] + Y_offset, (gfx_s32)screenPoints.x[6] + X_offset, (gfx_s32)screenPoints.y[6] + Y_offset, 1, Color);
}
//#######################################################################################
#if _USE_BITMAP_LIBRARY == 1
bool picturebox_put_bitmap(tPictureBox* settings, gfx_u8 *file, gfx_s32 X, gfx_s32 Y, bool use_transparency, bool scale)
{
	tDisplay *pDisplay = settings->Internals.pDisplay;
	return put_bitmap(pDisplay, file, settings->Internals.Position.X + X + 2, settings->Internals.Position.Y + Y + 2, use_transparency);
}
//#######################################################################################
bool picturebox_put_fbitmap(tPictureBox* settings, char *path, gfx_s32 X, gfx_s32 Y, bool use_transparency, bool scale)
{
	tDisplay *pDisplay = settings->Internals.pDisplay;
	return put_fbitmap(pDisplay, path, settings->Internals.Position.X + X + 2, settings->Internals.Position.Y + Y + 2, use_transparency);
}
#endif
//#######################################################################################
