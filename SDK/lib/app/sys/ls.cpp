/*
 * app/sys/ls.cpp
 */

#ifdef __AVR_XMEGA__
#include <avr/pgmspace.h>
#endif
#include "cmd.h"
#include <lib/fs/fat/inc/ff.h>

SysErr Cmd::ls(int argc, char *argv[])
{
    FRESULT res;
    DIR dir;
    FILINFO fno;
	memset(&dir, 0, sizeof(DIR));
	//memset(&fno, 0, sizeof(FILINFO));

    res = f_opendir(&dir, path->buff);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
				outPath->write((unsigned char *)"/");
				outPath->write((unsigned char *)fno.fname);
				outPath->write((unsigned char *)"\n\r");
            } else {                                       /* It is a file. */
				outPath->write((unsigned char *)fno.fname);
				outPath->write((unsigned char *)"\n\r");
            }
        }
        f_closedir(&dir);
    }
    else
		printError(res);
	return SYS_ERR_OK;
}
