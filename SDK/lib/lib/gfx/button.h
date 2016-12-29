/*
 *  lib/gfx/buton.h
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

#ifndef BUTON_H_
#define BUTON_H_
//#######################################################################################
#include <stdbool.h>
#include <lib/gfx/string.h>
#include "controls_definition.h"
#include <api/timer.h>
#include "gfxdef.h"
//#######################################################################################
namespace GI
{
namespace Screen
{
namespace Gfx
{
class Button {
public:
	Button(void *parentWindowHandler);
	~Button();
	void idle(tControlCommandData* control_comand);
	void idle();

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
	GI::Screen::String *Caption;
	controls_color_struct_t Color;
	struct {
		struct {
			Color::ARGB border;
			Color::ARGB back;
			Color::ARGB text;
		}disabled;
		struct {
			struct {
				Color::ARGB border;
				Color::ARGB back;
				Color::ARGB text;
			}push;
			struct {
				Color::ARGB border;
				Color::ARGB back;
				Color::ARGB text;
			}pull;
		}enabled;
	}color;
	struct
	{
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*);
		}OnMove;
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*);
		}OnUp;
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*);
		}OnDown;
		bool CursorUp;
		bool CursorDown;
		bool CursorMove;
	}Events;
	struct
	{
		struct
		{
			bool Initiated;
		}Control;
		struct
		{
			gfx_s32 X;
			gfx_s32 Y;
		}Position;
		struct
		{
			gfx_s32 X;
			gfx_s32 Y;
		}PositionOffset;
		struct
		{
			gfx_s32 X;
			gfx_s32 Y;
		}Size;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool CursorDownInsideBox;
		bool IsChildren;
		bool NoPaintBackGround;
		bool ContinuouslyPushTimerDisabled;
		CursorState OldStateCursor;
		GI::Sys::Timer ContinuouslyPushTimer;
		GI::Dev::Screen *pDisplay;
		void *parentWindowHandler;
		bool parentWindowStateEnabled;
		GI::Screen::String *Caption;
	}Internals;
	bool Visible;
	bool Enabled;
	CursorState StateChangedOn;

private:
	void paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand);

};
}
}
}
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "button.c"
#endif
//#######################################################################################
#endif /* BUTON_H_ */
