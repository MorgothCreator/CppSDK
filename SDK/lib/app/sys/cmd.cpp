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
	inPath->~IO();
	outPath->~IO();
	errPath->~IO();
	path->~String();
	input->~String();
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
		else if(tmp_term_char != 0x0D)
		{
			input->append((char) tmp_term_char);
			outPath->write((char)tmp_term_char);
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


			unsigned int cnt = 0;
			for(; cnt < char_string->itemsCount; cnt++)
			{
				outPath->write((unsigned char *)char_string->array[cnt]);
				outPath->write((unsigned char *)"\n\r");
			}
			result->~StringArray();
			char_string->~StringCharArray();
			input->clear();
			outPath->write((unsigned char *)"\n\r");
		}
	}
	return SYS_ERR_OK;
}

