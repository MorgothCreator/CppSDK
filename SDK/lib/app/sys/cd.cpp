/*
 * app/sys/cd.cpp
 */
#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#endif
#include <lib/fs/fat.h>
#include "cmd.h"

SysErr Cmd::cd(int argc, char *argv[])
{
	int argc_cnt = 0;
	SysErr err = SYS_ERR_OK;
	GI::String input = GI::String();
	for(; argc_cnt < argc; argc_cnt++)
	{
		input.set(argv[argc_cnt]);
		err = cdInt(path, &input);
	}
	if(err != SYS_ERR_OK)
	{
#if __AVR_XMEGA__
		char tmp[sizeof("Invalid path.\n\r")];
		strcpy_P(tmp, PSTR("Invalid path.\n\r"));
		errPath->write(tmp);
#else
		errPath->write((char *)"Invalid path.\n\r");
#endif
		return SYS_ERR_INVALID_PATH;
	}
	return SYS_ERR_OK;
}

SysErr Cmd::cdInt(GI::String *_path, GI::String *input)
{
	GI::String tmpInput = GI::String();
	if((input->length >=1 && input->buff[0] == '/') || (input->length >=2 && (input->buff[0] == '\\' && input->buff[1] == '\\')))
	{
		char *tmp;
		tmpInput.set((tmp = input->subString(1)));
		if(tmp)
			free(tmp);
		dir tmp_dir = dir();
		if(tmp_dir.fopendir(tmpInput.buff) == FR_OK)
		{
			tmp_dir.fclosedir();
			_path->set(&tmpInput);
		}
		else
			return SYS_ERR_INVALID_PATH;
	}
	else
	if(input->length >=2 && input->buff[0] == '\\' && input->buff[1] == '\\')
	{
		char *tmp;
		tmpInput.set((tmp = input->subString(2)));
		if(tmp)
			free(tmp);
		dir tmp_dir = dir();
		if(tmp_dir.fopendir(tmpInput.buff) == FR_OK)
		{
			tmp_dir.fclosedir();
			_path->set(&tmpInput);
		}
		else
			return SYS_ERR_INVALID_PATH;
	}
	else
	if(input->length >=2 && input->buff[0] == '\\' && input->buff[1] != '\\')
	{
		return SYS_ERR_INVALID_PATH;
	}
	else
	if(input->length >=3 && input->buff[0] == '.' && input->buff[1] == '.' && input->buff[2] == '/')
	{
		if(_path->length)
		{
			int cnt = _path->length - 1;
			while(cnt != -1)
			{
				if(_path->buff[cnt] == '/')
					break;
				cnt--;
			}
			char *tmp;
			tmpInput.set((tmp = _path->subString(0, cnt)));
			if(tmp)
				free(tmp);
			dir tmp_dir = dir();
			if(tmp_dir.fopendir(tmpInput.buff) == FR_OK)
			{
				tmp_dir.fclosedir();
				_path->set(&tmpInput);
				GI::String _tmpInput = GI::String(input->buff + 3);
				cdInt(_path, &_tmpInput);
			}
			else
				return SYS_ERR_INVALID_PATH;
		}
	}
	else
	{
		if(!input->length)
			return SYS_ERR_OK;
		if(_path->length)
		{
			tmpInput.set(_path);
			tmpInput.append((char)'/');
			tmpInput.append(input);
		}
		else
			tmpInput.set(input);
		dir tmp_dir = dir();
		if(tmp_dir.fopendir(tmpInput.buff) == FR_OK)
		{
			tmp_dir.fclosedir();
			_path->set(&tmpInput);
		}
		else
			return SYS_ERR_INVALID_PATH;
	}

	return SYS_ERR_OK;
}

