/*
 *  lib/gfx/listbox.h
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

#ifndef LISTBOX_H_
#define LISTBOX_H_
//#######################################################################################
#include <stdbool.h>
#include "scrollbar.h"
#include "scrollbar.h"
#include "controls_definition.h"
#include "string.h"
//#include "window_def.h"
//#######################################################################################
typedef struct
{
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
			colors text;
		}disabled;
		struct {
			colors border;
			colors back;
			colors text;
		}enabled;
	}color;

	bool CursorDownInsideBox;
}listboxItem;


namespace GI
{
namespace Screen
{
namespace Gfx
{
class ListBox {
public:
	ListBox(void *parentWindowHandler);
	~ListBox();
	void idle(tControlCommandData* control_comand);
	void idle();
	bool insert(char* text, unsigned int location);
	void add(char* text);
	void clear();
	bool remove(unsigned int location);

	struct
	{
		signed int X;
		signed int Y;
	}Position;
	struct
	{
		signed int X;
		signed int Y;
		signed int ItemSizeY;
		signed int ScrollSize;
		signed int MinScrollBtnSize;
	}Size;
	GI::Screen::String *Caption;
	controls_color_struct_t Color;

	struct {
		struct {
			struct {
				colors border;
				colors back;
				colors text;
			}selected;
			struct {
				colors border;
				colors back;
				colors text;
			}unSelected;
		}disabled;
		struct {
			struct {
				colors border;
				colors back;
				colors text;
			}selected;
			struct {
				colors border;
				colors back;
				colors text;
			}unSelected;
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
			signed int ItemSizeY;
			signed int ScrollSize;
			signed int MinScrollBtnSize;
		}Size;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool CursorDownInsideBox;
		bool IsChildren;
		bool NoPaintBackGround;
		unsigned int ItemStartOnBox;
		unsigned int OldItemStartOnBox;
		CursorState OldStateCursor;
		GI::Dev::Screen *pDisplay;
		GI::Screen::Gfx::ScrollBar* ScrollBar;
		void *parentWindowHandler;
		bool parentWindowStateEnabled;
	}Internals;
	bool Visible;
	bool Enabled;
	bool AlwaisShowScrollbar;
	CursorState StateChangedOn;
	unsigned int ItemsCount;
	unsigned int SelectedItem;
	listboxItem **Items;

private:
	bool paint(listboxItem* settings, void *pDisplay, signed int x_start, signed int y_start, tControlCommandData* control_comand, bool Pushed, bool Paint);

};
}
}
}
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "listbox.c"
#endif
//#######################################################################################
#endif /* LISTBOX_H_ */
