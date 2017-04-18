/*
 *  lib/gfx/scrollbar.h
 */

#ifndef SCROLLBAR_H_
#define SCROLLBAR_H_

#include <stdbool.h>
#include "controls_definition.h"
#include "button.h"
#include "string.h"
#include "gfxdef.h"

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
		gfx_s32 X;
		gfx_s32 Y;
	}Position;
	struct
	{
		gfx_s32 X;
		gfx_s32 Y;
		gfx_s32 MinBtnSize;
	}Size;
	controls_color_struct_t Color;

	struct {
		struct {
			Color::ARGB border;
			Color::ARGB back;
		}disabled;
		struct {
			Color::ARGB border;
			Color::ARGB back;
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
			gfx_s32 MinBtnSize;
		}Size;
		bool CursorDownInsideBox;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool NoPaintBackGround;
		bool IsChildren;
		gfx_s32 OldMinimum;
		gfx_s32 OldMaximum;
		gfx_s32 OldValue;
		gfx_s32 CoordonateOfTouchDown;
		gfx_s32 CoordonateOfButtonDown;
		//gfx_s32 OffsetButtonCoord;
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
	gfx_s32 Minimum;
	gfx_s32 Maximum;
	gfx_s32 Value;
	CursorState StateChangedOn;

private:
	void paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand);

};
}
}
}
//#######################################################################################
#ifdef USE_VISUAL_STUDIO
#include "scrollbar.cpp"
#endif
#endif /* SCROLLBAR_H_ */
