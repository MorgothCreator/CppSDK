/*
 *  lib/gfx/progressbar.h
 */

#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_
//#######################################################################################
#include <stdbool.h>
#include "controls_definition.h"
//#include "window_def.h"
#include <lib/util/converters.h>
#include "string.h"
#include "gfxdef.h"
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
			Color::ARGB progress;
			Color::ARGB text;
		}disabled;
		struct {
			Color::ARGB border;
			Color::ARGB back;
			Color::ARGB progress;
			Color::ARGB text;
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
		gfx_u32 OldValue;
		gfx_u32 OldMinimValue;
		gfx_u32 OldMaximValue;
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
	gfx_u32 Value;
	gfx_u32 MinimValue;
	gfx_u32 MaximValue;
	CursorState StateChangedOn;

private:
	void paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand);

};
}
}
}
//#######################################################################################
#ifdef USE_VISUAL_STUDIO
#include "progressbar.cpp"
#endif
//#######################################################################################
#endif /* PROGRESSBAR_H_ */
