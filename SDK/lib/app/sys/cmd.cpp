/*
 * cmd.cpp
 *
 *  Created on: Jan 29, 2017
 *      Author: John Smith
 */

#include "cmd.h"
#include <app/sys/util.h>

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
}

Cmd::~Cmd()
{
	delete inPath;
	delete outPath;
	delete errPath;
	delete path;
	delete input;
}

SysErr Cmd::idle()
{
	unsigned long tmp_term_char = 0;
	while(inPath->read(&tmp_term_char) == SYS_ERR_OK)
	{
		if(tmp_term_char == 0x7F)
		{
			int str_len;
			for(str_len = 0; str_len < input->length; str_len++)
			{
				outPath->write((char)0x7F);
			}
			input->append((char) tmp_term_char);
			outPath->write((unsigned char *)input->buff);
		}
		else if(tmp_term_char ==27)
		{
			escape_received = true;
			return SYS_ERR_OK;
		}
		else if(escape_received && tmp_term_char ==91)
		{
			escape_second_char = tmp_term_char;
			return SYS_ERR_OK;
		}
		else if(escape_received && escape_second_char == 91)
		{
			int cnt = 0;
			for(; cnt < input->length; cnt++)
				outPath->write((char)0x7F);
			if(tmp_term_char == 65)
			{
				hystoryUp(input);
				outPath->write((unsigned char *)input->buff);
			}
			else if(tmp_term_char == 66)
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
		else if(tmp_term_char != 0x0D)
		{
			input->append((char) tmp_term_char);
			outPath->write((char)tmp_term_char);
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
			if(!strcmp(result->array[0]->buff, "ls"))
			{
				ls(char_string->itemsCount, (char **)char_string->array);
			}
			else
			if(!strcmp(result->array[0]->buff, "cd"))
			{
				cd(char_string->itemsCount, (char **)char_string->array);
			}
			else
			if(!strcmp(result->array[0]->buff, "cat"))
			{
				cat(char_string->itemsCount, (char **)char_string->array);
			}
			hystoryAdd(input);
			outPath->write((unsigned char *)"GI@");
			outPath->write((unsigned char *)path->buff);
			outPath->write((unsigned char *)": ");
			delete result;
			delete char_string;
			input->clear();
			escape_received = false;
			escape_second_char = 0;
		}
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
