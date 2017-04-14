/*
 *  api/std.cpp
 *
 *  Copyright (C) 2016  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of CppSDK.
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

#include "std.h"
#include <api/init.h>
#include <lib/string.h>
#include <lib/gfx/window.h>
#include <api/dev_request.h>
#include <api/uart.h>

GI::Std::Std(char *devPath) :
IoDevices(STDIO_UART),
devHandle(NULL)
{
	if(!strncmp((char *)"uart-", devPath, sizeof("uart-") - 1))
	{
		GI::Dev::Uart *device;
		if(GI::Dev::DevRequest::request(devPath, &device) == SYS_ERR_OK)
		{
			devHandle = (void *)device;
			IoDevices = STDIO_UART;
		}
	}
#if (defined(SCREEN_ENABLE) && SCREEN_INTERFACE_COUNT > 0)
	else if(!strncmp((char *)"SCREEN<", devPath, sizeof("SCREEN<") - 1))
	{
		char* ptr = strchr((const char *)(devPath + sizeof("SCREEN<") - 1), (int)'>');
		if(ptr)
		{
			char tmp_name[32];
			memset(tmp_name, 0, 32);
			strncpy(tmp_name, (const char *)(devPath + sizeof("SCREEN<") - 1), (unsigned int)(ptr - (devPath + sizeof("SCREEN<") - 1)));
			GI::Screen::Gfx::Window *terminalWindow = new GI::Screen::Gfx::Window(NULL, dev.SCREEN[0]);
			terminalWindow->AllowHScroll = false;
			terminalWindow->AllowVScroll = false;
			terminalWindow->HideHScroll = true;
			terminalWindow->HideVScroll = true;
			terminalWindow->MaxMinEnabled = false;
			terminalWindow->Internals.FullScreen = false;
			terminalWindow->SizeNormalScreen.X = 400;
			terminalWindow->SizeNormalScreen.Y = 700;
			terminalWindow->MaxMinVisible = true;
			terminalWindow->MinimizeButonEnabled = true;
			terminalWindow->CloseButonEnabled = true;
			terminalWindow->Caption->textAlign = alignLeft;
			terminalWindow->Caption->setText(tmp_name);

			newTextBox(terminalWindow, terminalScreen);
			terminalScreen->Position.X = 1;
			terminalScreen->Position.Y = 2;
			terminalScreen->Size.X = terminalWindow->Size.X - 6;
			terminalScreen->Size.Y = terminalWindow->Size.Y - terminalWindow->Internals.Header.Size.Y - 6;
			terminalScreen->Color.Enabled.TextBackGround = 0x00000000;
			//terminalScreen->Color.Enabled. = 0x00000000;
			terminalScreen->Color.Enabled.Ink.Pull = 0xFFFFFFFF;
			terminalScreen->Color.Enabled.Ink.Push = 0xFFFFFFFF;

			terminalWindow->idle();
			terminalWindow->idle();
			terminalWindow->idle();
			terminalWindow->idle();
			devHandle = (void *)terminalWindow;
			userData = (void *)terminalScreen;
			IoDevices = STDIO_SCREEN;
		}
	}
#endif
	/*switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		devHandle = (void *)dev.UART[uartNr];
		break;
	case STDIO_SCREEN:
		break;
	case STDIO_FS:
		break;
	}*/
}

GI::Std::~Std()
{
	switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		((GI::Dev::Uart *)devHandle)->~Uart();
		break;
	case STDIO_SCREEN:
		break;
	case STDIO_FS:
		break;
	}
}

void GI::Std::print(char Char)
{
	switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		((GI::Dev::Uart *)devHandle)->putChar(Char);
		break;
	case STDIO_SCREEN:
		//GI::Screen::Gfx::TextBox *terminalScreen = (GI::Screen::Gfx::TextBox *)userData;
		//terminalScreen->text->append(Char);
		break;
	case STDIO_FS:
		break;
	}
}

void GI::Std::print(char* string)
{
	switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		//((GI::Dev::Uart *)devHandle)->print(string);
		break;
	case STDIO_SCREEN:
		//GI::Screen::Gfx::TextBox *terminalScreen = (GI::Screen::Gfx::TextBox *)userData;
		//terminalScreen->text->append(string);
		break;
	case STDIO_FS:
		break;
	}
}

/*void GI::Std::print(GI::Screen::String *string)
{
	switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		//((GI::Dev::Uart *)devHandle)->print(string);
		break;
	case STDIO_SCREEN:
		//GI::Screen::Gfx::TextBox *terminalScreen = (GI::Screen::Gfx::TextBox *)userData;
		//terminalScreen->text->append(string);
		break;
	case STDIO_FS:
		break;
	}
}*/

void GI::Std::print(GI::String *string)
{
	switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		((GI::Dev::Uart *)devHandle)->print(string);
		break;
	case STDIO_SCREEN:
		//GI::Screen::Gfx::TextBox *terminalScreen = (GI::Screen::Gfx::TextBox *)userData;
		//terminalScreen->text->append(string);
		break;
	case STDIO_FS:
		break;
	}
}

void GI::Std::clear()
{
	switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		break;
	case STDIO_SCREEN:
		//GI::Screen::Gfx::TextBox *terminalScreen = (GI::Screen::Gfx::TextBox *)userData;
		//terminalScreen->text->clear();
		break;
	case STDIO_FS:
		break;
	}
}
/*#####################################################*/
void GI::Std::printF(const char *pcString, ...)
{
	va_list args;
	va_start (args, pcString);
	char buff[1];
	s32 len = vsnprintf(buff, 0, pcString, args);
	va_end (args);
	if(len > 0)
	{
		char *tmp_str = (char *)malloc(len + 1);
		if(tmp_str)
		{
			va_start (args, pcString);
			vsnprintf(tmp_str, len, pcString, args);
			va_end (args);
			print(tmp_str);
			free(tmp_str);
		}
	}
}
/*#####################################################*/
void GI::Std::printF(GI::String *string, ...)
{
	va_list args;
	char *pcString = string->buff;
	va_start (args, pcString);
	char buff[1];
	s32 len = vsnprintf(buff, 0, pcString, args);
	va_end (args);
	if(len > 0)
	{
		char *tmp_str = (char *)malloc(len + 1);
		if(tmp_str)
		{
			va_start (args, pcString);
			vsnprintf(tmp_str, len, pcString, args);
			va_end (args);
			print(tmp_str);
			free(tmp_str);
		}
	}
}
/*#####################################################*/
/*void GI::Std::printF(GI::Screen::String *string, ...)
{
	const char *pcString = string->buff;
	VA_LOCAL_DECL;
	VA_START (pcString);
	VA_SHIFT (str, char *);
	VA_SHIFT (count, size_t );
	VA_SHIFT (fmt, char *);
	char buff[1];
	vsnprintf(buff, 65535, (const char *)string->buff, ap);
	VA_END;
}*/
/*#####################################################*/
