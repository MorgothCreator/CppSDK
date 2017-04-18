/*
 *  lib/gfx/textbox.h
 */

#ifndef TEXTBOX_H_
#define TEXTBOX_H_
//#######################################################################################
#include <stdbool.h>
#include "scrollbar.h"
#include "controls_definition.h"
#include "string.h"
#include "gfxdef.h"
//#include "window_def.h"
//#######################################################################################
namespace GI
{
namespace Screen
{
namespace Gfx
{
class TextBox {
public:
	TextBox(void *parentWindowHandler);
	~TextBox();
	void idle(tControlCommandData* control_comand);
	void idle();
	bool setText(char* string);
	bool append(char* string);
	bool insert(char* string, gfx_u32 location);
	bool toUpper();
	bool toLower();
	bool setFont(tFont* font);
	bool setWordWrap(bool word_wrap);
	bool setAlign(allign Align);


	struct
	{
		gfx_s32 X;
		gfx_s32 Y;
	}Position;
	struct
	{
		gfx_s32 X;
		gfx_s32 Y;
		gfx_s32 ScrollSize;
	}Size;
	//controls_caption_t Caption;
	controls_color_struct_t Color;

	struct {
		struct {
			Color::ARGB border;
			Color::ARGB back;
			struct {
				Color::ARGB back;
				Color::ARGB text;
			}selectedText;
			struct {
				Color::ARGB text;
			}unSelectedText;
		}disabled;
		struct {
			Color::ARGB border;
			Color::ARGB back;
			struct {
				Color::ARGB back;
				Color::ARGB text;
			}selectedText;
			struct {
				Color::ARGB text;
			}unSelectedText;
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
			gfx_s32 ScrollSize;
		}Size;
		//controls_caption_t Caption;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool CursorDownInsideBox;
		bool CursorDownInsideTextBox;
		bool IsChildren;
		bool NoPaintBackGround;
		gfx_s32 OldSelStart;
		gfx_s32 OldSelLen;
		gfx_s32 PenPushX;
		gfx_s32 PenPushY;
		gfx_s32 Start;
		CursorState OldStateCursor;
		GI::Screen::Gfx::ScrollBar *Hscrollbar;
		GI::Screen::Gfx::ScrollBar *Vscrollbar;
		GI::Dev::Screen *pDisplay;
		void *parentWindowHandler;
		bool parentWindowStateEnabled;
		GI::Screen::String *text;
	}Internals;
	bool Visible;
	bool Enabled;
	gfx_u32 SelectionStart;
	gfx_u32 SelectionLength;
	gfx_s32 SelStart;
	gfx_s32 SelLen;
	GI::Screen::String *text;
	CursorState StateChangedOn;

private:
	void paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand);

};
}
}
}
//#######################################################################################
#ifdef USE_VISUAL_STUDIO
#include "textbox.cpp"
#endif
//#######################################################################################
#endif /* TEXTBOX_H_ */
//#######################################################################################
