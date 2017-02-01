/*
 * ls.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: John Smith
 */

#include "ls.h"
#include "cmd.h"
#include <include/global.h>
#include <lib/fs/fat/inc/ff.h>


static const char *fs_err_table[] = {
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

SysErr Cmd::ls(int argc, char *argv[])
{
	outPath->write((unsigned char *)"This is ls function\n\r");

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
			errPath->write("Unknown error");
			errPath->write("\n\r");
    }
	return SYS_ERR_OK;
}
