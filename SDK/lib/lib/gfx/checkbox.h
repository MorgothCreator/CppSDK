/*
 *  lib/gfx/checkbox.h
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

#ifndef CHECKBOX_H_
#define CHECKBOX_H_
//#######################################################################################
#include <stdbool.h>
#include "string.h"
#include "controls_definition.h"
//#include "window_def.h"
#include "gfxdef.h"
//#######################################################################################
//#######################################################################################
namespace GI
{
namespace Screen
{
namespace Gfx
{
class CheckBox {
public:
	CheckBox(void *parentWindowHandler);
	~CheckBox();
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
			struct {
				Color::ARGB border;
				Color::ARGB back;
				Color::ARGB check;
				Color::ARGB ink;
			}checked;
			struct {
				Color::ARGB border;
				Color::ARGB back;
				Color::ARGB check;
				Color::ARGB ink;
			}unChecked;
		}disabled;
		struct {
			struct {
				struct {
					Color::ARGB border;
					Color::ARGB back;
					Color::ARGB check;
					Color::ARGB ink;
				}checked;
				struct {
					Color::ARGB border;
					Color::ARGB back;
					Color::ARGB check;
					Color::ARGB ink;
				}unChecked;
			}push;
			struct {
				struct {
					Color::ARGB border;
					Color::ARGB back;
					Color::ARGB check;
					Color::ARGB ink;
				}checked;
				struct {
					Color::ARGB border;
					Color::ARGB back;
					Color::ARGB check;
					Color::ARGB ink;
				}unChecked;
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
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*);
		}OnCheched;
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*);
		}OnUnCheched;
		bool CursorUp;
		bool CursorDown;
		bool CursorMove;
		bool Checked;
		bool UnChecked;
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
		bool OldStateCheched;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool CursorDownInsideBox;
		bool NoPaintBackGround;
		CursorState OldStateCursor;
		GI::Dev::Screen *pDisplay;
		void *parentWindowHandler;
		bool parentWindowStateEnabled;
		GI::Screen::String *Caption;
	}Internals;
	enum
	{
		normal = 0,
		radio_btn
	}Style;
	bool Visible;
	bool Enabled;
	bool Cheched;
	CursorState StateChangedOn;

private:
	void paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand);

};
}
}
}
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "checkbox.c"
#endif
//#######################################################################################
#endif /* CHECKBOX_H_ */
