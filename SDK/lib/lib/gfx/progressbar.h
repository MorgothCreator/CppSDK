/*
 *  lib/gfx/progressbar.h
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

#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_
//#######################################################################################
#include <stdbool.h>
#include "controls_definition.h"
//#include "window_def.h"
#include <lib/util/converters.h>
#include "string.h"
//#######################################################################################
namespace GI
{
namespace Screen
{
namespace Gfx
{
class ProgressBar {
public:
	ProgressBar(void *parentWindowHandler);
	~ProgressBar();
	void idle(tControlCommandData* control_comand);
	void idle();


	struct
	{
		signed int X;
		signed int Y;
	}Position;
	struct
	{
		signed int X;
		signed int Y;
	}Size;
	GI::Screen::String *Caption;
	controls_color_struct_t Color;

	struct {
		struct {
			colors border;
			colors back;
			colors progress;
			colors text;
		}disabled;
		struct {
			colors border;
			colors back;
			colors progress;
			colors text;
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
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*);
		}OnValueChange;
		bool CursorUp;
		bool CursorDown;
		bool CursorMove;
		bool ValueChanged;
	}Events;
	struct
	{
		struct
		{
			bool Initiated;
		}Control;
		struct
		{
			signed int X;
			signed int Y;
		}Position;
		struct
		{
			signed int X;
			signed int Y;
		}PositionOffset;
		struct
		{
			signed int X;
			signed int Y;
		}Size;
		unsigned int OldValue;
		unsigned int OldMinimValue;
		unsigned int OldMaximValue;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool OldStateCheched;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool NoPaintBackGround;
		bool CursorDownInsideBox;
		CursorState OldStateCursor;
		GI::Dev::Screen *pDisplay;
		void *parentWindowHandler;
		bool parentWindowStateEnabled;
		GI::Screen::String *Caption;
	}Internals;
	bool Visible;
	bool Enabled;
	unsigned int Value;
	unsigned int MinimValue;
	unsigned int MaximValue;
	CursorState StateChangedOn;

private:
	void paint(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, tControlCommandData* control_comand);

};
}
}
}
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "progressbar.c"
#endif
//#######################################################################################
#endif /* PROGRESSBAR_H_ */
