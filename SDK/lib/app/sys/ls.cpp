/*
 * ls.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: John Smith
 */

#include "cmd.h"
#include <include/global.h>
#include <lib/fs/fat/inc/ff.h>

SysErr Cmd::ls(int argc, char *argv[])
{
	//outPath->write((unsigned char *)"This is ls function\n\r");
    FRESULT res;
    DIR dir;
    //UINT i;
    FILINFO fno;

    res = f_opendir(&dir, path->buff);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
				outPath->write((unsigned char *)"/");
				outPath->write((unsigned char *)fno.fname);
                //i = strlen(path);
                //sprintf(&path[i], "/%s", fno.fname);
                //res = scan_files(path);                    /* Enter the directory */
                //if (res != FR_OK) break;
                //path[i] = 0;
				outPath->write((unsigned char *)"\n\r");
            } else {                                       /* It is a file. */
				outPath->write((unsigned char *)fno.fname);
				outPath->write((unsigned char *)"\n\r");
                //printf("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }
    else
    {
    	if(res <= FR_INVALID_PARAMETER)
			errPath->write((unsigned char *)fs_err_table[res]);
    	else
			errPath->write((unsigned char *)"Unknown error");
		errPath->write((unsigned char *)"\n\r");
    }
	return SYS_ERR_OK;
}
