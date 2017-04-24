/*
 * app/sys/cmd.cpp
 */

#include "cmd.h"
#include <app/sys/util.h>
#include <include/global.h>
#include <lib/fs/fat.h>

#if __AVR_XMEGA__
#include <avr/pgmspace.h>
#define cmd_to_flash(x, y) const char x[] PROGMEM = {y}

cmd_to_flash(cmd_err0, "Succeeded.");
cmd_to_flash(cmd_err1, "A hard error occurred in the low level disk I/O layer.");
cmd_to_flash(cmd_err2, "Assertion failed.");
cmd_to_flash(cmd_err3, "The physical drive cannot work.");
cmd_to_flash(cmd_err4, "Could not find the file.");
cmd_to_flash(cmd_err5, "Could not find the path.");
cmd_to_flash(cmd_err6, "The path name format is invalid.");
cmd_to_flash(cmd_err7, "Access denied due to prohibited access or directory full.");
cmd_to_flash(cmd_err8, "Access denied due to prohibited access.");
cmd_to_flash(cmd_err9, "The file/directory object is invalid.");
cmd_to_flash(cmd_err10, "The physical drive is write protected.");
cmd_to_flash(cmd_err11, "The logical drive number is invalid.");
cmd_to_flash(cmd_err12, "The volume has no work area.");
cmd_to_flash(cmd_err13, "There is no valid FAT volume.");
cmd_to_flash(cmd_err14, "The f_mkfs() aborted due to any problem.");
cmd_to_flash(cmd_err15, "Could not get a grant to access the volume within defined period.");
cmd_to_flash(cmd_err16, "The operation is rejected according to the file sharing policy.");
cmd_to_flash(cmd_err17, "LFN working buffer could not be allocated.");
cmd_to_flash(cmd_err18, "Number of open files > _FS_LOCK.");
cmd_to_flash(cmd_err19, "Given parameter is invalid.");

PGM_P fs_err_table[] = {
		cmd_err0,
		cmd_err1,
		cmd_err2,
		cmd_err3,
		cmd_err4,
		cmd_err5,
		cmd_err6,
		cmd_err7,
		cmd_err8,
		cmd_err9,
		cmd_err10,
		cmd_err11,
		cmd_err12,
		cmd_err13,
		cmd_err14,
		cmd_err15,
		cmd_err16,
		cmd_err17,
		cmd_err18,
		cmd_err19,
};
#else
const char *fs_err_table[] = {
		"Succeeded.",
		"A hard error occurred in the low level disk I/O layer.",
		"Assertion failed.",
		"The physical drive cannot work.",
		"Could not find the file.",
		"Could not find the path.",
		"The path name format is invalid.",
		"Access denied due to prohibited access or directory full.",
		"Access denied due to prohibited access.",
		"The file/directory object is invalid.",
		"The physical drive is write protected.",
		"The logical drive number is invalid.",
		"The volume has no work area.",
		"There is no valid FAT volume.",
		"The f_mkfs() aborted due to any problem.",
		"Could not get a grant to access the volume within defined period.",
		"The operation is rejected according to the file sharing policy.",
		"LFN working buffer could not be allocated.",
		"Number of open files > _FS_LOCK.",
		"Given parameter is invalid."
};
#endif

/*typedef struct {
	char name[8];
	SysErr (*fnc)(int argc, char *argv[]);
}cmd_list;*/

static GI::StringArray hystoryTable = GI::StringArray();

Cmd::Cmd(char *inPath, char *outPath, char *errPath)
{
	memset(this, 0, sizeof(*this));
	this->inPath = new GI::IO(inPath);
	this->outPath = new GI::IO(outPath);
	this->errPath = new GI::IO(errPath);
	path = new GI::String();
	input = new GI::String();
	outFifo = new GI::Buff::RingBuff(128, 1);
	charPopFromFifo = -1;
}

Cmd::~Cmd()
{
	delete inPath;
	delete outPath;
	delete errPath;
	delete path;
	delete input;
	delete outFifo;
}

SysErr Cmd::idle()
{
	if(charPopFromFifo < 0)
	{
		unsigned char tmp_char = 0;
		if(outFifo->pop(&tmp_char))
			charPopFromFifo = tmp_char;
	}
	else
	{
		if(!outPath->write((char)charPopFromFifo))
			charPopFromFifo = -1;
	}
	unsigned long tmp_term_char = 0;
	while(inPath->read(&tmp_term_char) == SYS_ERR_OK)
	{
		parse(tmp_term_char);
	}
	return SYS_ERR_OK;
}

SysErr Cmd::parse(char data)
{
	if(data == 0x7F)
	{
		unsigned int str_len;
		for(str_len = 0; str_len < input->length; str_len++)
		{
			outPath->write((char)0x7F);
		}
		input->append((char) data);
		outPath->write((unsigned char *)input->buff);
	}
	else if(data ==27)
	{
		escape_received = true;
		return SYS_ERR_OK;
	}
	else if(escape_received && data ==91)
	{
		escape_second_char = data;
		return SYS_ERR_OK;
	}
	else if(escape_received && escape_second_char == 91)
	{
		unsigned int cnt = 0;
		for(; cnt < input->length; cnt++)
			outPath->write((char)0x7F);
		if(data == 65)
		{
			hystoryUp(input);
			outPath->write((unsigned char *)input->buff);
		}
		else if(data == 66)
		{
			//if(hystoryPtr != 0)
			//{
				hystoryDn(input);
				outPath->write((unsigned char *)input->buff);
			//}
		}
		escape_received = false;
		escape_second_char = 0;
		return SYS_ERR_OK;
	}
	else if(data != 0x0D)
	{
		input->append((char) data);
		outFifo->push((unsigned char)data);
		escape_received = false;
		escape_second_char = 0;
	}
	else
	{
		outPath->write((unsigned char *)"\n\r");
		GI::StringArray *result;
		Sys::Util::strToCmd(input, &result);
		//unsigned int cmd_cnt = 0;
		GI::StringCharArray *char_string = new GI::StringCharArray(result, 1);
		if(char_string)
		{
#if __AVR_XMEGA__
			if(!strcmp_P(result->array[0]->buff, PSTR("ls")))
#else
			if(!strcmp(result->array[0]->buff, "ls"))
#endif
			{
				ls(char_string->itemsCount, (char **)char_string->array);
			}
			else
#if __AVR_XMEGA__
			if(!strcmp_P(result->array[0]->buff, PSTR("cd")))
#else
			if(!strcmp(result->array[0]->buff, "cd"))
#endif
			{
				cd(char_string->itemsCount, (char **)char_string->array);
			}
			else
#if __AVR_XMEGA__
			if(!strcmp_P(result->array[0]->buff, PSTR("cat")))
#else
			if(!strcmp(result->array[0]->buff, "cat"))
#endif
			{
				cat(char_string->itemsCount, (char **)char_string->array);
			}
		}
		else
		{
#if __AVR_XMEGA__
			char tmp[sizeof("Out of memory\n\r")];
			strcpy_P(tmp, PSTR("Out of memory\n\r"));
			errPath->write(tmp);
#else
			errPath->write((char *)"Out of memory\n\r");
#endif
		}
		hystoryAdd(input);
#if __AVR_XMEGA__
		char tmp[sizeof("GI@")];
		strcpy_P(tmp, PSTR("GI@"));
		errPath->write(tmp);
#else
		outPath->write((unsigned char *)"\n\rGI@");
#endif
		outPath->write((unsigned char *)path->buff);
		outPath->write((unsigned char *)": ");
		delete result;
		delete char_string;
		input->clear();
		escape_received = false;
		escape_second_char = 0;
	}
	return SYS_ERR_OK;
}

SysErr Cmd::hystoryAdd(GI::String *input)
{
	unsigned int cnt = 0;
	if(hystoryTable.itemsCount)
	{
		for(; cnt < hystoryTable.itemsCount; cnt++)
		{
			if(!strcmp(hystoryTable.array[cnt]->buff, input->buff))
			{
				hystoryTable.remove(cnt);
				break;
			}
		}
		hystoryTable.insert(input, 0);
	}
	else
		hystoryTable.add(input);
	hystoryPtr = 0;
	return SYS_ERR_OK;
}

SysErr Cmd::hystoryUp(GI::String *out)
{
	out->set(hystoryTable.array[hystoryPtr]);
	if(hystoryPtr < hystoryTable.itemsCount - 1)
		hystoryPtr++;
	return SYS_ERR_OK;
}

SysErr Cmd::hystoryDn(GI::String *out)
{
	out->set(hystoryTable.array[hystoryPtr]);
	if(hystoryPtr > 0)
		hystoryPtr--;
	return SYS_ERR_OK;
}

void Cmd::printError(int error)
{
#if __AVR_XMEGA__
	char *tmp;
	if(error <= FR_INVALID_PARAMETER)
	{
		tmp = (char *)malloc(strlen_P(fs_err_table[error]) + 1);
		if(tmp)
			strcpy_P(tmp, fs_err_table[error]);
	}
	else
	{
		tmp = (char *)malloc(sizeof("Unknown error"));
		if(tmp)
			strcpy_P(tmp, PSTR("Unknown error"));
	}
	errPath->write(tmp);
	if(tmp)
		free(tmp);
#else
	if(error <= FR_INVALID_PARAMETER)
		errPath->write((char *)fs_err_table[error]);
	else
		errPath->write((char *)"Unknown error");
#endif
	this->errPath->write((unsigned char *)"\n\r");
}
