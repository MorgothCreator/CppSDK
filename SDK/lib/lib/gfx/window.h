/*
 *  lib/gfx/window.h
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

#ifndef WINDOW_H_
#define WINDOW_H_
//#######################################################################################
#include "controls_definition.h"
#include "button.h"
#include "checkbox.h"
#include "listbox.h"
#include "progressbar.h"
#include "scrollbar.h"
#include "textbox.h"
#include "picturebox.h"
#include "keyboard.h"

#include "string.h"
#include "gfxdef.h"
//#######################################################################################
typedef enum {
	WindowButtonChildren = 1,
	WindowCheckboxChildren,
	WindowListboxChildren,
	WindowProgressbarChildren,
	WindowScrollbarChildren,
	WindowTextboxChildren,
	WindowPictureboxChildren,
	WindowKbdQwertyChildren,
	WindowWindowChildren,
	WindowTabGroupChildren,
}windowChildrenType;

typedef struct
{
	struct
	{
		bool OldStateVisible;
	}Internals;
	bool Visible;
	char *ChildrenName;
	void *Children;
	gfx_u32 Type;
	gfx_u32 TabLocNr;
}window_children_t;

typedef struct
{
	gfx_s32 X;
	gfx_s32 Y;
}ChildrenWindowSize_t;

//#######################################################################################
/* Definitions to add new private items to a parent window */
/*
 * This macro will allocate and add a Button to desired Window.
 *
 * windowHandler = Window structure pointer.
 * button_name = Button name.
 */
#define newButton(windowHandler, button_name)\
		GI::Screen::Gfx::Button *button_name = windowHandler->addButton((char *)#button_name)

/*
 * This macro will allocate and add a CheckBox to desired Window.
 *
 * windowHandler = Window structure pointer.
 * button_name = CheckBox name.
 */
#define newCheckBox(windowHandler, checkbox_name)\
		GI::Screen::Gfx::CheckBox *checkbox_name = windowHandler->addCheckBox((char *)#checkbox_name)

/*
 * This macro will allocate and add a ListBox to desired Window.
 *
 * windowHandler = Window structure pointer.
 * button_name = ListBox name.
 */
#define newListBox(windowHandler, listbox_name)\
		GI::Screen::Gfx::ListBox *listbox_name = windowHandler->addListBox((char *)#listbox_name)

/*
 * This macro will allocate and add a ProgressBar to desired Window.
 *
 * windowHandler = Window structure pointer.
 * button_name = ProgressBar name.
 */
#define newProgressBar(windowHandler, progressbar_name)\
		GI::Screen::Gfx::ProgressBar *progressbar_name = windowHandler->addProgressBar((char *)#progressbar_name)

/*
 * This macro will allocate and add a ScrollBar to desired Window.
 *
 * windowHandler = Window structure pointer.
 * button_name = ScrollBar name.
 */
#define newScrollBar(windowHandler, scrollbar_name)\
			tScrollBar *scrollbar_name = (tScrollBar *)windowHandler->addChildren(WindowScrollbarChildren, (char *)#scrollbar_name)

/*
 * This macro will allocate and add a TextBox to desired Window.
 *
 * windowHandler = Window structure pointer.
 * button_name = TextBox name.
 */
#define newTextBox(windowHandler, textbox_name)\
		GI::Screen::Gfx::TextBox *textbox_name = windowHandler->addTextBox((char *)#textbox_name)

/*
 * This macro will allocate and add a PictureBox to desired Window.
 *
 * windowHandler = Window structure pointer.
 * button_name = PictureBox name.
 */
#define newPictureBox(windowHandler, picturebox_name)\
		GI::Screen::Gfx::Picturebox *picturebox_name = windowHandler->addPictureBox((char *)#picturebox_name)

/*
 * This macro will allocate and add a Keyboard to desired Window.
 *
 * windowHandler = Window structure pointer.
 * button_name = Keyboard name.
 */
#define newKeyboard(windowHandler, keyboard_name)\
		GI::Screen::Gfx::KbdQwerty *keyboard_name = windowHandler->addKbdQwerty((char *)#keyboard_name)

/*
 * This macro will allocate and add a Window to desired Window.
 *
 * windowHandler = Window structure pointer.
 * button_name = Window name.
 */
#define newWindow(windowHandler, window_name)\
		GI::Screen::Gfx::Window *window_name = windowHandler->addWindow((char *)#window_name)\

/*
 * This macro will allocate and add a TabGroup to desired Window.
 *
 * windowHandler = Window structure pointer.
 * button_name = TabGroup name.
 */
#define newTabGroup(windowHandler, tab_group_name)\
		GI::Screen::Gfx::Window *tab_group_name = windowHandler->addTabGroup((char *)#tab_group_name)
//#######################################################################################
/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_button(<item name>); before main
 *  and <item name> = window_new_button_global(<window name>, <item name>); after used window declaration.
*/
#define newButtonGlobal(windowHandler, button_name)\
		windowHandler->addButton((char *)#button_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_checkbox(<item name>); before main
 *  and <item name> = window_new_checkbox_global(<window name>, <item name>); after used window declaration.
*/
#define newCheckBoxGlobal(windowHandler, checkbox_name)\
		windowHandler->addCheckBox((char *)#checkbox_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_listbox(<item name>); before main
 *  and <item name> = window_new_listbox_global(<window name>, <item name>); after used window declaration.
*/
#define newListBoxGlobal(windowHandler, listbox_name)\
		windowHandler->addListBox((char *)#listbox_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_progressbar(<item name>); before main
 *  and <item name> = window_new_progressbar_global(<window name>, <item name>); after used window declaration.
*/
#define newProgressBarGlobal(windowHandler, progressbar_name)\
		windowHandler->addProgressBar((char *)#progressbar_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_scrollbar(<item name>); before main
 *  and <item name> = window_new_scrollbar_global(<window name>, <item name>); after used window declaration.
*/
#define newScrollBarGlobal(windowHandler, scrollbar_name)\
		windowHandler->addScrollBar((char *)#scrollbar_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_textbox(<item name>); before main
 *  and <item name> = window_new_textbox_global(<window name>, <item name>); after used window declaration.
*/
#define newTextBoxGlobal(windowHandler, textbox_name)\
		windowHandler->addTextBox((char *)#textbox_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_picturebox(<item name>); before main
 *  and <item name> = window_new_picturebox_global(<window name>, <item name>); after used window declaration.
*/
#define newPictureBoxGlobal(windowHandler, picturebox_name)\
		windowHandler->addPictureBox((char *)#picturebox_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_vqkbd(<item name>); before main
 *  and <item name> = window_new_keyboard_global(<window name>, <item name>); after used window declaration.
*/
#define newKeyboardGlobal(windowHandler, keyboard_name)\
		windowHandler->addKbdQwerty((char *)#keyboard_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_window(<item name>); before main
 *  and <item name> = window_new_window_global(<window name>, <item name>); after used window declaration.
*/
#define newWindowGlobal(windowHandler, window_name)\
			windowHandler->addWindow((char *)#window_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_tabgroup(<item name>); before main
 *  and <item name> = window_new_tab_group_global(<window name>, <item name>); after used window declaration.
*/
#define newTabGroupGlobal(windowHandler, tab_group_name)\
			windowHandler->addTabGroup((char *)#tab_group_name)
//#######################################################################################
/* Definitions to add new private items to a parent tab group */
/*
 * This macro will allocate and add a Button to desired Tab inside desired TabGroup.
 *
 * windowHandler = Tab group structure pointer.
 * button_name = Button name.
 * tab_nr = Tab number where to register this button.
 */
#define tabGroupNewButton(windowHandler, button_name, tab_nr)\
		GI::Screen::Gfx::Button *button_name = windowHandler->addButton((char *)#button_name, tab_nr)

/*
 * This macro will allocate and add a CheckBox to desired Tab inside desired TabGroup.
 *
 * windowHandler = Tab group structure pointer.
 * button_name = CheckBox name.
 * tab_nr = Tab number where to register this CheckBox.
 */
#define tabGroupNewCheckBox(windowHandler, checkbox_name, tab_nr)\
		GI::Screen::Gfx::CheckBox *checkbox_name = windowHandler->addCheckBox((char *)#checkbox_name, tab_nr)

/*
 * This macro will allocate and add a ListBox to desired Tab inside desired TabGroup.
 *
 * windowHandler = Tab group structure pointer.
 * button_name = ListBox name.
 * tab_nr = Tab number where to register this ListBox.
 */
#define tabGroupNewListBox(windowHandler, listbox_name, tab_nr)\
		GI::Screen::Gfx::ListBox *listbox_name = windowHandler->addListBox((char *)#listbox_name, tab_nr)

/*
 * This macro will allocate and add a ProgressBar to desired Tab inside desired TabGroup.
 *
 * windowHandler = Tab group structure pointer.
 * button_name = ProgressBar name.
 * tab_nr = Tab number where to register this ProgressBar.
 */
#define tabGroupNewProgressBar(windowHandler, progressbar_name, tab_nr)\
		GI::Screen::Gfx::ProgressBar *progressbar_name = windowHandler->addProgressBar((char *)#progressbar_name, tab_nr)

/*
 * This macro will allocate and add a ScrollBar to desired Tab inside desired TabGroup.
 *
 * windowHandler = Tab group structure pointer.
 * button_name = ScrollBar name.
 * tab_nr = Tab number where to register this ScrollBar.
 */
#define tabGroupNewScrollBar(windowHandler, scrollbar_name, tab_nr)\
		GI::Screen::Gfx::ScrollBar *scrollbar_name = windowHandler->addScrollBar((char *)#scrollbar_name, tab_nr)

/*
 * This macro will allocate and add a TextBox to desired Tab inside desired TabGroup.
 *
 * windowHandler = Tab group structure pointer.
 * button_name = TextBox name.
 * tab_nr = Tab number where to register this TextBox.
 */
#define tabGroupNewTextBox(windowHandler, textbox_name, tab_nr)\
		GI::Screen::Gfx::TextBox *textbox_name = windowHandler->addTextBox((char *)#textbox_name, tab_nr)

/*
 * This macro will allocate and add a PictureBox to desired Tab inside desired TabGroup.
 *
 * windowHandler = Tab group structure pointer.
 * button_name = PictureBox name.
 * tab_nr = Tab number where to register this PictureBox.
 */
#define tabGroupNewPictureBox(windowHandler, picturebox_name, tab_nr)\
		GI::Screen::Gfx::Picturebox *picturebox_name = windowHandler->addPictureBox((char *)#picturebox_name, tab_nr)

/*
 * This macro will allocate and add a Keyboard to desired Tab inside desired TabGroup.
 *
 * windowHandler = Tab group structure pointer.
 * button_name = Keyboard name.
 * tab_nr = Tab number where to register this Keyboard.
 */
#define tabGroupNewKeyboard(windowHandler, keyboard_name, tab_nr)\
		GI::Screen::Gfx::KbdQwerty *keyboard_name = windowHandler->addKbdQwerty((char *)#keyboard_name, tab_nr)

/*
 * This macro will allocate and add a Window to desired Tab inside desired TabGroup.
 *
 * windowHandler = Tab group structure pointer.
 * button_name = Window name.
 * tab_nr = Tab number where to register this Window.
 */
#define tabGroupNewWindow(windowHandler, window_name, tab_nr)\
		GI::Screen::Gfx::Window *window_name = windowHandler->addWindow((char *)#window_name, tab_nr)

/*
 * This macro will allocate and add a TabGroup to desired Tab inside desired TabGroup.
 *
 * windowHandler = Tab group structure pointer.
 * button_name = TabGroup name.
 * tab_nr = Tab number where to register this TabGroup.
 */
#define tabGroupNewTabGroup(windowHandler, window_name, tab_nr)\
		GI::Screen::Gfx::Window *window_name = windowHandler->addTabGroup((char *)#window_name, tab_nr)
//#######################################################################################
/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_button(<item name>); before main
 *  and tab_group_new_button_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tabGroupNewButtonGlobal(windowHandler, button_name, tab_nr)\
		windowHandler->addButton((char *)#button_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_checkbox(<item name>); before main
 *  and tab_group_new_checkbox_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tabGroupNewCheckBoxGlobal(windowHandler, checkbox_name, tab_nr)\
		windowHandler->addCheckBox((char *)#checkbox_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_listbox(<item name>); before main
 *  and tab_group_new_listbox_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tabGroupNewListBoxGlobal(windowHandler, listbox_name, tab_nr)\
		windowHandler->addListBox((char *)#listbox_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_progressbar(<item name>); before main
 *  and tab_group_new_progressbar_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tabGroupNewProgressBarGlobal(windowHandler, progressbar_name, tab_nr)\
		windowHandler->addProgressBar((char *)#progressbar_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_scrollbar(<item name>); before main
 *  and tab_group_new_scrollbar_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tabGroupNewScrollBarGlobal(windowHandler, scrollbar_name, tab_nr)\
		windowHandler->addScrollBar((char *)#scrollbar_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_textbox(<item name>); before main
 *  and tab_group_new_textbox_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tabGroupNewTextBoxGlobal(windowHandler, textbox_name, tab_nr)\
		windowHandler->addTextBox((char *)#textbox_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_picturebox(<item name>); before main
 *  and tab_group_new_picturebox_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tabGroupNewPictureBoxGlobal(windowHandler, picturebox_name, tab_nr)\
		windowHandler->addPictureBox((char *)#picturebox_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_vqkbd(<item name>); before main
 *  and tab_group_new_keyboard_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tabGroupNewKeyboardGlobal(windowHandler, keyboard_name, tab_nr)\
		windowHandler->addKbdQwerty((char *)#keyboard_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_window(<item name>); before main
 *  and tab_group_new_window_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tabGroupNewWindowGlobal(windowHandler, window_name, tab_nr)\
		windowHandler->addWindow((char *)#window_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_tabgroup(<item name>); before main
 *  and tab_group_new_tab_group_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tabGroupNewTabGroupGlobal(windowHandler, window_name, tab_nr)\
		windowHandler->addTabGroup((char *)#window_name, tab_nr)
//#######################################################################################
#define _new_window(name) tWindow *name = NULL
//#define free_window(address) free(address);
//#######################################################################################
#define _new_tabgroup(name) tWindow *name = NULL
//#define free_tabgroup(address) free(address);


//#######################################################################################
namespace GI
{
namespace Screen
{
namespace Gfx
{
class Window {
public:
	Window(void *ParentWindow, GI::Dev::Screen *ScreenDisplay);
	Window(GI::Dev::Screen *ScreenDisplay);
	~Window();
	void idle(tControlCommandData* control_comand);
	void idle();
	void* addChildren(gfx_u32 children_type, char *children_name);
	void* addChildren(gfx_u32 children_type, char *children_name, gfx_u32 tab_nr);

	GI::Screen::Gfx::Button *addButton(char *name);
	GI::Screen::Gfx::CheckBox *addCheckBox(char *name);
	GI::Screen::Gfx::ListBox *addListBox(char *name);
	GI::Screen::Gfx::Picturebox *addPictureBox(char *name);
	GI::Screen::Gfx::TextBox *addTextBox(char *name);
	GI::Screen::Gfx::ProgressBar *addProgressBar(char *name);
	GI::Screen::Gfx::ScrollBar *addScrollBar(char *name);
	GI::Screen::Gfx::KbdQwerty *addKbdQwerty(char *name);
	GI::Screen::Gfx::Window *addWindow(char *name);
	GI::Screen::Gfx::Window *addTabGroup(char *name);


	GI::Screen::Gfx::Button *addButton(char *name, gfx_u32 tabIndex);
	GI::Screen::Gfx::CheckBox *addCheckBox(char *name, gfx_u32 tabIndex);
	GI::Screen::Gfx::ListBox *addListBox(char *name, gfx_u32 tabIndex);
	GI::Screen::Gfx::Picturebox *addPictureBox(char *name, gfx_u32 tabIndex);
	GI::Screen::Gfx::TextBox *addTextBox(char *name, gfx_u32 tabIndex);
	GI::Screen::Gfx::ProgressBar *addProgressBar(char *name, gfx_u32 tabIndex);
	GI::Screen::Gfx::ScrollBar *addScrollBar(char *name, gfx_u32 tabIndex);
	GI::Screen::Gfx::KbdQwerty *addKbdQwerty(char *name, gfx_u32 tabIndex);
	GI::Screen::Gfx::Window *addWindow(char *name, gfx_u32 tabIndex);
	GI::Screen::Gfx::Window *addTabGroup(char *name, gfx_u32 tabIndex);



	struct
	{
		gfx_s32 X;
		gfx_s32 Y;
	}Position;
	struct
	{
		gfx_s32 X;
		gfx_s32 Y;
		gfx_s32 ScrollBarSize;
	}Size;
	struct
	{
		gfx_s32 X;
		gfx_s32 Y;
	}SizeNormalScreen;
	GI::Screen::String *Caption;
	controls_color_struct_t Color;

	struct {
		struct {
			Color::ARGB back;
			Color::ARGB text;
		}header;

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
			gfx_u32 WindowBorder;
			gfx_u32 WindowHeader;
		}Enabled;
		struct
		{
			gfx_u32 WindowBorder;
			gfx_u32 WindowHeader;
		}Disabled;
	}WindowColor;
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
		}OnTabChange;
		bool CursorUp;
		bool CursorDown;
		bool CursorMove;
		bool TabChanged;
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
			gfx_s32 ChildrenPosition_X;
			gfx_s32 ChildrenPosition_Y;
		}Position;
		struct
		{
			gfx_s32 X;
			gfx_s32 Y;
			gfx_s32 ScrollBarSize;
		}Size;
		struct
		{
			struct
			{
				gfx_s32 Y;
				gfx_s32 TabGroupSelectorSize;
			}Size;
			gfx_s32 TabGroupSelectorPosition;
			GI::Screen::Gfx::Button *Close;
			GI::Screen::Gfx::CheckBox *MaxMin;
			GI::Screen::Gfx::Button *Minimize;
			GI::Screen::Gfx::Button *Resize;
			GI::Screen::Gfx::Button *TabGroupScrollLeft;
			GI::Screen::Gfx::Button *TabGroupScrollRight;
			GI::Screen::Gfx::CheckBox **TabGroupTabsList;
			gfx_u32 TabGroupTabsListNr;
			gfx_u32 OldTabGroupTabsListNr;
		}Header;
		struct
		{
			gfx_s32 X;
			gfx_s32 Y;
		}NormalScreenPosition;
		struct
		{
			gfx_s32 X;
			gfx_s32 Y;
		}PositionOffset;
		GI::Screen::String *Caption;
		gfx_s32 HeaderTouchDownPointX;
		gfx_s32 HeaderTouchDownPointY;
		gfx_s32 WindowTouchDownPointX;
		gfx_s32 WindowTouchDownPointY;
		gfx_s32 WindowResizeTouchDownPointX;
		gfx_s32 WindowResizeTouchDownPointY;
		gfx_s32 WindowResizeTouchDownSizeX;
		gfx_s32 WindowResizeTouchDownSizeY;
		bool CursorDownOnHeader;
		bool CursorDownOnResizeBtn;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool CloseButonEnabled;
		bool CloseButonVisible;
		bool MaxMinEnabled;
		bool MaxMinVisible;
		bool MinimizeButonEnabled;
		bool MinimizeButonVisible;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool CursorDownInsideBox;
		bool FullScreen;
		bool OneChildrenHasBeenModified;
		bool NoPaintBackGround;
		bool CursorDownInsideChildrenWindow;
		bool TabGroupMode;
		bool OldTabGroupMode;
		bool HideHeader;
		//bool TabChanged;
		volatile gfx_u32 ChildrensNr;
		window_children_t **Childrens;
		CursorState OldStateCursor;
		GI::Dev::Screen *pDisplay;
		GI::Screen::Gfx::ScrollBar *H_ScrollBar;
		GI::Screen::Gfx::ScrollBar *V_ScrollBar;
		ChildrenWindowSize_t ChildrenWindowSize;
		tRectangle WindowMoveLimits;
		void *parentWindowHandler;
	}Internals;
	bool Visible;
	bool Enabled;
	bool CloseButonEnabled;
	bool CloseButonVisible;
	bool MaxMinEnabled;
	bool MaxMinVisible;
	bool MinimizeButonEnabled;
	bool MinimizeButonVisible;
	bool AllowVScroll;
	bool AllowHScroll;
	bool HideVScroll;
	bool HideHScroll;
	bool HideHeader;
	gfx_u32 SelectedTab;
	CursorState StateChangedOn;
	tRectangle WindowMoveLimits;


private:
	void _newWindow(void *ParentWindow, GI::Dev::Screen *ScreenDisplay);
	void *_addChildren(gfx_u32 children_type, char *children_name, gfx_u32 tab_nr);
	void setChildrens(bool call_childrens, bool transfer_settings, tControlCommandData* control_comand, bool refresh_childrens, ChildrenWindowSize_t *ChildrenWindowSize);
};
}
}
}
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "window.c"
#endif
//#######################################################################################
#endif /* WINDOW_H_ */
//#######################################################################################

