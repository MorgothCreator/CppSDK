/*
 * cat.cpp
 *
 *  Created on: Feb 8, 2017
 *      Author: John Smith
 */

#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#endif
#include <lib/fs/fat.h>
#include "cmd.h"

SysErr Cmd::cat(int argc, char *argv[])
{
	int argc_cnt = 0;
	GI::String input = GI::String();
	input.set(argv[argc_cnt]);

	int cnt = input.length - 1;
	while(cnt != -1)
	{
		if(input.buff[cnt] == '/')
			break;
		cnt--;
	}
	char *tmp_filename;
	GI::String filename = GI::String((tmp_filename = input.subString(cnt + 1)));
	if(tmp_filename)
		free(tmp_filename);
	char *tmp_path;
	GI::String directory = GI::String((tmp_path = input.subString(0, cnt + 1)));
	if(tmp_path)
		free(tmp_path);

	GI::String directory_with_path = GI::String(path->buff);

	SysErr err = cdInt(&directory_with_path, &directory);
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
	file file_read = file();
	if(directory_with_path.length)
	{
		directory_with_path.append((char)'/');
		directory_with_path.append(&filename);
	}
	else
		directory_with_path.set(&filename);


	FRESULT res = file_read.fopen(directory_with_path.buff, FA_READ);
	if(!res)
	{
		unsigned char buff[32];
		unsigned int bytes_read;
		while((bytes_read = file_read.fread(buff, 32)))
		{
			outPath->write((unsigned char *)buff, bytes_read);
		}
		file_read.fclose();
	}
	else
	{
		printError(res);
	}

	return SYS_ERR_OK;
}



