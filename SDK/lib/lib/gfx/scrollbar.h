/*
 *  lib/gfx/scrollbar.h
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

#ifndef SCROLLBAR_H_
#define SCROLLBAR_H_

#include <stdbool.h>
#include "controls_definition.h"
//#include "window_def.h"
#include "button.h"
#include "string.h"

//#######################################################################################
namespace GI
{
namespace Screen
{
namespace Gfx
{
class ScrollBar {
public:
	ScrollBar(void *parentWindowHandler);
	~ScrollBar();
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
		signed int MinBtnSize;
	}Size;
	controls_color_struct_t Color;

	struct {
		struct {
			colors border;
			colors back;
		}disabled;
		struct {
			colors border;
			colors back;
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
		}OnValueChanged;
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
			signed int MinBtnSize;
		}Size;
		bool CursorDownInsideBox;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool NoPaintBackGround;
		bool IsChildren;
		signed int OldMinimum;
		signed int OldMaximum;
		signed int OldValue;
		signed int CoordonateOfTouchDown;
		signed int CoordonateOfButtonDown;
		//signed int OffsetButtonCoord;
		CursorState OldStateCursor;
		GI::Screen::Gfx::Button *BtnSettings;
		GI::Screen::Gfx::Button *BtnUpSettings;
		GI::Screen::Gfx::Button *BtnDnSettings;
		GI::Dev::Screen *pDisplay;
		void *parentWindowHandler;
		bool parentWindowStateEnabled;
	}Internals;
	bool Visible;
	bool Enabled;
	signed int Minimum;
	signed int Maximum;
	signed int Value;
	CursorState StateChangedOn;

private:
	void paint(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, tControlCommandData* control_comand);

};
}
}
}
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "scrollbar.c"
#endif
#endif /* SCROLLBAR_H_ */
