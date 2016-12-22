/*
 * PasswordWindowNumeric.h
 *
 *  Created on: Sep 18, 2016
 *      Author: John Smith
 */


#ifndef PASSWORDWINDOWNUMERIC_H_
#define PASSWORDWINDOWNUMERIC_H_

#include "string.h"
#include "window.h"
#include "button.h"
#include "string.h"

namespace GI
{
namespace Screen
{
namespace Gfx
{
class PasswordWindowNumeric {
public:
	PasswordWindowNumeric();
	~PasswordWindowNumeric();
	bool idle(tControlCommandData* control_comand);
	bool idle();
	struct {
		GI::Screen::Gfx::Button *textBox;
		GI::Screen::Gfx::Button *Btn1;
		GI::Screen::Gfx::Button *Btn2;
		GI::Screen::Gfx::Button *Btn3;
		GI::Screen::Gfx::Button *Btn4;
		GI::Screen::Gfx::Button *Btn5;
		GI::Screen::Gfx::Button *Btn6;
		GI::Screen::Gfx::Button *Btn7;
		GI::Screen::Gfx::Button *Btn8;
		GI::Screen::Gfx::Button *Btn9;
		GI::Screen::Gfx::Button *BtnBack;
		GI::Screen::Gfx::Button *Btn0;
		GI::Screen::Gfx::Button *BtnOk;
		GI::Screen::Gfx::Window *parentWindowHandler;
		GI::Screen::Gfx::Window *windowHandler;
		bool afterWrongPassword;
		bool textRefrashed;
	}internals;
	//GI::Screen::Gfx::Window *Window;
	int minChars;
	int maxChars;
	GI::Screen::String* text;
};
}
}
}

#define newWindowPasswordNumeric(_parentWindowHandler, windowHandlerName, btn_space_x, btn_space_y)\
		GI::Screen::Gfx::PasswordWindowNumeric *windowHandlerName = new GI::Screen::Gfx::PasswordWindowNumeric();\
		newWindow(_parentWindowHandler, windowHandlerName##_window);\
		windowHandlerName##_window->Position.X = 0;\
		windowHandlerName##_window->Position.Y = 0;\
		windowHandlerName##_window->Size.X = _parentWindowHandler->Size.X - 5;\
		windowHandlerName##_window->Size.Y = windowHandlerName##_window->Size.Y - 5 - windowHandlerName##_window->Internals.Header.Size.Y;\
		windowHandlerName##_window->Internals.FullScreen = true;\
		windowHandlerName##_window->AllowHScroll = false;\
		windowHandlerName##_window->AllowVScroll = false;\
		windowHandlerName##_window->HideHScroll = true;\
		windowHandlerName##_window->HideVScroll = true;\
		windowHandlerName##_window->MaxMinEnabled = false;\
		windowHandlerName##_window->MinimizeButonEnabled = false;\
		windowHandlerName##_window->CloseButonEnabled = false;\
		windowHandlerName##_window->Caption->textAlign = alignCenter;\
		windowHandlerName##_window->Caption->setText((char *)"Numeric password");\
		\
		int pass_window_numeric_keys_upleft_X = 5;\
		int pass_window_numeric_keys_upleft_Y = 5;\
		int pass_window_numeric_btn_high = (windowHandlerName##_window->Size.Y - windowHandlerName##_window->Internals.Header.Size.Y - pass_window_numeric_keys_upleft_Y - (btn_space_y * 6)) / 5;\
		newButton(windowHandlerName##_window, TextBoxPass);\
		TextBoxPass->Position.X = pass_window_numeric_keys_upleft_X;\
		TextBoxPass->Position.Y = pass_window_numeric_keys_upleft_Y;\
		TextBoxPass->Size.X = windowHandlerName##_window->Size.X - (pass_window_numeric_keys_upleft_X * 2) - btn_space_x;\
		TextBoxPass->Size.Y = pass_window_numeric_btn_high;\
		TextBoxPass->Enabled = false;\
		TextBoxPass->Caption->setText((char *)"");\
		\
		pass_window_numeric_keys_upleft_Y += TextBoxPass->Size.Y + btn_space_y;\
		int pass_window_numeric_btn_width = (windowHandlerName##_window->Size.X - pass_window_numeric_keys_upleft_X - (btn_space_x * 4)) / 3;\
		\
		newButton(windowHandlerName##_window, WinPassNumBtn1);\
		WinPassNumBtn1->Position.X = pass_window_numeric_keys_upleft_X;\
		WinPassNumBtn1->Position.Y = pass_window_numeric_keys_upleft_Y;\
		WinPassNumBtn1->Size.X = pass_window_numeric_btn_width;\
		WinPassNumBtn1->Size.Y = pass_window_numeric_btn_high;\
		WinPassNumBtn1->Caption->setText((char *)"1");\
		newButton(windowHandlerName##_window, WinPassNumBtn2);\
		WinPassNumBtn2->Position.X = pass_window_numeric_keys_upleft_X + btn_space_x + pass_window_numeric_btn_width;\
		WinPassNumBtn2->Position.Y = pass_window_numeric_keys_upleft_Y;\
		WinPassNumBtn2->Size.X = pass_window_numeric_btn_width;\
		WinPassNumBtn2->Size.Y = pass_window_numeric_btn_high;\
		WinPassNumBtn2->Caption->setText((char *)"2");\
		newButton(windowHandlerName##_window, WinPassNumBtn3);\
		WinPassNumBtn3->Position.X = pass_window_numeric_keys_upleft_X + ((btn_space_x + pass_window_numeric_btn_width) * 2);\
		WinPassNumBtn3->Position.Y = pass_window_numeric_keys_upleft_Y;\
		WinPassNumBtn3->Size.X = pass_window_numeric_btn_width;\
		WinPassNumBtn3->Size.Y = pass_window_numeric_btn_high;\
		WinPassNumBtn3->Caption->setText((char *)"3");\
		\
		newButton(windowHandlerName##_window, WinPassNumBtn4);\
		WinPassNumBtn4->Position.X = pass_window_numeric_keys_upleft_X;\
		WinPassNumBtn4->Position.Y = pass_window_numeric_keys_upleft_Y + btn_space_y + pass_window_numeric_btn_high;\
		WinPassNumBtn4->Size.X = pass_window_numeric_btn_width;\
		WinPassNumBtn4->Size.Y = pass_window_numeric_btn_high;\
		WinPassNumBtn4->Caption->setText((char *)"4");\
		newButton(windowHandlerName##_window, WinPassNumBtn5);\
		WinPassNumBtn5->Position.X = pass_window_numeric_keys_upleft_X + btn_space_x + pass_window_numeric_btn_width;\
		WinPassNumBtn5->Position.Y = pass_window_numeric_keys_upleft_Y + btn_space_y + pass_window_numeric_btn_high;\
		WinPassNumBtn5->Size.X = pass_window_numeric_btn_width;\
		WinPassNumBtn5->Size.Y = pass_window_numeric_btn_high;\
		WinPassNumBtn5->Caption->setText((char *)"5");\
		newButton(windowHandlerName##_window, WinPassNumBtn6);\
		WinPassNumBtn6->Position.X = pass_window_numeric_keys_upleft_X + ((btn_space_x + pass_window_numeric_btn_width) * 2);\
		WinPassNumBtn6->Position.Y = pass_window_numeric_keys_upleft_Y + btn_space_y + pass_window_numeric_btn_high;\
		WinPassNumBtn6->Size.X = pass_window_numeric_btn_width;\
		WinPassNumBtn6->Size.Y = pass_window_numeric_btn_high;\
		WinPassNumBtn6->Caption->setText((char *)"6");\
		\
		newButton(windowHandlerName##_window, WinPassNumBtn7);\
		WinPassNumBtn7->Position.X = pass_window_numeric_keys_upleft_X;\
		WinPassNumBtn7->Position.Y = pass_window_numeric_keys_upleft_Y + ((btn_space_y + pass_window_numeric_btn_high) * 2);\
		WinPassNumBtn7->Size.X = pass_window_numeric_btn_width;\
		WinPassNumBtn7->Size.Y = pass_window_numeric_btn_high;\
		WinPassNumBtn7->Caption->setText((char *)"7");\
		newButton(windowHandlerName##_window, WinPassNumBtn8);\
		WinPassNumBtn8->Position.X = pass_window_numeric_keys_upleft_X + btn_space_x + pass_window_numeric_btn_width;\
		WinPassNumBtn8->Position.Y = pass_window_numeric_keys_upleft_Y + ((btn_space_y + pass_window_numeric_btn_high) * 2);\
		WinPassNumBtn8->Size.X = pass_window_numeric_btn_width;\
		WinPassNumBtn8->Size.Y = pass_window_numeric_btn_high;\
		WinPassNumBtn8->Caption->setText((char *)"8");\
		newButton(windowHandlerName##_window, WinPassNumBtn9);\
		WinPassNumBtn9->Position.X = pass_window_numeric_keys_upleft_X + ((btn_space_x + pass_window_numeric_btn_width) * 2);\
		WinPassNumBtn9->Position.Y = pass_window_numeric_keys_upleft_Y + ((btn_space_y + pass_window_numeric_btn_high) * 2);\
		WinPassNumBtn9->Size.X = pass_window_numeric_btn_width;\
		WinPassNumBtn9->Size.Y = pass_window_numeric_btn_high;\
		WinPassNumBtn9->Caption->setText((char *)"9");\
		\
		newButton(windowHandlerName##_window, WinPassNumBtnBack);\
		WinPassNumBtnBack->Position.X = pass_window_numeric_keys_upleft_X;\
		WinPassNumBtnBack->Position.Y = pass_window_numeric_keys_upleft_Y + ((btn_space_y + pass_window_numeric_btn_high) * 3);\
		WinPassNumBtnBack->Size.X = pass_window_numeric_btn_width;\
		WinPassNumBtnBack->Size.Y = pass_window_numeric_btn_high;\
		WinPassNumBtnBack->Caption->setText((char *)"Back");\
		newButton(windowHandlerName##_window, WinPassNumBtn0);\
		WinPassNumBtn0->Position.X = pass_window_numeric_keys_upleft_X + btn_space_x + pass_window_numeric_btn_width;\
		WinPassNumBtn0->Position.Y = pass_window_numeric_keys_upleft_Y + ((btn_space_y + pass_window_numeric_btn_high) * 3);\
		WinPassNumBtn0->Size.X = pass_window_numeric_btn_width;\
		WinPassNumBtn0->Size.Y = pass_window_numeric_btn_high;\
		WinPassNumBtn0->Caption->setText((char *)"0");\
		newButton(windowHandlerName##_window, WinPassNumBtnOk);\
		WinPassNumBtnOk->Position.X = pass_window_numeric_keys_upleft_X + ((btn_space_x + pass_window_numeric_btn_width) * 2);\
		WinPassNumBtnOk->Position.Y = pass_window_numeric_keys_upleft_Y + ((btn_space_y + pass_window_numeric_btn_high) * 3);\
		WinPassNumBtnOk->Size.X = pass_window_numeric_btn_width;\
		WinPassNumBtnOk->Size.Y = pass_window_numeric_btn_high;\
		WinPassNumBtnOk->Caption->setText((char *)"Ok");\
		\
		windowHandlerName->internals.textBox = TextBoxPass;\
		windowHandlerName->internals.Btn1 = WinPassNumBtn1;\
		windowHandlerName->internals.Btn2 = WinPassNumBtn2;\
		windowHandlerName->internals.Btn3 = WinPassNumBtn3;\
		windowHandlerName->internals.Btn4 = WinPassNumBtn4;\
		windowHandlerName->internals.Btn5 = WinPassNumBtn5;\
		windowHandlerName->internals.Btn6 = WinPassNumBtn6;\
		windowHandlerName->internals.Btn7 = WinPassNumBtn7;\
		windowHandlerName->internals.Btn8 = WinPassNumBtn8;\
		windowHandlerName->internals.Btn9 = WinPassNumBtn9;\
		windowHandlerName->internals.BtnBack = WinPassNumBtnBack;\
		windowHandlerName->internals.Btn0 = WinPassNumBtn0;\
		windowHandlerName->internals.BtnOk = WinPassNumBtnOk;\
		windowHandlerName->internals.parentWindowHandler = _parentWindowHandler;\
		windowHandlerName->internals.windowHandler = windowHandlerName##_window;\
		windowHandlerName->minChars = 4;\
		windowHandlerName->maxChars = 16;\
		windowHandlerName->text = new GI::Screen::String(_parentWindowHandler->Internals.pDisplay);\
		windowHandlerName->text->setText((char *)"");


#endif /* PASSWORDWINDOWNUMERIC_H_ */
