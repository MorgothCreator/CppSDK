/*
 * PasswordWindowNumeric.c
 *
 *  Created on: Sep 18, 2016
 *      Author: John Smith
 */

#include <stdlib.h>
#include "PasswordWindowNumeric.h"

/******************************************************************************/
/*
 * Example of use:
 *
 * Construct the window
 *
 * MainWindow = parent window.
 * PasswordWindowNumeric = name of structure to maintain all ressources for this window.
 * 3 = Vertical space between numeric buttons.
 * 3 = Horizontal space between numeric buttons.
 *     new_password_window_numeric(MainWindow, PasswordWindowNumeric, 3, 3);
 *
 */
/******************************************************************************/
bool GI::Screen::Gfx::PasswordWindowNumeric::idle()
{
	if(internals.windowHandler->Visible == false)
		return false;
	//if(PassWindow->after_wrong_password == true && PassWindow->Text->modifyed)
	if(strcmp(internals.textBox->Caption->buff, text->buff) && internals.afterWrongPassword == true)
		internals.textBox->Caption->setText(text->buff);
	char character = 0;
	if(internals.Btn1->Events.CursorUp)
	{
		internals.Btn1->Events.CursorUp = false;
		character = '1';
	}
	else if(internals.Btn2->Events.CursorUp)
	{
		internals.Btn2->Events.CursorUp = false;
		character = '2';
	}
	else if(internals.Btn3->Events.CursorUp)
	{
		internals.Btn3->Events.CursorUp = false;
		character = '3';
	}
	else if(internals.Btn4->Events.CursorUp)
	{
		internals.Btn4->Events.CursorUp = false;
		character = '4';
	}
	else if(internals.Btn5->Events.CursorUp)
	{
		internals.Btn5->Events.CursorUp = false;
		character = '5';
	}
	else if(internals.Btn6->Events.CursorUp)
	{
		internals.Btn6->Events.CursorUp = false;
		character = '6';
	}
	else if(internals.Btn7->Events.CursorUp)
	{
		internals.Btn7->Events.CursorUp = false;
		character = '7';
	}
	else if(internals.Btn8->Events.CursorUp)
	{
		internals.Btn8->Events.CursorUp = false;
		character = '8';
	}
	else if(internals.Btn9->Events.CursorUp)
	{
		internals.Btn9->Events.CursorUp = false;
		character = '9';
	}
	else if(internals.BtnBack->Events.CursorUp || internals.BtnBack->Events.CursorMove)
	{
		internals.BtnBack->Events.CursorUp = false;
		internals.BtnBack->Events.CursorMove = false;
		if(internals.textBox->Caption->length > 0)
		{
			char *tmp = text->subString(0, text->length - 1);
			text->setText(tmp);
			if(tmp)
				free(tmp);
			if(internals.afterWrongPassword == false)
			{
				int cnt = 0;
				internals.textBox->Caption->clear();
				for(; cnt < text->length; cnt++)
				{
					internals.textBox->Caption->append('*');
				}
			}
			else
			{
				internals.textBox->Caption->setText(text->buff);
			}
			return false;
		}
	}
	else if(internals.Btn0->Events.CursorUp)
	{
		internals.Btn0->Events.CursorUp = false;
		character = '0';
	}
	else if(internals.BtnOk->Events.CursorUp)
	{
		internals.BtnOk->Events.CursorUp = false;
		return true;
	}

	if(character != 0)
	{
		if(internals.afterWrongPassword == true)
		{
			text->clear();
		}
		internals.afterWrongPassword = false;
		text->append(character);
		int cnt = 0;
		internals.textBox->Caption->clear();
		for(; cnt < text->length; cnt++)
		{
			internals.textBox->Caption->append('*');
		}
	}
	return false;
}

GI::Screen::Gfx::PasswordWindowNumeric::PasswordWindowNumeric()
{
	memset(this, 0, sizeof(*this));
}
GI::Screen::Gfx::PasswordWindowNumeric::~PasswordWindowNumeric()
{

}
