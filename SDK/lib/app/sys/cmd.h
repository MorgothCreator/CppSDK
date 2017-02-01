/*
 * sys.h
 *
 *  Created on: Jan 28, 2017
 *      Author: John Smith
 */

#ifndef LIB_APP_SYS_CMD_H_
#define LIB_APP_SYS_CMD_H_

#include <lib/string.h>
#include <api/io_handle.h>
#include <include/global.h>
#include <api/gpio.h>

class Cmd
{
public:
	Cmd(char *inPath, char *outPath, char *errPath);
	~Cmd();
	SysErr idle();
	SysErr ls(int argc, char *argv[]);

private:
	GI::String *input;
	GI::String *path;
	GI::IO *inPath;
	GI::IO *outPath;
	GI::IO *errPath;
};


#endif /* LIB_APP_SYS_CMD_H_ */
