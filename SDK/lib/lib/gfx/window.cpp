/*
 *  lib/gfx/window.c
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

#include "window.h"
#include "graphic_string.h"
#include "controls_definition.h"
#include "sys/plat_properties.h"
#include "api/lcd_def.h"
#include "util.h"
#include "button.h"
#include "checkbox.h"
#include "listbox.h"
#include "progressbar.h"
#include "scrollbar.h"
#include "textbox.h"
#include "picturebox.h"
#include "keyboard.h"
#include "resource/fonts.h"
//#######################################################################################
GI::Screen::Gfx::Window::Window(void *parentWindowHandler, GI::Dev::Screen *screenDisplayHandler)
{
	_newWindow(parentWindowHandler, screenDisplayHandler);
}

GI::Screen::Gfx::Window::Window(GI::Dev::Screen *screenDisplayHandler)
{
	_newWindow(NULL, screenDisplayHandler);
}

void GI::Screen::Gfx::Window::_newWindow(void *parentWindowHandler, GI::Dev::Screen *screenDisplayHandler)
{
	memset(this, 0, sizeof(*this));
	GI::Screen::Gfx::Window *_parentWindowHandler = (GI::Screen::Gfx::Window *)parentWindowHandler;
	GI::Dev::Screen *_ScreenDisplay = screenDisplayHandler;
	if(!_ScreenDisplay && !parentWindowHandler)
		return;
	if(parentWindowHandler)
		_ScreenDisplay = _parentWindowHandler->Internals.pDisplay;

	if(parentWindowHandler)
		Internals.parentWindowHandler = parentWindowHandler;
	else
		Internals.parentWindowHandler = this;
	Internals.pDisplay = _ScreenDisplay;
	GI::Screen::String *tmp_str = new GI::Screen::String(Internals.pDisplay);
	tmp_str->textAlign = alignCenter;
	tmp_str->wordWrap = false;
	tmp_str->pFont = controls_color.DefaultFont;
	tmp_str->setText((char *)"Window");
	Caption = tmp_str;

	tmp_str = new GI::Screen::String(Internals.pDisplay);
	Internals.Caption = tmp_str;
	Internals.Caption->clone(Caption);

	//settings->Caption.Text = malloc(sizeof("Textbox") + 1);
	//strcpy(settings->Caption.Text, "Textbox");

	Color.Scren = controls_color.Scren;
	Color.Enabled.TextBackGround = controls_color.Control_Color_Enabled_TextBackGround;
	Color.Enabled.BackGround = controls_color.Control_Color_Enabled_BackGround;
	Color.Enabled.Border.Pull = controls_color.Control_Color_Enabled_Border_Pull;
	Color.Enabled.Border.Push = controls_color.Control_Color_Enabled_Border_Push;
	Color.Enabled.Buton.Pull = controls_color.Control_Color_Enabled_Buton_Pull;
	Color.Enabled.Buton.Push = controls_color.Control_Color_Enabled_Buton_Push;
	Color.Enabled.Ink.Pull = controls_color.Control_Color_Enabled_Ink_Pull;
	Color.Enabled.Ink.Push = controls_color.Control_Color_Enabled_Ink_Push;
	WindowColor.Enabled.WindowBorder = controls_color.Control_Color_Enabled_WindowBorder;
	WindowColor.Enabled.WindowHeader = controls_color.Control_Color_Enabled_WindowHeader;

	Color.Disabled.BackGround = controls_color.Control_Color_Disabled_BackGround;
	Color.Disabled.Border = controls_color.Control_Color_Disabled_Border_Pull;
	Color.Disabled.Buton = controls_color.Control_Color_Disabled_Buton_Pull;
	Color.Disabled.Ink = controls_color.Control_Color_Disabled_Ink_Pull;
	WindowColor.Disabled.WindowBorder = controls_color.Control_Color_Disabled_WindowBorder;
	WindowColor.Disabled.WindowHeader = controls_color.Control_Color_Disabled_WindowHeader;

	//Position.X = PositionFullScreen.X;
	//Position.Y = PositionFullScreen.Y;
	Size.X = _ScreenDisplay->LcdTimings->X;
	Size.Y = _ScreenDisplay->LcdTimings->Y;
	Size.ScrollBarSize = 20;
	//SizeFullScreen.X = screenDisplayHandler->Width;
	//SizeFullScreen.Y = screenDisplayHandler->Height;
	SizeNormalScreen.X = 200;
	SizeNormalScreen.Y = 150;
	StateChangedOn = Cursor_Up;
	Visible = true;
	Enabled = true;
	CloseButonEnabled = true;
	CloseButonVisible = true;
	MaxMinEnabled = true;
	MaxMinVisible = true;
	MinimizeButonEnabled = true;
	MinimizeButonVisible = true;
	if(parentWindowHandler)
	{
		WindowMoveLimits = _parentWindowHandler->WindowMoveLimits;
	}
	else
	{
		WindowMoveLimits.sXMin = 0;
		WindowMoveLimits.sXMax = _ScreenDisplay->LcdTimings->X;
		WindowMoveLimits.sYMin = 0;
		WindowMoveLimits.sYMax = _ScreenDisplay->LcdTimings->Y;
	}
	Internals.Header.Size.Y = 20;
	Internals.FullScreen = true;

	Internals.Header.Size.TabGroupSelectorSize = 70;
}

void* GI::Screen::Gfx::Window::_addChildren(gfx_u32 childrenType, char *childrenName, gfx_u32 tabIndex)
{
	if(!this)
		return NULL;
	if(tabIndex > Internals.Header.TabGroupTabsListNr)
		return NULL;
	void *children_addr = NULL;
	switch(childrenType)
	{
	case WindowButtonChildren:
		children_addr = (void *)new GI::Screen::Gfx::Button(this);
		break;
	case WindowCheckboxChildren:
		children_addr = (void *)new GI::Screen::Gfx::CheckBox(this);
		break;
	case WindowListboxChildren:
		children_addr = (void *)new GI::Screen::Gfx::ListBox(this);
		break;
	case WindowProgressbarChildren:
		children_addr = (void *)new GI::Screen::Gfx::ProgressBar(this);
		break;
	case WindowScrollbarChildren:
		children_addr = (void *)new GI::Screen::Gfx::ScrollBar(this);
		break;
	case WindowTextboxChildren:
		children_addr = (void *)new GI::Screen::Gfx::TextBox(this);
		break;
	case WindowPictureboxChildren:
		children_addr = (void *)new GI::Screen::Gfx::Picturebox(this);
		break;
	case WindowKbdQwertyChildren:
		children_addr = (void *)new GI::Screen::Gfx::KbdQwerty(this);
		break;
	case WindowWindowChildren:
		children_addr = (void *)new GI::Screen::Gfx::Window(this, NULL);
		break;
	case WindowTabGroupChildren:
		children_addr = (void *)new GI::Screen::Gfx::Window(this, NULL);
		break;
	default:
		return NULL;
	}
	if(!children_addr) return NULL;
	gfx_u32 added_children = (gfx_u32)Internals.ChildrensNr;
	if(!added_children)
	{
		Internals.Childrens = (window_children_t **)calloc(1, sizeof(window_children_t *));
		if(!Internals.Childrens) goto window_can_not_add_children;
	}
	else
	{
		window_children_t **new_pointer_of_childrens = (window_children_t **)realloc(Internals.Childrens, sizeof(window_children_t *) * (added_children + 1));
		if(!new_pointer_of_childrens)
			goto window_can_not_add_children;
		Internals.Childrens = new_pointer_of_childrens;
	}
	Internals.Childrens[added_children] = (window_children_t *)calloc(1, sizeof(window_children_t));
	if(!Internals.Childrens[added_children])
		goto window_can_not_add_children;
	Internals.Childrens[added_children]->Children = children_addr;
	Internals.Childrens[added_children]->ChildrenName = childrenName;
	Internals.Childrens[added_children]->Type = childrenType;
	Internals.Childrens[added_children]->TabLocNr = tabIndex;
	switch(childrenType)
	{
	case WindowButtonChildren:
		((GI::Screen::Gfx::Button *)children_addr)->Position.X = /*Position.X*/ 0;
		((GI::Screen::Gfx::Button *)children_addr)->Position.Y = /*Position.Y*/ 0;
		((GI::Screen::Gfx::Button *)children_addr)->Size.X = 40;
		((GI::Screen::Gfx::Button *)children_addr)->Size.Y = 20;
		((GI::Screen::Gfx::Button *)children_addr)->StateChangedOn = StateChangedOn;
		((GI::Screen::Gfx::Button *)children_addr)->Caption->pFont = Caption->pFont;
		((GI::Screen::Gfx::Button *)children_addr)->Caption->wordWrap = Caption->wordWrap;
		((GI::Screen::Gfx::Button *)children_addr)->Enabled = Enabled;
		((GI::Screen::Gfx::Button *)children_addr)->Visible = Visible;
		((GI::Screen::Gfx::Button *)children_addr)->Color = Color;
		((GI::Screen::Gfx::Button *)children_addr)->Caption->setText(childrenName);
		((GI::Screen::Gfx::Button *)children_addr)->Internals.parentWindowHandler = (void*)this;
		break;
	case WindowCheckboxChildren:
		((GI::Screen::Gfx::CheckBox *)children_addr)->Position.X = /*Position.X*/ 0 + 45;
		((GI::Screen::Gfx::CheckBox *)children_addr)->Position.Y = /*Position.Y*/ 0;
		((GI::Screen::Gfx::CheckBox *)children_addr)->Size.X = 60;
		((GI::Screen::Gfx::CheckBox *)children_addr)->Size.Y = 20;
		((GI::Screen::Gfx::CheckBox *)children_addr)->StateChangedOn = StateChangedOn;
		((GI::Screen::Gfx::CheckBox *)children_addr)->Caption->pFont = Caption->pFont;
		((GI::Screen::Gfx::CheckBox *)children_addr)->Caption->textAlign = Caption->textAlign;
		((GI::Screen::Gfx::CheckBox *)children_addr)->Caption->wordWrap = Caption->wordWrap;
		((GI::Screen::Gfx::CheckBox *)children_addr)->Enabled = Enabled;
		((GI::Screen::Gfx::CheckBox *)children_addr)->Visible = Visible;
		((GI::Screen::Gfx::CheckBox *)children_addr)->Color = Color;
		((GI::Screen::Gfx::CheckBox *)children_addr)->Caption->setText(childrenName);
		((GI::Screen::Gfx::CheckBox *)children_addr)->Internals.parentWindowHandler = (void*)this;
		break;
	case WindowListboxChildren:
		((GI::Screen::Gfx::ListBox *)children_addr)->Position.X = /*Position.X*/ 0 + 110;
		((GI::Screen::Gfx::ListBox *)children_addr)->Position.Y = /*Position.Y*/ 0;
		((GI::Screen::Gfx::ListBox *)children_addr)->Size.X = 200;
		((GI::Screen::Gfx::ListBox *)children_addr)->Size.Y = 70;
		((GI::Screen::Gfx::ListBox *)children_addr)->StateChangedOn = StateChangedOn;
		((GI::Screen::Gfx::ListBox *)children_addr)->Caption->pFont = Caption->pFont;
		((GI::Screen::Gfx::ListBox *)children_addr)->Caption->textAlign = alignLeft;//Caption.TextAlign;
		((GI::Screen::Gfx::ListBox *)children_addr)->Caption->wordWrap = Caption->wordWrap;
		((GI::Screen::Gfx::ListBox *)children_addr)->Enabled = Enabled;
		((GI::Screen::Gfx::ListBox *)children_addr)->Visible = Visible;
		((GI::Screen::Gfx::ListBox *)children_addr)->Color = Color;
		((GI::Screen::Gfx::ListBox *)children_addr)->Caption->setText(childrenName);
		((GI::Screen::Gfx::ListBox *)children_addr)->Internals.parentWindowHandler = (void*)this;
		break;
	case WindowProgressbarChildren:
		((GI::Screen::Gfx::ProgressBar *)children_addr)->Position.X = /*Position.X*/ 0;
		((GI::Screen::Gfx::ProgressBar *)children_addr)->Position.Y = /*Position.Y*/ 0 + 25;
		((GI::Screen::Gfx::ProgressBar *)children_addr)->Size.X = 105;
		((GI::Screen::Gfx::ProgressBar *)children_addr)->Size.Y = 20;
		((GI::Screen::Gfx::ProgressBar *)children_addr)->StateChangedOn = StateChangedOn;
		((GI::Screen::Gfx::ProgressBar *)children_addr)->Caption->pFont = Caption->pFont;
		((GI::Screen::Gfx::ProgressBar *)children_addr)->Caption->textAlign = Caption->textAlign;
		((GI::Screen::Gfx::ProgressBar *)children_addr)->Caption->wordWrap = Caption->wordWrap;
		((GI::Screen::Gfx::ProgressBar *)children_addr)->Enabled = Enabled;
		((GI::Screen::Gfx::ProgressBar *)children_addr)->Visible = Visible;
		((GI::Screen::Gfx::ProgressBar *)children_addr)->Color = Color;
		((GI::Screen::Gfx::ProgressBar *)children_addr)->Caption->setText(childrenName);
		((GI::Screen::Gfx::ProgressBar *)children_addr)->Internals.parentWindowHandler = (void*)this;
		break;
	case WindowScrollbarChildren:
		((GI::Screen::Gfx::ScrollBar *)children_addr)->Position.X = /*Position.X*/ 0;
		((GI::Screen::Gfx::ScrollBar *)children_addr)->Position.Y = /*Position.Y*/ 0 + 50;
		((GI::Screen::Gfx::ScrollBar *)children_addr)->Size.X = 105;
		((GI::Screen::Gfx::ScrollBar *)children_addr)->Size.Y = 20;
		((GI::Screen::Gfx::ScrollBar *)children_addr)->StateChangedOn = Cursor_Move;
		((GI::Screen::Gfx::ScrollBar *)children_addr)->Enabled = Enabled;
		((GI::Screen::Gfx::ScrollBar *)children_addr)->Visible = Visible;
		((GI::Screen::Gfx::ScrollBar *)children_addr)->Color = Color;
		((GI::Screen::Gfx::ScrollBar *)children_addr)->Internals.parentWindowHandler = (void*)this;
		break;
	case WindowTextboxChildren:
		((GI::Screen::Gfx::TextBox *)children_addr)->Position.X = /*Position.X*/ 0;
		((GI::Screen::Gfx::TextBox *)children_addr)->Position.Y = /*Position.Y*/ 0 + 75;
		((GI::Screen::Gfx::TextBox *)children_addr)->Size.X = 310;
		((GI::Screen::Gfx::TextBox *)children_addr)->Size.Y = 160;
		((GI::Screen::Gfx::TextBox *)children_addr)->StateChangedOn = StateChangedOn;
		((GI::Screen::Gfx::TextBox *)children_addr)->text->pFont = Caption->pFont;
		((GI::Screen::Gfx::TextBox *)children_addr)->text->textAlign = Caption->textAlign;
		((GI::Screen::Gfx::TextBox *)children_addr)->text->wordWrap = Caption->wordWrap;
		((GI::Screen::Gfx::TextBox *)children_addr)->Enabled = Enabled;
		((GI::Screen::Gfx::TextBox *)children_addr)->Visible = Visible;
		((GI::Screen::Gfx::TextBox *)children_addr)->Color = Color;
		((GI::Screen::Gfx::TextBox *)children_addr)->text->setText(childrenName);
		((GI::Screen::Gfx::TextBox *)children_addr)->Internals.parentWindowHandler = (void*)this;
		break;
	case WindowPictureboxChildren:
		((GI::Screen::Gfx::Picturebox *)children_addr)->Position.X = /*Position.X*/ 0 + 320;
		((GI::Screen::Gfx::Picturebox *)children_addr)->Position.Y = /*Position.Y*/ 0;
		((GI::Screen::Gfx::Picturebox *)children_addr)->Size.X = 128;
		((GI::Screen::Gfx::Picturebox *)children_addr)->Size.Y = 96;
		((GI::Screen::Gfx::Picturebox *)children_addr)->Enabled = Enabled;
		((GI::Screen::Gfx::Picturebox *)children_addr)->Visible = Visible;
		((GI::Screen::Gfx::Picturebox *)children_addr)->Color = Color;
		break;
	case WindowKbdQwertyChildren:
		((GI::Screen::Gfx::KbdQwerty *)children_addr)->Position.X = 0;
		((GI::Screen::Gfx::KbdQwerty *)children_addr)->Position.Y = 0;
		((GI::Screen::Gfx::KbdQwerty *)children_addr)->Size.X = 320;
		((GI::Screen::Gfx::KbdQwerty *)children_addr)->Size.Y = 70;
		((GI::Screen::Gfx::KbdQwerty *)children_addr)->Enabled = Enabled;
		((GI::Screen::Gfx::KbdQwerty *)children_addr)->Visible = Visible;
		((GI::Screen::Gfx::KbdQwerty *)children_addr)->Color = Color;
		break;
	case WindowWindowChildren:
		((Screen::Gfx::Window *)children_addr)->StateChangedOn = StateChangedOn;
		((Screen::Gfx::Window *)children_addr)->Caption->pFont = Caption->pFont;
		((Screen::Gfx::Window *)children_addr)->Caption->textAlign = Caption->textAlign;
		((Screen::Gfx::Window *)children_addr)->Caption->wordWrap = Caption->wordWrap;
		((Screen::Gfx::Window *)children_addr)->Enabled = Enabled;
		((Screen::Gfx::Window *)children_addr)->Visible = Visible;
		((Screen::Gfx::Window *)children_addr)->Color = Color;
		((Screen::Gfx::Window *)children_addr)->Caption->setText(childrenName);
		((Screen::Gfx::Window *)children_addr)->Internals.parentWindowHandler = (void*)this;
		break;
	case WindowTabGroupChildren:
		((Screen::Gfx::Window *)children_addr)->StateChangedOn = StateChangedOn;
		((Screen::Gfx::Window *)children_addr)->Caption->pFont = Caption->pFont;
		((Screen::Gfx::Window *)children_addr)->Caption->textAlign = Caption->textAlign;
		((Screen::Gfx::Window *)children_addr)->Caption->wordWrap = Caption->wordWrap;
		((Screen::Gfx::Window *)children_addr)->Enabled = Enabled;
		((Screen::Gfx::Window *)children_addr)->Visible = Visible;
		((Screen::Gfx::Window *)children_addr)->Color = Color;
		((Screen::Gfx::Window *)children_addr)->Caption->setText(childrenName);
		((Screen::Gfx::Window *)children_addr)->Internals.parentWindowHandler = (void*)this;
		((Screen::Gfx::Window *)children_addr)->Internals.FullScreen = false;
		((Screen::Gfx::Window *)children_addr)->Internals.TabGroupMode = true;
		((Screen::Gfx::Window *)children_addr)->Internals.Header.Size.Y = 25;
		break;
	}
	Internals.ChildrensNr++;
	ChildrenWindowSize_t ChildrenWindowSize;
	setChildrens(false, true, NULL, false, &ChildrenWindowSize);
	return children_addr;

window_can_not_add_children:
	switch(childrenType)
	{
	case WindowButtonChildren:
		((GI::Screen::Gfx::Button*)children_addr)->~Button();
		break;
	case WindowCheckboxChildren:
		((GI::Screen::Gfx::CheckBox*)children_addr)->~CheckBox();
		break;
	case WindowListboxChildren:
		((GI::Screen::Gfx::ListBox*)children_addr)->~ListBox();
		break;
	case WindowProgressbarChildren:
		((GI::Screen::Gfx::ProgressBar*)children_addr)->~ProgressBar();
		break;
	case WindowScrollbarChildren:
		((GI::Screen::Gfx::ScrollBar*)children_addr)->~ScrollBar();
		break;
	case WindowTextboxChildren:
		((GI::Screen::Gfx::TextBox*)children_addr)->~TextBox();
		break;
	case WindowPictureboxChildren:
		((GI::Screen::Gfx::Picturebox*)children_addr)->~Picturebox();
		break;
	case WindowWindowChildren:
		//free_window(children_addr);
		break;
	}
	return NULL;
}

//#######################################################################################
void* GI::Screen::Gfx::Window::addChildren(gfx_u32 children_type, char *children_name)
{
	return _addChildren(children_type, children_name, 0);
}
//#######################################################################################
void* GI::Screen::Gfx::Window::addChildren(gfx_u32 children_type, char *children_name, gfx_u32 tab_nr)
{
	return _addChildren(children_type, children_name, tab_nr);
}


void GI::Screen::Gfx::Window::setChildrens(bool call_childrens, bool transfer_settings, tControlCommandData* control_comand, bool refresh_childrens, ChildrenWindowSize_t *ChildrenWindowSize)
{
	gfx_u32 Tmp_Children_Cnt = 0;
	tRectangle sClipRegion;
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window *)Internals.parentWindowHandler;
	gfx_s32 ParentHeaderSize = 0;
	if(parentWindowHandler->Internals.HideHeader)
		ParentHeaderSize = parentWindowHandler->Internals.Header.Size.Y;
	gfx_s32 HeaderSize = 0;
	if(Internals.HideHeader == false)
		HeaderSize = Internals.Header.Size.Y;
	if(parentWindowHandler != NULL && this != parentWindowHandler)
	{
		if(Internals.V_ScrollBar && Internals.H_ScrollBar)
		{
			sClipRegion = Internals.pDisplay->sClipRegion;
			Internals.pDisplay->sClipRegion.sXMin = (Position.X + 5 + parentWindowHandler->Internals.Position.X) - parentWindowHandler->Internals.H_ScrollBar->Value;
			Internals.pDisplay->sClipRegion.sXMax = (Position.X + Size.X + parentWindowHandler->Internals.Position.X + 1) - 0 - Internals.V_ScrollBar->Size.X - parentWindowHandler->Internals.H_ScrollBar->Value;
			Internals.pDisplay->sClipRegion.sYMin = (Position.Y + 1 + HeaderSize + ParentHeaderSize + parentWindowHandler->Internals.Position.Y) - parentWindowHandler->Internals.V_ScrollBar->Value;
			Internals.pDisplay->sClipRegion.sYMax = (Position.Y + HeaderSize + Size.Y + parentWindowHandler->Internals.Position.Y) - 0 - Internals.H_ScrollBar->Size.Y - parentWindowHandler->Internals.V_ScrollBar->Value;
			GI::Screen::Util::clipLimit(&Internals.pDisplay->sClipRegion, &sClipRegion);
		}
	}
	else
	{
		if(Internals.V_ScrollBar && Internals.H_ScrollBar)
		{
			sClipRegion = Internals.pDisplay->sClipRegion;
			Internals.pDisplay->sClipRegion.sXMin = Position.X + 2;
			Internals.pDisplay->sClipRegion.sXMax = (Position.X + Size.X) - 2 - Internals.V_ScrollBar->Size.X;
			Internals.pDisplay->sClipRegion.sYMin = Position.Y + 1 + HeaderSize;
			Internals.pDisplay->sClipRegion.sYMax = (Position.Y + Size.Y) - 2 - Internals.H_ScrollBar->Size.Y;
			GI::Screen::Util::clipLimit(&Internals.pDisplay->sClipRegion, &sClipRegion);
		}
	}
	ChildrenWindowSize->X = 0;
	ChildrenWindowSize->Y = 0;

	CursorState back = Cursor_NoAction;
	if(control_comand)
		back = control_comand->Cursor;
	if(control_comand && (Internals.CursorDownOnHeader || Internals.CursorDownOnResizeBtn || refresh_childrens))
		control_comand->Cursor = Cursor_Up;
	Tmp_Children_Cnt = 0;
	while(Tmp_Children_Cnt < Internals.ChildrensNr && Internals.ChildrensNr != 0)
	{
		if(Internals.Childrens[Tmp_Children_Cnt] && Internals.Childrens[Tmp_Children_Cnt]->TabLocNr == SelectedTab)
		{
			gfx_u32 children_type = Internals.Childrens[Tmp_Children_Cnt]->Type;
			void *children = Internals.Childrens[Tmp_Children_Cnt]->Children;
			GI::Screen::Gfx::Window *Window_settings = (GI::Screen::Gfx::Window *)children;
			if((WindowWindowChildren == children_type || WindowTabGroupChildren == children_type) && Window_settings->Visible == true)
			{
				/*if(control_comand->Cursor == Cursor_Down &&
						check_if_inside_box(Window_settings->Internals.Position.X,
							Window_settings->Internals.Position.Y,
								Window_settings->Internals.Position.X +  Window_settings->Internals.Size.X,
									Window_settings->Internals.Position.Y + Window_settings->Internals.Size.Y,
										control_comand->X, control_comand->Y))
					settings->Internals.CursorDownInsideChildrenWindow = true;*/
				tRectangle sClipRegion;
				sClipRegion.sXMin = Window_settings->Internals.Position.X;
				sClipRegion.sYMin = Window_settings->Internals.Position.Y;
				sClipRegion.sXMax = Window_settings->Internals.Position.X +  Window_settings->Internals.Size.X;
				sClipRegion.sYMax = Window_settings->Internals.Position.Y +  Window_settings->Internals.Size.Y;
				GI::Screen::Util::clipLimit(&sClipRegion, &Window_settings->WindowMoveLimits);
				CursorState Cursor = Cursor_NoAction;
				if(control_comand)
					Cursor = control_comand->Cursor;
				if(Cursor == Cursor_Down &&
						GI::Screen::Util::insideBox(sClipRegion.sXMin,
							sClipRegion.sYMin,
								sClipRegion.sXMax,
									sClipRegion.sYMax,
										control_comand->X,
											control_comand->Y))
					Internals.CursorDownInsideChildrenWindow = true;
			}
		}
		Tmp_Children_Cnt++;
	}
	CursorState _back = Cursor_NoAction;
	if(control_comand)
	{
		_back = control_comand->Cursor;
		if(Internals.CursorDownInsideChildrenWindow)
			control_comand->Cursor = Cursor_NoAction;
	}
	//bool _back_cursor_used = control_comand->CursorCoordonateUsed;
	Tmp_Children_Cnt = 0;
	bool back_need_window_refrash = false;
	if(control_comand)
	{
		back_need_window_refrash = control_comand->WindowRefresh;
		control_comand->WindowRefresh = false;
	}
	while(Tmp_Children_Cnt < Internals.ChildrensNr && Internals.ChildrensNr != 0)
	{
		if(Internals.Childrens[Tmp_Children_Cnt] && Internals.Childrens[Tmp_Children_Cnt]->TabLocNr == SelectedTab)
		{
			gfx_u32 children_type = Internals.Childrens[Tmp_Children_Cnt]->Type;
			void *children = Internals.Childrens[Tmp_Children_Cnt]->Children;
			if(WindowButtonChildren == children_type)
			{
				GI::Screen::Gfx::Button *Buton_settings = (GI::Screen::Gfx::Button *)children;
				if(Buton_settings->Visible)
				{
					if(ChildrenWindowSize->X < (Buton_settings->Position.X + Buton_settings->Size.X))
						ChildrenWindowSize->X = Buton_settings->Position.X + Buton_settings->Size.X;
					if(ChildrenWindowSize->Y < (Buton_settings->Position.Y + Buton_settings->Size.Y))
						ChildrenWindowSize->Y = Buton_settings->Position.Y + Buton_settings->Size.Y;
				}
				if(refresh_childrens)
					Buton_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					Buton_settings->Internals.PositionOffset.X = 3 + Internals.Position.ChildrenPosition_X;
					Buton_settings->Internals.PositionOffset.Y = HeaderSize + 1 + Internals.Position.ChildrenPosition_Y;
					Buton_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(Buton_settings->Visible == Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || Buton_settings->Visible == false)
					{
						Buton_settings->idle(control_comand);
					}
					Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = Buton_settings->Visible;
				}
			}
			else if(WindowCheckboxChildren == children_type)
			{
				GI::Screen::Gfx::CheckBox *CheckBox_settings = (GI::Screen::Gfx::CheckBox *)children;
				if(CheckBox_settings->Visible)
				{
					if(ChildrenWindowSize->X < (CheckBox_settings->Position.X + CheckBox_settings->Size.X))
						ChildrenWindowSize->X = CheckBox_settings->Position.X + CheckBox_settings->Size.X;
					if(ChildrenWindowSize->Y < (CheckBox_settings->Position.Y + CheckBox_settings->Size.Y))
						ChildrenWindowSize->Y = CheckBox_settings->Position.Y + CheckBox_settings->Size.Y;
				}
				if(refresh_childrens)
					CheckBox_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					CheckBox_settings->Internals.PositionOffset.X = 3 + Internals.Position.ChildrenPosition_X;
					CheckBox_settings->Internals.PositionOffset.Y = HeaderSize + 1 + Internals.Position.ChildrenPosition_Y;
					CheckBox_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(CheckBox_settings->Visible == Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || CheckBox_settings->Visible == false)
					{
						CheckBox_settings->idle(control_comand);
					}
					Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = CheckBox_settings->Visible;
				}
			}
			else if(WindowListboxChildren == children_type)
			{
				GI::Screen::Gfx::ListBox *ListBox_settings = (GI::Screen::Gfx::ListBox *)children;
				if(ListBox_settings->Visible)
				{
					if(ChildrenWindowSize->X < (ListBox_settings->Position.X + ListBox_settings->Size.X))
						ChildrenWindowSize->X = ListBox_settings->Position.X + ListBox_settings->Size.X;
					if(ChildrenWindowSize->Y < (ListBox_settings->Position.Y + ListBox_settings->Size.Y))
						ChildrenWindowSize->Y = ListBox_settings->Position.Y + ListBox_settings->Size.Y;
				}
				if(refresh_childrens)
					ListBox_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					ListBox_settings->Internals.PositionOffset.X = 3 + Internals.Position.ChildrenPosition_X;
					ListBox_settings->Internals.PositionOffset.Y = HeaderSize + 1 + Internals.Position.ChildrenPosition_Y;
					ListBox_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(ListBox_settings->Visible == Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || ListBox_settings->Visible == false)
					{
						ListBox_settings->idle(control_comand);
					}
					Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = ListBox_settings->Visible;
				}
			}
			else if(WindowProgressbarChildren == children_type)
			{
				GI::Screen::Gfx::ProgressBar *ProgressBar_settings = (GI::Screen::Gfx::ProgressBar *)children;
				if(ProgressBar_settings->Visible)
				{
					if(ChildrenWindowSize->X < (ProgressBar_settings->Position.X + ProgressBar_settings->Size.X))
						ChildrenWindowSize->X = ProgressBar_settings->Position.X + ProgressBar_settings->Size.X;
					if(ChildrenWindowSize->Y < (ProgressBar_settings->Position.Y + ProgressBar_settings->Size.Y))
						ChildrenWindowSize->Y = ProgressBar_settings->Position.Y + ProgressBar_settings->Size.Y;
				}
				if(refresh_childrens)
					ProgressBar_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					ProgressBar_settings->Internals.PositionOffset.X = 3 + Internals.Position.ChildrenPosition_X;
					ProgressBar_settings->Internals.PositionOffset.Y = HeaderSize + 1 + Internals.Position.ChildrenPosition_Y;
					ProgressBar_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(ProgressBar_settings->Visible == Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || ProgressBar_settings->Visible == false)
					{
						ProgressBar_settings->idle(control_comand);
					}
					Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = ProgressBar_settings->Visible;
				}
			}
			else if(WindowScrollbarChildren == children_type)
			{
				GI::Screen::Gfx::ScrollBar *ScrollBar_settings = (GI::Screen::Gfx::ScrollBar *)children;
				if(ScrollBar_settings->Visible)
				{
					if(ChildrenWindowSize->X < (ScrollBar_settings->Position.X + ScrollBar_settings->Size.X))
						ChildrenWindowSize->X = ScrollBar_settings->Position.X + ScrollBar_settings->Size.X;
					if(ChildrenWindowSize->Y < (ScrollBar_settings->Position.Y + ScrollBar_settings->Size.Y))
						ChildrenWindowSize->Y = ScrollBar_settings->Position.Y + ScrollBar_settings->Size.Y;
				}
				if(refresh_childrens)
					ScrollBar_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					ScrollBar_settings->Internals.PositionOffset.X = 3 + Internals.Position.ChildrenPosition_X;
					ScrollBar_settings->Internals.PositionOffset.Y = HeaderSize + 1 + Internals.Position.ChildrenPosition_Y;
					ScrollBar_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(ScrollBar_settings->Visible == Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || ScrollBar_settings->Visible == false)
					{
						ScrollBar_settings->idle(control_comand);
					}
					Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = ScrollBar_settings->Visible;
				}
			}
			else if(WindowTextboxChildren == children_type)
			{
				GI::Screen::Gfx::TextBox *TextBox_settings = (GI::Screen::Gfx::TextBox *)children;
				if(TextBox_settings->Visible)
				{
					if(ChildrenWindowSize->X < (TextBox_settings->Position.X + TextBox_settings->Size.X))
						ChildrenWindowSize->X = TextBox_settings->Position.X + TextBox_settings->Size.X;
					if(ChildrenWindowSize->Y < (TextBox_settings->Position.Y + TextBox_settings->Size.Y))
						ChildrenWindowSize->Y = TextBox_settings->Position.Y + TextBox_settings->Size.Y;
				}
				if(refresh_childrens)
					TextBox_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					TextBox_settings->Internals.PositionOffset.X = 3 + Internals.Position.ChildrenPosition_X;
					TextBox_settings->Internals.PositionOffset.Y = HeaderSize + 1 + Internals.Position.ChildrenPosition_Y;
					TextBox_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(TextBox_settings->Visible == Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || TextBox_settings->Visible == false)
					{
						TextBox_settings->idle(control_comand);
					}
					Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = TextBox_settings->Visible;
				}
			}
			else if(WindowPictureboxChildren == children_type)
			{
				GI::Screen::Gfx::Picturebox *PictureBox_settings = (GI::Screen::Gfx::Picturebox *)children;
				if(PictureBox_settings->Visible)
				{
					if(ChildrenWindowSize->X < (PictureBox_settings->Position.X + PictureBox_settings->Size.X))
						ChildrenWindowSize->X = PictureBox_settings->Position.X + PictureBox_settings->Size.X;
					if(ChildrenWindowSize->Y < (PictureBox_settings->Position.Y + PictureBox_settings->Size.Y))
						ChildrenWindowSize->Y = PictureBox_settings->Position.Y + PictureBox_settings->Size.Y;
				}
				if(refresh_childrens)
					PictureBox_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					PictureBox_settings->Internals.PositionOffset.X = 3 + Internals.Position.ChildrenPosition_X;
					PictureBox_settings->Internals.PositionOffset.Y = HeaderSize + 1 + Internals.Position.ChildrenPosition_Y;
					PictureBox_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(PictureBox_settings->Visible == Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || PictureBox_settings->Visible == false)
					{
						PictureBox_settings->idle(control_comand);
					}
					Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = PictureBox_settings->Visible;
				}
			}
			else if(WindowKbdQwertyChildren == children_type)
			{
				GI::Screen::Gfx::KbdQwerty *Keyboard_settings = (GI::Screen::Gfx::KbdQwerty *)children;
				if(Keyboard_settings->Visible)
				{
					if(ChildrenWindowSize->X < (Keyboard_settings->Position.X + Keyboard_settings->Size.X))
						ChildrenWindowSize->X = Keyboard_settings->Position.X + Keyboard_settings->Size.X;
					if(ChildrenWindowSize->Y < (Keyboard_settings->Position.Y + Keyboard_settings->Size.Y))
						ChildrenWindowSize->Y = Keyboard_settings->Position.Y + Keyboard_settings->Size.Y;
				}
				if(refresh_childrens)
					Keyboard_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					Keyboard_settings->Internals.PositionOffset.X = 3 + Internals.Position.ChildrenPosition_X;
					Keyboard_settings->Internals.PositionOffset.Y = HeaderSize + 1 + Internals.Position.ChildrenPosition_Y;
					Keyboard_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(Keyboard_settings->Visible == Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || Keyboard_settings->Visible == false)
					{
						Keyboard_settings->idle(control_comand);
					}
					Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = Keyboard_settings->Visible;
				}
			}
		}
		Tmp_Children_Cnt++;
	}
	if(control_comand) control_comand->Cursor = _back;
	//if(ParentWindow->Internals.CursorDownInsideChildrenWindow) control_comand->CursorCoordonateUsed = _back_cursor_used;
	Tmp_Children_Cnt = 0;
	while(Tmp_Children_Cnt < Internals.ChildrensNr && Internals.ChildrensNr != 0)
	{
		if(Internals.Childrens[Tmp_Children_Cnt] && Internals.Childrens[Tmp_Children_Cnt]->TabLocNr == SelectedTab)
		{
			gfx_u32 children_type = Internals.Childrens[Tmp_Children_Cnt]->Type;
			void *children = Internals.Childrens[Tmp_Children_Cnt]->Children;
			if(WindowWindowChildren == children_type || WindowTabGroupChildren == children_type)
			{
				GI::Screen::Gfx::Window *windowHandler = (GI::Screen::Gfx::Window *)children;
				if(windowHandler->Visible)
				{
					if(ChildrenWindowSize->X < (windowHandler->Position.X + windowHandler->Size.X))
						ChildrenWindowSize->X = windowHandler->Position.X + windowHandler->Size.X;
					if(ChildrenWindowSize->Y < (windowHandler->Position.Y + windowHandler->Size.Y))
						ChildrenWindowSize->Y = windowHandler->Position.Y + windowHandler->Size.Y;
				}
				bool tmp_win_rfsh = false;
				if(control_comand)
					tmp_win_rfsh = control_comand->WindowRefresh;
				if(refresh_childrens || tmp_win_rfsh)
					windowHandler->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					windowHandler->Internals.PositionOffset.X = 3 + Internals.Position.ChildrenPosition_X;
					windowHandler->Internals.PositionOffset.Y = HeaderSize + 1 + Internals.Position.ChildrenPosition_Y;
					windowHandler->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(windowHandler->Visible == Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || windowHandler->Visible == false)
					{
						windowHandler->idle(control_comand);
					}
					Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = windowHandler->Visible;
				}
			}
		}
		Tmp_Children_Cnt++;
	}
	if(control_comand)
		control_comand->WindowRefresh |= back_need_window_refrash;
	if(_back == Cursor_Up || _back == Cursor_NoAction)
		Internals.CursorDownInsideChildrenWindow = false;
	if(control_comand) control_comand->Cursor = back;
	if(Internals.V_ScrollBar && Internals.H_ScrollBar)
		Internals.pDisplay->sClipRegion = sClipRegion;
}

void GI::Screen::Gfx::Window::idle(tControlCommandData* control_comand)
{
	if(!this)
		return;
	GI::Screen::Gfx::Window *parentWindowHandler = (GI::Screen::Gfx::Window *)Internals.parentWindowHandler;
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
		Internals.WindowMoveLimits.sXMin = WindowMoveLimits.sXMin;
		Internals.WindowMoveLimits.sYMin = WindowMoveLimits.sYMin;
		Internals.WindowMoveLimits.sXMax = WindowMoveLimits.sXMax;
		Internals.WindowMoveLimits.sYMax = WindowMoveLimits.sYMax;
		if(Internals.FullScreen)
		{
			Position.X = Internals.WindowMoveLimits.sXMin;
			Position.Y = Internals.WindowMoveLimits.sYMin;
			Size.X = WindowMoveLimits.sXMax - Internals.WindowMoveLimits.sXMin;
			Size.Y = WindowMoveLimits.sYMax - Internals.WindowMoveLimits.sYMin;
		}
		else
		{
			Internals.Header.MaxMin->Events.Checked = false;
			Size.X = SizeNormalScreen.X;
			Size.Y = SizeNormalScreen.Y;
			Position.X = Internals.NormalScreenPosition.X;
			Position.Y = Internals.NormalScreenPosition.Y;
			Internals.Size.X = SizeNormalScreen.X;
			Internals.Size.Y = SizeNormalScreen.Y;
			Internals.Position.X = Internals.NormalScreenPosition.X;
			Internals.Position.Y = Internals.NormalScreenPosition.Y;
			Internals.FullScreen = false;
			Internals.Position.ChildrenPosition_X = 0;
			Internals.Position.ChildrenPosition_Y = 0;
			parentWindowHandler->Internals.Position.ChildrenPosition_X = 0;
			parentWindowHandler->Internals.Position.ChildrenPosition_Y = 0;
			//settings->Internals.H_ScrollBar->Value = 0;
			//settings->Internals.V_ScrollBar->Value = 0;
			parentWindowHandler->Internals.NeedEntireRefresh = true;
		}
		if(parentWindowHandler != NULL && this != parentWindowHandler)
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
		Internals.NormalScreenPosition.X = Position.X;
		Internals.NormalScreenPosition.Y = Position.Y;
		Internals.Header.Close = new GI::Screen::Gfx::Button(Internals.parentWindowHandler);
		if(!Internals.Header.Close)
			return;

		Internals.Header.MaxMin = new GI::Screen::Gfx::CheckBox(Internals.parentWindowHandler);
		if(!Internals.Header.MaxMin)
			return;
		if(Internals.FullScreen)
			Internals.Header.MaxMin->Cheched = false;
		else
			Internals.Header.MaxMin->Cheched = true;

		Internals.H_ScrollBar = new GI::Screen::Gfx::ScrollBar(Internals.parentWindowHandler);
		Internals.V_ScrollBar = new GI::Screen::Gfx::ScrollBar(Internals.parentWindowHandler);

		Internals.Header.Minimize = new GI::Screen::Gfx::Button(Internals.parentWindowHandler);
		if(!Internals.Header.Minimize)
			return;

		Internals.Header.TabGroupScrollLeft = new GI::Screen::Gfx::Button(Internals.parentWindowHandler);
		if(!Internals.Header.TabGroupScrollLeft)
			return;

		Internals.Header.TabGroupScrollRight = new GI::Screen::Gfx::Button(Internals.parentWindowHandler);
		if(!Internals.Header.TabGroupScrollRight)
			return;

		//settings->Internals.Header.TabGroupTabsList = (tCheckBox **)calloc(1, sizeof(tCheckBox *));
		//if(!settings->Internals.Header.TabGroupTabsList) return;

		//settings->Internals.Header.TabGroupTabsList[0] = new_checkbox(settings->Internals.parentWindowHandler);
		//if(!settings->Internals.Header.TabGroupTabsList[0]) return;
		//settings->Internals.Header.TabGroupTabsList[0]->Cheched = true;
		//settings->Internals.Header.TabGroupTabsList[0]->Caption.Font = (tFont *)&g_sFontCm12;
		//settings->Internals.Header.TabGroupTabsList[0]->Color.Scren = controls_color.Control_Color_Enabled_Buton_Push;
		//settings->Internals.Header.TabGroupTabsListNr = 0;


		/*settings->Internals.Header.Resize = new_button(settings->Internals.parentWindowHandler);
		if(!settings->Internals.Header.Resize) return;*/
		if(Internals.FullScreen == true)
		{
			SizeNormalScreen.X = Size.X;
			SizeNormalScreen.Y = Size.Y;
		}

		ChildrenWindowSize_t ChildrenWindowSize;
		setChildrens(false, true, control_comand, false, &ChildrenWindowSize);
		Internals.NeedEntireRefresh = true;
	}

	bool text_caption_has_changed = false;
	gfx_s32 X_StartBox = Internals.Position.X;
	gfx_s32 Y_StartBox = Internals.Position.Y;
	gfx_s32 X_LenBox = Internals.Size.X;
	gfx_s32 Y_LenBox = Internals.Size.Y;
	void *pDisplay = Internals.pDisplay;
	GI::Dev::Screen* LcdStruct = (GI::Dev::Screen *) pDisplay;

	if(parentWindowHandler != NULL && this != parentWindowHandler)
	{
			if(Internals.Caption->pFont != Caption->pFont ||
					Caption->modifyed||
						Internals.Caption->textAlign != Caption->textAlign ||
							Internals.Caption->wordWrap != Caption->wordWrap)
			{
				text_caption_has_changed = true;
			}
	}
	else
	{
		if(Internals.Caption->pFont != Caption->pFont ||
				Caption->modifyed ||
					Internals.Caption->textAlign != Caption->textAlign ||
						Internals.Caption->wordWrap != Caption->wordWrap)
		{
			text_caption_has_changed = true;
		}
	}

	if(text_caption_has_changed && !Internals.OldTabGroupMode && Internals.Control.Initiated == true)
	{
		tRectangle back_up_clip = LcdStruct->sClipRegion;
		Internals.Caption->pFont = Caption->pFont;
		Caption->modifyed = 0;
		Internals.Caption->textAlign = Caption->textAlign;
		Internals.Caption->wordWrap = Caption->wordWrap;
		gfx_s32 HeaderSize = 0;
		if(Internals.HideHeader == false)
			HeaderSize = Internals.Header.Size.Y;
		gfx_u32 header_btn_size = HeaderSize - 6;
		//gfx_u32 header_btn_space = HeaderSize - 5;
		tRectangle back_up_clip_header = LcdStruct->sClipRegion;
		gfx_s32 _X_StartBox_, _Y_StartBox_, _X_LenBox_, _Y_LenBox_;;
		_X_StartBox_ = (Internals.Position.X + 2);
		_X_LenBox_ = (Internals.Size.X + Internals.Position.X) - ((header_btn_size + 2) * 3);
		_Y_StartBox_ = (2 + Internals.Position.Y);
		_Y_LenBox_ = (2 + Internals.Position.Y) + header_btn_size;
		LcdStruct->sClipRegion.sXMin = _X_StartBox_ + 1;
		LcdStruct->sClipRegion.sXMax = _X_LenBox_ - 1;
		LcdStruct->sClipRegion.sYMin = _Y_StartBox_ + 1;
		LcdStruct->sClipRegion.sYMax = _Y_LenBox_ - 1;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip_header);
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &WindowMoveLimits);
		gfx_s32 x_str_location = LcdStruct->sClipRegion.sXMin;
		gfx_s32 y_str_location = LcdStruct->sClipRegion.sYMin;
		LcdStruct->drawRectangle(_X_StartBox_ + 1, _Y_StartBox_ + 1, _X_LenBox_ - 2, _Y_LenBox_ - 1, true, WindowColor.Enabled.WindowHeader);
		//LcdStruct->put_rectangle(pDisplay, X_StartBox + 1, (Y_StartBox + HeaderSize) - 1, X_LenBox - 2, Y_LenBox - HeaderSize - 0, true, controls_color.Scren);
		LcdStruct->cacheClean(_X_StartBox_, _Y_StartBox_, _X_LenBox_, _Y_LenBox_);
		if(Caption->textAlign == alignCenter){
			StringProperties_t str_properties = Caption->getStrProp();
			x_str_location = _X_StartBox_ + ((_X_LenBox_>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
			y_str_location = _Y_StartBox_ + ((_Y_LenBox_>>1)-(str_properties.StringColsHeight_Pixels>>1));
		}
		Caption->ulOpaque = false;
		Caption->ulVisible = true;
		Caption->lX = x_str_location;
		Caption->lY = y_str_location;
		Caption->_SelStart = 0;
		Caption->_SelLen = 0;
		Caption->foreground_color = Color.Enabled.Ink.Pull;
		Caption->background_color = Color.Enabled.Buton.Pull;
		Caption->drawString();
		LcdStruct->sClipRegion = back_up_clip;
	}

	/* Verify if position on size has been modified */
	if(parentWindowHandler != NULL && this != parentWindowHandler)
	{
		if((Position.X + parentWindowHandler->Internals.Position.X + Internals.PositionOffset.X) != Internals.Position.X ||
				(Position.Y + parentWindowHandler->Internals.Position.Y + Internals.PositionOffset.Y) != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							//settings->Internals.Caption.Font != settings->Caption.Font ||
								//settings->Caption.Text->modifyed == true ||
									//settings->Internals.Caption.TextAlign != settings->Caption.TextAlign ||
										//settings->Internals.Caption.WordWrap != settings->Caption.WordWrap ||
											Internals.WindowMoveLimits.sXMin != WindowMoveLimits.sXMin ||
												Internals.WindowMoveLimits.sYMin != WindowMoveLimits.sYMin ||
													Internals.WindowMoveLimits.sXMax != WindowMoveLimits.sXMax ||
														Internals.WindowMoveLimits.sYMax != WindowMoveLimits.sYMax ||
															Internals.HideHeader != HideHeader ||
																Internals.OldTabGroupMode != Internals.TabGroupMode ||
																	Internals.CloseButonEnabled != CloseButonEnabled ||
																		Internals.CloseButonVisible != CloseButonVisible ||
																			Internals.MaxMinEnabled != MaxMinEnabled ||
																				Internals.MaxMinVisible != MaxMinVisible ||
																					Internals.MinimizeButonEnabled != MinimizeButonEnabled ||
																						Internals.MinimizeButonVisible != MinimizeButonVisible)
		{
			Internals.NeedEntireRefresh = true;
			Caption->modifyed = 0;
		}
	}
	else
	{
		if(Position.X != Internals.Position.X ||
				Position.Y != Internals.Position.Y ||
					Size.X != Internals.Size.X ||
						Size.Y != Internals.Size.Y ||
							//settings->Internals.Caption.Font != settings->Caption.Font ||
								//settings->Caption.Text->modifyed == true ||
									//settings->Internals.Caption.TextAlign != settings->Caption.TextAlign ||
										//settings->Internals.Caption.WordWrap != settings->Caption.WordWrap ||
											Internals.WindowMoveLimits.sXMin != WindowMoveLimits.sXMin ||
												Internals.WindowMoveLimits.sYMin != WindowMoveLimits.sYMin ||
													Internals.WindowMoveLimits.sXMax != WindowMoveLimits.sXMax ||
														Internals.WindowMoveLimits.sYMax != WindowMoveLimits.sYMax ||
															Internals.HideHeader != HideHeader ||
																Internals.OldTabGroupMode != Internals.TabGroupMode ||
																	Internals.CloseButonEnabled != CloseButonEnabled ||
																		Internals.CloseButonVisible != CloseButonVisible ||
																			Internals.MaxMinEnabled != MaxMinEnabled ||
																				Internals.MaxMinVisible != MaxMinVisible ||
																					Internals.MinimizeButonEnabled != MinimizeButonEnabled ||
																						Internals.MinimizeButonVisible != MinimizeButonVisible)
		{
			Internals.NeedEntireRefresh = true;
			Caption->modifyed = 0;
		}
	}

	/*if(settings->Internals.OldStateEnabled == false || settings->Internals.OldStateVisible == false)
	{
		control_comand->CursorCoordonateUsed = false;
		return;
	}*/
	//if(settings->Internals.Caption.Text != NULL && settings->Caption.Text != NULL && strcmp(settings->Internals.Caption.Text, settings->Caption.Text) == NULL)
		//settings->Internals.NeedEntireRefresh = true;

	/*Clear background of box with actual painted dimensions and positions if they been changed*/
	if(!Visible)
		Internals.NeedEntireRefresh = false;
	if(Internals.NeedEntireRefresh == true || Internals.OldStateVisible != Visible)
	{
		if(!Internals.NoPaintBackGround || !Visible)
		{
			tRectangle back_up_clip = LcdStruct->sClipRegion;
			LcdStruct->sClipRegion.sXMin = X_StartBox;
			LcdStruct->sClipRegion.sYMin = Y_StartBox;
			LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
			LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &WindowMoveLimits);
			LcdStruct->drawRectangle(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, true, Color.Scren);
			LcdStruct->cacheClean(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
			LcdStruct->sClipRegion = back_up_clip;
		}
		if(Internals.OldStateVisible != Visible)
		{
			Internals.OldStateVisible = Visible;
			if(!Visible)
			{
				control_comand->CursorCoordonateUsed = true;
				if(parentWindowHandler != NULL && this != parentWindowHandler)
				{
					parentWindowHandler->Internals.NeedEntireRefresh = true;
					Internals.NeedEntireRefresh = false;
				}
				return;
			}
		}
		Internals.NeedEntireRefresh = true;
	}
	if(!Visible)
	{
		Internals.NeedEntireRefresh = false;
		return;
	}
	/* Verify if is Entire refresh, entire repaint, or state changed */
	if(Visible == true && (
			Internals.NeedEntireRefresh == true ||
				Internals.NeedEntireRepaint == true ||
					Internals.Control.Initiated == false ||
						Internals.OldStateEnabled != Enabled ||
							Internals.OldStateVisible != Visible ||
								Internals.Size.ScrollBarSize != Size.ScrollBarSize))
	{
		/* Copy new locations and dimensions to actual locations and dimensions */
		if(parentWindowHandler != NULL && this != parentWindowHandler)
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
		//settings->Internals.Caption.Font = settings->Caption.Font;
		////settings->Internals.Caption.Text = settings->Caption.Text;
		//settings->Internals.Caption.TextAlign = settings->Caption.TextAlign;
		//settings->Internals.Caption.WordWrap = settings->Caption.WordWrap;
		Internals.Size.ScrollBarSize = Size.ScrollBarSize;
		Internals.WindowMoveLimits.sXMin = WindowMoveLimits.sXMin;
		Internals.WindowMoveLimits.sYMin = WindowMoveLimits.sYMin;
		Internals.WindowMoveLimits.sXMax = WindowMoveLimits.sXMax;
		Internals.WindowMoveLimits.sYMax = WindowMoveLimits.sYMax;
		Internals.OldTabGroupMode = Internals.TabGroupMode;
		Internals.CloseButonEnabled = CloseButonEnabled;
		Internals.CloseButonVisible = CloseButonVisible;
		Internals.MaxMinEnabled = MaxMinEnabled;
		Internals.MaxMinVisible = MaxMinVisible;
		Internals.MinimizeButonEnabled = MinimizeButonEnabled;
		Internals.MinimizeButonVisible = MinimizeButonVisible;
		Internals.HideHeader = HideHeader;
		X_StartBox = Internals.Position.X;
		Y_StartBox = Internals.Position.Y;
		X_LenBox = Internals.Size.X;
		Y_LenBox = Internals.Size.Y;
		//settings->Internals.Caption.Text = gfx_change_str(settings->Internals.Caption.Text, settings->Caption.Text);
		//settings->Caption.Text = settings->Internals.Caption.Text;

		gfx_s32 HeaderSize = 0;
		if(Internals.HideHeader == false)
			HeaderSize = Internals.Header.Size.Y;
		gfx_u32 header_btn_size = HeaderSize - 6;
		gfx_u32 header_btn_space = HeaderSize - 5;

		Internals.Header.Close->Size.X = header_btn_size;
		Internals.Header.Close->Size.Y = header_btn_size;
		if(parentWindowHandler != NULL && this != parentWindowHandler)
		{
			Internals.Header.Close->Position.X = (Internals.Size.X + Internals.Position.X) - parentWindowHandler->Internals.Position.X - header_btn_space - 1;
			Internals.Header.Close->Position.Y = (2 + Internals.Position.Y) - parentWindowHandler->Internals.Position.Y;
		}
		else
		{
			Internals.Header.Close->Position.X = (Internals.Size.X) - header_btn_space - 1;
			Internals.Header.Close->Position.Y = 2;
		}
		Internals.Header.Close->Caption->clear();
		Internals.Header.Close->Internals.IsChildren = true;
		Internals.Header.Close->Internals.NoPaintBackGround = true;
		Internals.Header.Close->Internals.NeedEntireRefresh = true;
		Internals.Header.Close->Enabled = Internals.CloseButonEnabled;
		Internals.Header.Close->Visible = Internals.CloseButonVisible;

		Internals.Header.MaxMin->Size.X = header_btn_size;
		Internals.Header.MaxMin->Size.Y = header_btn_size;
		if(parentWindowHandler != NULL && this != parentWindowHandler)
		{
			Internals.Header.MaxMin->Position.X = (Internals.Size.X + Internals.Position.X) - parentWindowHandler->Internals.Position.X - (header_btn_space << 1) - 1;
			Internals.Header.MaxMin->Position.Y = (2 + Internals.Position.Y) - parentWindowHandler->Internals.Position.Y;
		}
		else
		{
			Internals.Header.MaxMin->Position.X = (Internals.Size.X) - (header_btn_space << 1) - 1;
			Internals.Header.MaxMin->Position.Y = 2;
		}
		Internals.Header.MaxMin->Caption->clear();
		Internals.Header.MaxMin->Internals.NoPaintBackGround = true;
		Internals.Header.MaxMin->Internals.NeedEntireRefresh = true;
		Internals.Header.MaxMin->Enabled = Internals.MaxMinEnabled;
		Internals.Header.MaxMin->Visible = Internals.MaxMinVisible;

		Internals.Header.Minimize->Size.X = header_btn_size;
		Internals.Header.Minimize->Size.Y = header_btn_size;
		if(parentWindowHandler != NULL && this != parentWindowHandler)
		{
			Internals.Header.Minimize->Position.X = (Internals.Size.X + Internals.Position.X) - parentWindowHandler->Internals.Position.X - ((header_btn_space << 1) + header_btn_space) - 1;
			Internals.Header.Minimize->Position.Y = (2 + Internals.Position.Y) - parentWindowHandler->Internals.Position.Y;
		}
		else
		{
			Internals.Header.Minimize->Position.X = (Internals.Size.X) - ((header_btn_space << 1) + header_btn_space) - 1;
			Internals.Header.Minimize->Position.Y = 2;
		}
		Internals.Header.Minimize->Caption->clear();
		Internals.Header.Minimize->Internals.NoPaintBackGround = true;
		Internals.Header.Minimize->Internals.IsChildren = true;
		Internals.Header.Minimize->Internals.NeedEntireRefresh = true;
		Internals.Header.Minimize->Enabled = Internals.MinimizeButonEnabled;
		Internals.Header.Minimize->Visible = Internals.MinimizeButonVisible;

		Internals.Header.TabGroupScrollLeft->Size.X = header_btn_size;
		Internals.Header.TabGroupScrollLeft->Size.Y = header_btn_size;
		if(parentWindowHandler != NULL && this != parentWindowHandler)
		{
			Internals.Header.TabGroupScrollLeft->Position.X = (Internals.Position.X + 2) - parentWindowHandler->Internals.Position.X;
			Internals.Header.TabGroupScrollLeft->Position.Y = (2 + Internals.Position.Y) - parentWindowHandler->Internals.Position.Y;
		}
		else
		{
			Internals.Header.TabGroupScrollLeft->Position.X = (Internals.Position.X + 2);
			Internals.Header.TabGroupScrollLeft->Position.Y = 2;
		}
		Internals.Header.TabGroupScrollLeft->Caption->setText((char *)"L");
		Internals.Header.TabGroupScrollLeft->Caption->wordWrap = false;
		//settings->Internals.Header.TabGroupScrollLeft->Caption.Font = &g_sFontCm12;
		Internals.Header.TabGroupScrollLeft->Internals.NoPaintBackGround = true;
		Internals.Header.TabGroupScrollLeft->Internals.IsChildren = true;
		Internals.Header.TabGroupScrollLeft->Internals.NeedEntireRefresh = true;

		Internals.Header.TabGroupScrollRight->Size.X = header_btn_size;
		Internals.Header.TabGroupScrollRight->Size.Y = header_btn_size;
		if(parentWindowHandler != NULL && this != parentWindowHandler)
		{
			Internals.Header.TabGroupScrollRight->Position.X = (Internals.Size.X + Internals.Position.X) - parentWindowHandler->Internals.Position.X - header_btn_space - 1;
			Internals.Header.TabGroupScrollRight->Position.Y = (2 + Internals.Position.Y) - parentWindowHandler->Internals.Position.Y;
		}
		else
		{
			Internals.Header.TabGroupScrollRight->Position.X = (Internals.Size.X) - header_btn_space - 1;
			Internals.Header.TabGroupScrollRight->Position.Y = 2;
		}
		Internals.Header.TabGroupScrollRight->Caption->setText((char *)"R");
		Internals.Header.TabGroupScrollRight->Caption->wordWrap = false;
		//settings->Internals.Header.TabGroupScrollRight->Caption.Font = &g_sFontCm12;
		Internals.Header.TabGroupScrollRight->Internals.NoPaintBackGround = true;
		Internals.Header.TabGroupScrollRight->Internals.IsChildren = true;
		Internals.Header.TabGroupScrollRight->Internals.NeedEntireRefresh = true;

		if(parentWindowHandler != NULL && this != parentWindowHandler)
		{
			Internals.H_ScrollBar->Position.X = (3 + Internals.Position.X) - parentWindowHandler->Internals.Position.X;
			Internals.H_ScrollBar->Position.Y = (Internals.Size.Y + Internals.Position.Y) - parentWindowHandler->Internals.Position.Y - Internals.Size.ScrollBarSize - 1;
		}
		else
		{
			Internals.H_ScrollBar->Position.X = 3;
			Internals.H_ScrollBar->Position.Y = (Internals.Size.Y) - Internals.Size.ScrollBarSize - 2;
		}
		Internals.H_ScrollBar->Internals.NeedEntireRefresh = true;
		Internals.H_ScrollBar->Internals.NoPaintBackGround = true;

		if(parentWindowHandler != NULL && this != parentWindowHandler)
		{
			Internals.V_ScrollBar->Position.X = (Internals.Size.X + Internals.Position.X) - parentWindowHandler->Internals.Position.X - Internals.Size.ScrollBarSize - 2;
			Internals.V_ScrollBar->Position.Y = (HeaderSize + Internals.Position.Y) - parentWindowHandler->Internals.Position.Y;
		}
		else
		{
			Internals.V_ScrollBar->Position.X = (Internals.Size.X) - Internals.Size.ScrollBarSize - 2;
			Internals.V_ScrollBar->Position.Y = HeaderSize;
		}
		Internals.V_ScrollBar->Internals.NeedEntireRefresh = true;
		Internals.V_ScrollBar->Internals.NoPaintBackGround = true;

		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = X_StartBox;
		LcdStruct->sClipRegion.sYMin = Y_StartBox;
		LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
		LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
		GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &WindowMoveLimits);
		LcdStruct->drawHLine(X_StartBox, X_LenBox, Y_StartBox, 1, controlls_change_color(WindowColor.Enabled.WindowBorder, +BORDER_LINE_TWO_DARK));
		LcdStruct->drawHLine(X_StartBox, X_LenBox, (Y_StartBox + Y_LenBox) - 1, 1, controlls_change_color(WindowColor.Enabled.WindowBorder, -BORDER_LINE_TWO_DARK));

		LcdStruct->drawVLine(Y_StartBox, Y_LenBox, X_StartBox, 1, controlls_change_color(WindowColor.Enabled.WindowBorder, +BORDER_LINE_TWO_DARK));
		LcdStruct->drawVLine(Y_StartBox, Y_LenBox, (X_StartBox + X_LenBox) - 1, 1, controlls_change_color(WindowColor.Enabled.WindowBorder, -BORDER_LINE_TWO_DARK));

		//put_rectangle(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, false, settings->WindowColor.Enabled.WindowBorder);
		LcdStruct->drawRectangle(X_StartBox + 1, Y_StartBox + 1, X_LenBox - 2, HeaderSize - 1, true, WindowColor.Enabled.WindowHeader);
		LcdStruct->drawRectangle(X_StartBox + 1, (Y_StartBox + HeaderSize) - 1, X_LenBox - 2, Y_LenBox - HeaderSize - 0, true, controls_color.Scren);
		LcdStruct->cacheClean(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);


		CursorState cursor = control_comand->Cursor;
		control_comand->Cursor = Cursor_Up;
		//ControlCommands control_comand_comand = control_comand->Comand;
		ChildrenWindowSize_t ChildrenWindowSize;
		setChildrens(false, false, control_comand, false, &ChildrenWindowSize);
		Internals.ChildrenWindowSize.X = ChildrenWindowSize.X;
		Internals.ChildrenWindowSize.Y = ChildrenWindowSize.Y;

		Internals.H_ScrollBar->Maximum = ChildrenWindowSize.X - (Size.X - 8 - Internals.Size.ScrollBarSize);
		Internals.V_ScrollBar->Maximum = ChildrenWindowSize.Y - (Size.Y - 6 - Internals.Size.ScrollBarSize - HeaderSize);
		if((Internals.V_ScrollBar->Maximum > 0 && HideVScroll == false/* || settings->ShowVScroll == true*/)/* && settings->AllowVScroll*/)
		{
			Internals.V_ScrollBar->Size.X = Internals.Size.ScrollBarSize;
		}
		else
		{
			Internals.V_ScrollBar->Maximum = 0;
			Internals.V_ScrollBar->Value = 0;
			Internals.V_ScrollBar->Size.X = 0;
		}
		if((Internals.H_ScrollBar->Maximum > 0 && HideHScroll == false/* || settings->ShowHScroll == true*/)/* && settings->AllowHScroll*/)
		{
			Internals.H_ScrollBar->Size.Y = Internals.Size.ScrollBarSize;
		}
		else
		{
			Internals.H_ScrollBar->Maximum = 0;
			Internals.H_ScrollBar->Value = 0;
			Internals.H_ScrollBar->Size.Y = 0;
		}

		Internals.V_ScrollBar->Size.Y = Internals.Size.Y - 2 - Internals.Size.ScrollBarSize - HeaderSize;
		Internals.H_ScrollBar->Size.X = Internals.Size.X - 5 - Internals.Size.ScrollBarSize;


		control_comand->Cursor = Cursor_Up;
		setChildrens(true, true, control_comand, true, &ChildrenWindowSize);
		control_comand->Cursor = Cursor_Up;
		setChildrens(true, true, control_comand, true, &ChildrenWindowSize);
		if(Internals.OldTabGroupMode == false)
		{
			if(Internals.HideHeader == false) {
				Internals.Header.Close->idle(control_comand);
				Internals.Header.MaxMin->idle(control_comand);
				Internals.Header.Minimize->idle(control_comand);
			}
		}
		else
		{
			Internals.Header.TabGroupScrollLeft->idle(control_comand);
			Internals.Header.TabGroupScrollRight->idle(control_comand);
		}
		Internals.H_ScrollBar->idle(control_comand);
		Internals.V_ScrollBar->idle(control_comand);

		if(Internals.OldTabGroupMode)
		{
			tRectangle back_up_clip_header = LcdStruct->sClipRegion;
			gfx_s32 _X_StartBox_, _Y_StartBox_, _X_LenBox_, _Y_LenBox_;;
			_X_StartBox_ = (Internals.Position.X + header_btn_space + 2);
			_X_LenBox_ = (Internals.Size.X + Internals.Position.X) - header_btn_space - 2;
			_Y_StartBox_ = (2 + Internals.Position.Y);
			_Y_LenBox_ = (2 + Internals.Position.Y) + header_btn_size;
			LcdStruct->sClipRegion.sXMin = _X_StartBox_;
			LcdStruct->sClipRegion.sXMax = _X_LenBox_;
			LcdStruct->sClipRegion.sYMin = _Y_StartBox_;
			LcdStruct->sClipRegion.sYMax = _Y_LenBox_;
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip_header);
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &WindowMoveLimits);
			LcdStruct->drawRectangle(_X_StartBox_, _Y_StartBox_, _X_LenBox_ - _X_StartBox_, _Y_LenBox_ - _Y_StartBox_, false, WindowColor.Enabled.WindowBorder);
			LcdStruct->sClipRegion.sXMin = _X_StartBox_ + 1;
			LcdStruct->sClipRegion.sXMax = _X_LenBox_ - 1;
			LcdStruct->sClipRegion.sYMin = _Y_StartBox_ + 1;
			LcdStruct->sClipRegion.sYMax = _Y_LenBox_ - 1;
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip_header);
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &WindowMoveLimits);
			gfx_u32 TabGroupCnt = 0;
			while(TabGroupCnt < Internals.Header.TabGroupTabsListNr && Internals.Header.TabGroupTabsList[TabGroupCnt])
			{
				Internals.Header.TabGroupTabsList[TabGroupCnt]->Position.X = ((_X_StartBox_ + 1 + (Internals.Header.Size.TabGroupSelectorSize * TabGroupCnt) - Internals.Header.TabGroupSelectorPosition) - parentWindowHandler->Internals.Position.X);
				Internals.Header.TabGroupTabsList[TabGroupCnt]->Position.Y = (_Y_StartBox_ + 1) - parentWindowHandler->Internals.Position.Y;
				Internals.Header.TabGroupTabsList[TabGroupCnt]->Size.X = Internals.Header.Size.TabGroupSelectorSize;
				Internals.Header.TabGroupTabsList[TabGroupCnt]->Size.Y = _Y_LenBox_ - _Y_StartBox_ - 2;
				Internals.Header.TabGroupTabsList[TabGroupCnt]->Internals.NeedEntireRefresh = true;
				Internals.Header.TabGroupTabsList[TabGroupCnt]->idle(control_comand);
				TabGroupCnt++;
			}

			//pDisplay->sClipRegion = back_up_clip_header;
		}
		else
		{
			tRectangle back_up_clip_header = LcdStruct->sClipRegion;
			gfx_s32 _X_StartBox_, _Y_StartBox_, _X_LenBox_, _Y_LenBox_;;
			_X_StartBox_ = (Internals.Position.X + 2);
			_X_LenBox_ = (Internals.Size.X + Internals.Position.X) - ((header_btn_size + 2) * 3);
			_Y_StartBox_ = (2 + Internals.Position.Y);
			_Y_LenBox_ = (2 + Internals.Position.Y) + header_btn_size;
			/*LcdStruct->sClipRegion.sXMin = _X_StartBox_;
			LcdStruct->sClipRegion.sXMax = _X_LenBox_;
			LcdStruct->sClipRegion.sYMin = _Y_StartBox_;
			LcdStruct->sClipRegion.sYMax = _Y_LenBox_;
			GScreen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip_header);
			GScreen::Util::clipLimit(&LcdStruct->sClipRegion, &settings->WindowMoveLimits);
			LcdStruct->put_rectangle(pDisplay, _X_StartBox_, _Y_StartBox_, _X_LenBox_ - _X_StartBox_, _Y_LenBox_ - _Y_StartBox_, false, settings->WindowColor.Enabled.WindowBorder);*/
			LcdStruct->sClipRegion.sXMin = _X_StartBox_ + 1;
			LcdStruct->sClipRegion.sXMax = _X_LenBox_ - 1;
			LcdStruct->sClipRegion.sYMin = _Y_StartBox_ + 1;
			LcdStruct->sClipRegion.sYMax = _Y_LenBox_ - 1;
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip_header);
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &WindowMoveLimits);
			gfx_s32 x_str_location = LcdStruct->sClipRegion.sXMin;
			gfx_s32 y_str_location = LcdStruct->sClipRegion.sYMin;
			if(Caption->textAlign == alignCenter){
				StringProperties_t str_properties = Caption->getStrProp();
				x_str_location = _X_StartBox_ + ((_X_LenBox_>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
				y_str_location = _Y_StartBox_ + ((_Y_LenBox_>>1)-(str_properties.StringColsHeight_Pixels>>1));
			}
			Caption->ulOpaque = false;
			Caption->ulVisible = true;
			Caption->lX = x_str_location;
			Caption->lY = y_str_location;
			Caption->_SelStart = 0;
			Caption->_SelLen = 0;
			Caption->foreground_color = Color.Enabled.Ink.Pull;
			Caption->background_color = Color.Enabled.Buton.Pull;
			Caption->drawString();

		}
		LcdStruct->sClipRegion = back_up_clip;
		//control_comand->Comand = control_comand_comand;
		control_comand->Cursor = cursor;

		Internals.OldStateVisible = Visible;
		Internals.OldStateEnabled = Enabled;
		Internals.Control.Initiated = true;
		Internals.NeedEntireRefresh = false;
		Internals.NeedEntireRepaint = false;
		//control_comand->WindowRefresh |= true;
		return;
	}
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion.sXMin = X_StartBox;
	LcdStruct->sClipRegion.sYMin = Y_StartBox;
	LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
	LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip);
	GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &WindowMoveLimits);
	if(Internals.OldTabGroupMode == false)
	{
		if(Internals.HideHeader == false) {
			Internals.Header.Close->idle(control_comand);
			Internals.Header.MaxMin->idle(control_comand);
			Internals.Header.Minimize->idle(control_comand);
		}
	}
	else
	{
		Internals.Header.TabGroupScrollLeft->idle(control_comand);
		Internals.Header.TabGroupScrollRight->idle(control_comand);
		gfx_s32 HeaderSize = 0;
		if(Internals.HideHeader == false)
			HeaderSize = Internals.Header.Size.Y;
		gfx_u32 header_btn_size = HeaderSize - 6;
		gfx_u32 header_btn_space = HeaderSize - 5;
		bool TabGroupSelectorPositionHasChanged = false;
		if(Internals.Header.TabGroupScrollRight->Events.CursorDown || Internals.Header.TabGroupScrollRight->Events.CursorMove)
		{
			Internals.Header.TabGroupScrollRight->Events.CursorDown = false;
			Internals.Header.TabGroupScrollRight->Events.CursorMove = false;
			gfx_u32 len_of_selection_tab = Internals.Header.Size.TabGroupSelectorSize * Internals.Header.TabGroupTabsListNr;
			if(len_of_selection_tab > Internals.Size.X - (header_btn_space * 2) - 2)
			{
				if(Internals.Header.TabGroupSelectorPosition < len_of_selection_tab - (Internals.Size.X - (header_btn_space * 2)) - 2)
				{
					Internals.Header.TabGroupSelectorPosition++;
					TabGroupSelectorPositionHasChanged = true;
				}
			}
		}
		if(Internals.Header.TabGroupScrollLeft->Events.CursorDown || Internals.Header.TabGroupScrollLeft->Events.CursorMove)
		{
			Internals.Header.TabGroupScrollLeft->Events.CursorDown = false;
			Internals.Header.TabGroupScrollLeft->Events.CursorMove = false;
			gfx_u32 len_of_selection_tab = Internals.Header.Size.TabGroupSelectorSize * Internals.Header.TabGroupTabsListNr;
			if(len_of_selection_tab > Internals.Size.X - header_btn_space - 2)
			{
				if(Internals.Header.TabGroupSelectorPosition > 0)
				{
					Internals.Header.TabGroupSelectorPosition--;
					TabGroupSelectorPositionHasChanged = true;
				}
			}
		}
		gfx_s32 _X_StartBox_, _Y_StartBox_, _X_LenBox_, _Y_LenBox_;;
		_X_StartBox_ = (Internals.Position.X + header_btn_space + 2);
		_X_LenBox_ = (Internals.Size.X + Internals.Position.X) - header_btn_space - 2;
		_Y_StartBox_ = (2 + Internals.Position.Y);
		_Y_LenBox_ = (2 + Internals.Position.Y) + header_btn_size;
		tRectangle back_up_clip_header = LcdStruct->sClipRegion;
		if(TabGroupSelectorPositionHasChanged || Internals.Header.OldTabGroupTabsListNr != Internals.Header.TabGroupTabsListNr)
		{
			LcdStruct->sClipRegion.sXMin = _X_StartBox_;
			LcdStruct->sClipRegion.sXMax = _X_LenBox_;
			LcdStruct->sClipRegion.sYMin = _Y_StartBox_;
			LcdStruct->sClipRegion.sYMax = _Y_LenBox_;
			//GScreen::Util::clipLimit(&pDisplay->sClipRegion, &back_up_clip_header);
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &WindowMoveLimits);
			LcdStruct->drawRectangle(_X_StartBox_, _Y_StartBox_, _X_LenBox_ - _X_StartBox_, _Y_LenBox_ - _Y_StartBox_, false, WindowColor.Enabled.WindowBorder);
			LcdStruct->sClipRegion.sXMin = _X_StartBox_ + 1;
			LcdStruct->sClipRegion.sXMax = _X_LenBox_ - 1;
			LcdStruct->sClipRegion.sYMin = _Y_StartBox_ + 1;
			LcdStruct->sClipRegion.sYMax = _Y_LenBox_ - 1;
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip_header);
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &WindowMoveLimits);
			gfx_u32 TabGroupCnt = 0;
			CursorState cursor = control_comand->Cursor;
			control_comand->Cursor = Cursor_Up;
			while(TabGroupCnt < Internals.Header.TabGroupTabsListNr && Internals.Header.TabGroupTabsList[TabGroupCnt])
			{
				Internals.Header.TabGroupTabsList[TabGroupCnt]->Position.X = ((_X_StartBox_ + 1 + (Internals.Header.Size.TabGroupSelectorSize * TabGroupCnt) - Internals.Header.TabGroupSelectorPosition) - parentWindowHandler->Internals.Position.X);
				Internals.Header.TabGroupTabsList[TabGroupCnt]->Position.Y = (_Y_StartBox_ + 1) - parentWindowHandler->Internals.Position.Y;
				Internals.Header.TabGroupTabsList[TabGroupCnt]->Size.X = Internals.Header.Size.TabGroupSelectorSize;
				Internals.Header.TabGroupTabsList[TabGroupCnt]->Size.Y = _Y_LenBox_ - _Y_StartBox_ - 2;
				if(Internals.Header.OldTabGroupTabsListNr != Internals.Header.TabGroupTabsListNr) Internals.Header.TabGroupTabsList[TabGroupCnt]->Internals.NeedEntireRefresh = true;
				Internals.Header.TabGroupTabsList[TabGroupCnt]->idle(control_comand);
				TabGroupCnt++;
			}
			control_comand->Cursor = cursor;
			Internals.Header.OldTabGroupTabsListNr = Internals.Header.TabGroupTabsListNr;
			//pDisplay->sClipRegion = back_up_clip_header;
		}
		else
		{
			LcdStruct->sClipRegion.sXMin = _X_StartBox_ + 1;
			LcdStruct->sClipRegion.sXMax = _X_LenBox_ - 1;
			LcdStruct->sClipRegion.sYMin = _Y_StartBox_ + 1;
			LcdStruct->sClipRegion.sYMax = _Y_LenBox_ - 1;
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &back_up_clip_header);
			GI::Screen::Util::clipLimit(&LcdStruct->sClipRegion, &WindowMoveLimits);
			gfx_u32 TabGroupCnt = 0;
			while(TabGroupCnt < Internals.Header.TabGroupTabsListNr && Internals.Header.TabGroupTabsList[TabGroupCnt])
			{
				Internals.Header.TabGroupTabsList[TabGroupCnt]->idle(control_comand);
				if(Internals.Header.TabGroupTabsList[TabGroupCnt]->Events.Checked)
				{
					Internals.Header.TabGroupTabsList[TabGroupCnt]->Events.Checked = false;
					//settings->Internals.TabChanged = true;
					Internals.NeedEntireRefresh = true;
					SelectedTab = TabGroupCnt;
					gfx_u32 TabGroupCntUncheck = 0;
					for(; TabGroupCntUncheck < Internals.Header.TabGroupTabsListNr; TabGroupCntUncheck++)
					{
						if(TabGroupCntUncheck != TabGroupCnt)
							Internals.Header.TabGroupTabsList[TabGroupCntUncheck]->Cheched = false;
					}
				}
				TabGroupCnt++;
			}
			LcdStruct->sClipRegion = back_up_clip_header;
		}
	}

	bool full_screen_has_changed_state = false;
	if(Internals.Header.MaxMin->Events.Checked)
	{
		Internals.Header.MaxMin->Events.Checked = false;
		Size.X = SizeNormalScreen.X;
		Size.Y = SizeNormalScreen.Y;
		Position.X = Internals.NormalScreenPosition.X;
		Position.Y = Internals.NormalScreenPosition.Y;
		Internals.FullScreen = false;
		Internals.Position.ChildrenPosition_X = 0;
		Internals.Position.ChildrenPosition_Y = 0;
		parentWindowHandler->Internals.Position.ChildrenPosition_X = 0;
		parentWindowHandler->Internals.Position.ChildrenPosition_Y = 0;
		//settings->Internals.H_ScrollBar->Value = 0;
		//settings->Internals.V_ScrollBar->Value = 0;
		full_screen_has_changed_state = true;
		parentWindowHandler->Internals.NeedEntireRefresh = true;
		//if(parentWindowHandler != NULL && settings != parentWindowHandler)
		//{
		//	parentWindowHandler->Internals.NeedEntireRefresh = true;
		//}
		//ChildrenWindowSize_t _ChildrenWindowSize;
		//window_set_children_settings(settings, true, true, control_comand, true, &_ChildrenWindowSize);
	}
	else if(Internals.Header.MaxMin->Events.UnChecked)
	{
		Internals.Header.MaxMin->Events.UnChecked = false;
		Internals.NormalScreenPosition.X = Position.X;
		Internals.NormalScreenPosition.Y = Position.Y;
		Position.X = Internals.WindowMoveLimits.sXMin;
		Position.Y = Internals.WindowMoveLimits.sYMin;
		Internals.Position.ChildrenPosition_X = 0;
		Internals.Position.ChildrenPosition_Y = 0;
		Size.X = WindowMoveLimits.sXMax - Internals.WindowMoveLimits.sXMin;
		Size.Y = WindowMoveLimits.sYMax - Internals.WindowMoveLimits.sYMin;
		Internals.FullScreen = true;
		full_screen_has_changed_state = true;
	}

	Internals.H_ScrollBar->idle(control_comand);
	Internals.V_ScrollBar->idle(control_comand);
	if(GI::Screen::Util::insideBox(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y) && control_comand->Cursor == Cursor_Down)
		Internals.CursorDownInsideBox = true;
	bool _back_coordonate_used = control_comand->CursorCoordonateUsed;
	if(Internals.FullScreen == false && control_comand->CursorCoordonateUsed == false)
	{
		int Resize_Position_X = (Internals.Position.X + Internals.Size.X) - (Internals.Size.ScrollBarSize);
		int Resize_Position_Y = (Internals.Position.Y + Internals.Size.Y) - (Internals.Size.ScrollBarSize);
		int Resize_Size_X = Internals.Size.ScrollBarSize - 4;
		int Resize_Size_Y = Internals.Size.ScrollBarSize - 4;

		if(GI::Screen::Util::insideBox(Resize_Position_X, Resize_Position_Y, Resize_Size_X, Resize_Size_Y, control_comand->X, control_comand->Y) && control_comand->Cursor == Cursor_Down && control_comand->CursorCoordonateUsed == false)
			control_comand->CursorCoordonateUsed = true;
	}
	ChildrenWindowSize_t ChildrenWindowSize;
	setChildrens(true, false, control_comand, false, &ChildrenWindowSize);
	control_comand->CursorCoordonateUsed = _back_coordonate_used;
	if(Internals.ChildrenWindowSize.X != ChildrenWindowSize.X || Internals.ChildrenWindowSize.Y != ChildrenWindowSize.Y)
	{
		gfx_s32 HeaderSize = 0;
		if(Internals.HideHeader == false)
			HeaderSize = Internals.Header.Size.Y;
		Internals.ChildrenWindowSize.X = ChildrenWindowSize.X;
		Internals.ChildrenWindowSize.Y = ChildrenWindowSize.Y;
		Internals.H_ScrollBar->Maximum = ChildrenWindowSize.X - (Size.X - 8 - Internals.Size.ScrollBarSize);
		Internals.V_ScrollBar->Maximum = ChildrenWindowSize.Y - (Size.Y - 6 - Internals.Size.ScrollBarSize - HeaderSize);
		if((Internals.V_ScrollBar->Maximum > 0 && HideVScroll == false/* || settings->ShowVScroll == true*/)/* && settings->AllowVScroll*/)
		{
			Internals.V_ScrollBar->Size.X = Internals.Size.ScrollBarSize;
		}
		else
		{
			Internals.V_ScrollBar->Maximum = 0;
			Internals.V_ScrollBar->Value = 0;
			Internals.V_ScrollBar->Size.X = 0;
		}
		if((Internals.H_ScrollBar->Maximum > 0 && HideHScroll == false /*|| settings->ShowHScroll == true*/)/* && settings->AllowHScroll*/)
		{
			Internals.H_ScrollBar->Size.Y = Internals.Size.ScrollBarSize;
		}
		else
		{
			Internals.H_ScrollBar->Maximum = 0;
			Internals.H_ScrollBar->Value = 0;
			Internals.H_ScrollBar->Size.Y = 0;
		}

		Internals.V_ScrollBar->Size.Y = Internals.Size.Y - 2 - Internals.Size.ScrollBarSize - HeaderSize;
		Internals.H_ScrollBar->Size.X = Internals.Size.X - 5 - Internals.Size.ScrollBarSize;


		Internals.V_ScrollBar->Internals.NoPaintBackGround = false;
		Internals.V_ScrollBar->idle(control_comand);
		Internals.V_ScrollBar->Internals.NoPaintBackGround = true;


		Internals.H_ScrollBar->Internals.NoPaintBackGround = false;
		Internals.H_ScrollBar->idle(control_comand);
		Internals.H_ScrollBar->Internals.NoPaintBackGround = true;
		if(Internals.V_ScrollBar->Size.X == 0 && Internals.V_ScrollBar->Value != 0)
		{
			Internals.V_ScrollBar->Events.ValueChanged = true;
			Internals.V_ScrollBar->Value = 0;
		}

		if(Internals.H_ScrollBar->Size.Y == 0 && Internals.H_ScrollBar->Value != 0)
		{
			Internals.H_ScrollBar->Events.ValueChanged = true;
			Internals.H_ScrollBar->Value = 0;
		}
	}
	if(Internals.H_ScrollBar->Events.ValueChanged || Internals.V_ScrollBar->Events.ValueChanged)
	{
		Internals.H_ScrollBar->Events.ValueChanged = false;
		Internals.V_ScrollBar->Events.ValueChanged = false;
		gfx_s32 HeaderSize = 0;
		if(Internals.HideHeader == false)
			HeaderSize = Internals.Header.Size.Y;
		LcdStruct->drawRectangle(X_StartBox + 1, (Y_StartBox + HeaderSize) - 1, X_LenBox - 3 - Internals.V_ScrollBar->Size.X, Y_LenBox - Internals.H_ScrollBar->Size.Y - 1 - HeaderSize, true, /*controlls_change_color(*/controls_color.Scren/*, 1.2)*/);
		LcdStruct->cacheClean(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
		Internals.Position.ChildrenPosition_X = -Internals.H_ScrollBar->Value;
		Internals.Position.ChildrenPosition_Y = -Internals.V_ScrollBar->Value;
		CursorState cursor = control_comand->Cursor;
		control_comand->Cursor = Cursor_Up;
		setChildrens(true, true, control_comand, true, &ChildrenWindowSize);
		control_comand->Cursor = cursor;
		control_comand->CursorCoordonateUsed = true;
	}
	LcdStruct->sClipRegion = back_up_clip;

	if(full_screen_has_changed_state)
		return;

	if(Internals.FullScreen == false && control_comand->CursorCoordonateUsed == false && Internals.OldTabGroupMode == false)
	{
		int Resize_Position_X = (Internals.Position.X + Internals.Size.X) - (Internals.Size.ScrollBarSize);
		int Resize_Position_Y = (Internals.Position.Y + Internals.Size.Y) - (Internals.Size.ScrollBarSize);
		int Resize_Size_X = Internals.Size.ScrollBarSize - 4;
		int Resize_Size_Y = Internals.Size.ScrollBarSize - 4;

		if(GI::Screen::Util::insideBox(Resize_Position_X, Resize_Position_Y, Resize_Size_X, Resize_Size_Y, control_comand->X, control_comand->Y) && control_comand->Cursor == Cursor_Down && control_comand->CursorCoordonateUsed == false)
			Internals.CursorDownOnResizeBtn = true;
		/* If is in sizeable window, from here begin to calculate the resize of window using touchscreen */
		if(Internals.CursorDownOnResizeBtn == true)
		{
			if(control_comand->Cursor == Cursor_Down)
			{
				Internals.WindowResizeTouchDownPointX = control_comand->X;
				Internals.WindowResizeTouchDownPointY = control_comand->Y;
				Internals.WindowResizeTouchDownSizeX = Size.X;
				Internals.WindowResizeTouchDownSizeY = Size.Y;
				control_comand->CursorCoordonateUsed = true;
			}
			else if(control_comand->Cursor == Cursor_Move)
			{
				Size.X = Internals.WindowResizeTouchDownSizeX + (control_comand->X - Internals.WindowResizeTouchDownPointX);
				Size.Y = Internals.WindowResizeTouchDownSizeY + (control_comand->Y - Internals.WindowResizeTouchDownPointY);
				if(Size.X < 160)
					Size.X = 160;
				if(Size.Y < 120)
					Size.Y = 120;
				SizeNormalScreen.X = Size.X;
				SizeNormalScreen.Y = Size.Y;
				control_comand->CursorCoordonateUsed = true;
				if(parentWindowHandler != NULL && this != parentWindowHandler)
				{
					parentWindowHandler->Internals.NeedEntireRefresh = true;
				}
			}
		}

		gfx_s32 HeaderSize = 0;
		if(Internals.HideHeader == false)
			HeaderSize = Internals.Header.Size.Y;
		if(GI::Screen::Util::insideBox(X_StartBox + 1, Y_StartBox + 1, X_LenBox - 2, HeaderSize - 2, control_comand->X, control_comand->Y) && control_comand->Cursor == Cursor_Down && control_comand->CursorCoordonateUsed == false)
			Internals.CursorDownOnHeader = true;
		/* If is in sizeable window, from here begin to calculate the move of window using touchscreen */

		if(Internals.CursorDownOnHeader == true)
		{
			if(control_comand->Cursor == Cursor_Down)
			{
				Internals.CursorDownOnHeader = true;
				Internals.HeaderTouchDownPointX = control_comand->X;
				Internals.HeaderTouchDownPointY = control_comand->Y;
				Internals.WindowTouchDownPointX = Position.X;
				Internals.WindowTouchDownPointY = Position.Y;
				control_comand->CursorCoordonateUsed = true;
			}
			else if(control_comand->Cursor == Cursor_Move)
			{
				Position.X = Internals.WindowTouchDownPointX + (control_comand->X - Internals.HeaderTouchDownPointX);
				Position.Y = Internals.WindowTouchDownPointY + (control_comand->Y - Internals.HeaderTouchDownPointY);
				if(Position.Y < WindowMoveLimits.sYMin)
					Position.Y = WindowMoveLimits.sYMin;
				if(Position.Y + HeaderSize >= WindowMoveLimits.sYMax)
					Position.Y = WindowMoveLimits.sYMax - HeaderSize;
				control_comand->CursorCoordonateUsed = true;
				if(parentWindowHandler != NULL && this != parentWindowHandler)
				{
					parentWindowHandler->Internals.NeedEntireRefresh = true;
				}
			}
		}
	}
	if(control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction)
	{
		Internals.CursorDownOnHeader = false;
		Internals.CursorDownOnResizeBtn = false;
		Internals.CursorDownInsideBox = false;
	}
	if(control_comand->Cursor != Cursor_NoAction && Internals.CursorDownInsideBox == true)
		control_comand->CursorCoordonateUsed |= true;
}

void GI::Screen::Gfx::Window::idle()
{
	tControlCommandData control_comand;
	memset(&control_comand, 0, sizeof(tControlCommandData));
	idle(&control_comand);
}

GI::Screen::Gfx::Button *GI::Screen::Gfx::Window::addButton(char *name)
{
	return (GI::Screen::Gfx::Button *)addChildren(WindowButtonChildren, name);
}

GI::Screen::Gfx::CheckBox *GI::Screen::Gfx::Window::addCheckBox(char *name)
{
	return (GI::Screen::Gfx::CheckBox *)addChildren(WindowCheckboxChildren, name);
}

GI::Screen::Gfx::ListBox *GI::Screen::Gfx::Window::addListBox(char *name)
{
	return (GI::Screen::Gfx::ListBox *)addChildren(WindowListboxChildren, name);
}

GI::Screen::Gfx::Picturebox *GI::Screen::Gfx::Window::addPictureBox(char *name)
{
	return (GI::Screen::Gfx::Picturebox *)addChildren(WindowPictureboxChildren, name);
}

GI::Screen::Gfx::TextBox *GI::Screen::Gfx::Window::addTextBox(char *name)
{
	return (GI::Screen::Gfx::TextBox *)addChildren(WindowTextboxChildren, name);
}

GI::Screen::Gfx::ProgressBar *GI::Screen::Gfx::Window::addProgressBar(char *name)
{
	return (GI::Screen::Gfx::ProgressBar *)addChildren(WindowProgressbarChildren, name);
}

GI::Screen::Gfx::KbdQwerty *GI::Screen::Gfx::Window::addKbdQwerty(char *name)
{
	return (GI::Screen::Gfx::KbdQwerty *)addChildren(WindowKbdQwertyChildren, name);
}

GI::Screen::Gfx::Window *GI::Screen::Gfx::Window::addWindow(char *name)
{
	return (GI::Screen::Gfx::Window *)addChildren(WindowWindowChildren, name);
}

GI::Screen::Gfx::Window *GI::Screen::Gfx::Window::addTabGroup(char *name)
{
	return (GI::Screen::Gfx::Window *)addChildren(WindowTabGroupChildren, name);
}





GI::Screen::Gfx::Button *GI::Screen::Gfx::Window::addButton(char *name, gfx_u32 tabIndex)
{
	return (GI::Screen::Gfx::Button *)addChildren(WindowButtonChildren, name, tabIndex);
}

GI::Screen::Gfx::CheckBox *GI::Screen::Gfx::Window::addCheckBox(char *name, gfx_u32 tabIndex)
{
	return (GI::Screen::Gfx::CheckBox *)addChildren(WindowCheckboxChildren, name, tabIndex);
}

GI::Screen::Gfx::ListBox *GI::Screen::Gfx::Window::addListBox(char *name, gfx_u32 tabIndex)
{
	return (GI::Screen::Gfx::ListBox *)addChildren(WindowListboxChildren, name, tabIndex);
}

GI::Screen::Gfx::Picturebox *GI::Screen::Gfx::Window::addPictureBox(char *name, gfx_u32 tabIndex)
{
	return (GI::Screen::Gfx::Picturebox *)addChildren(WindowPictureboxChildren, name, tabIndex);
}

GI::Screen::Gfx::TextBox *GI::Screen::Gfx::Window::addTextBox(char *name, gfx_u32 tabIndex)
{
	return (GI::Screen::Gfx::TextBox *)addChildren(WindowTextboxChildren, name, tabIndex);
}

GI::Screen::Gfx::ProgressBar *GI::Screen::Gfx::Window::addProgressBar(char *name, gfx_u32 tabIndex)
{
	return (GI::Screen::Gfx::ProgressBar *)addChildren(WindowProgressbarChildren, name, tabIndex);
}

GI::Screen::Gfx::KbdQwerty *GI::Screen::Gfx::Window::addKbdQwerty(char *name, gfx_u32 tabIndex)
{
	return (GI::Screen::Gfx::KbdQwerty *)addChildren(WindowKbdQwertyChildren, name, tabIndex);
}

GI::Screen::Gfx::Window *GI::Screen::Gfx::Window::addWindow(char *name, gfx_u32 tabIndex)
{
	return (GI::Screen::Gfx::Window *)addChildren(WindowWindowChildren, name, tabIndex);
}

GI::Screen::Gfx::Window *GI::Screen::Gfx::Window::addTabGroup(char *name, gfx_u32 tabIndex)
{
	return (GI::Screen::Gfx::Window *)addChildren(WindowTabGroupChildren, name, tabIndex);
}

