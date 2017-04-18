/*
 *  lib/fs/fat.h
 */
/*#####################################################*/
#ifndef LIB_LIB_FS_FAT_FAT_H_
#define LIB_LIB_FS_FAT_FAT_H_

#include <lib/fs/fat/inc/ff.h>
#include <lib/fs/fat/inc/integer.h>

typedef FILINFO filinfo;

class fatfs {
public:
	fatfs();
	~fatfs();
	FRESULT mount (const TCHAR* path, BYTE opt);									/* Mount a logical drive */
	FRESULT unmount ();															/* Unmount a logical drive */
	static FRESULT mkdir (const TCHAR* path);											/* Create a sub directory */
	static FRESULT unlink (const TCHAR* path);											/* Delete an existing file or directory */
	static FRESULT rename (const TCHAR* path_old, const TCHAR* path_new);				/* Rename/Move a file or directory */
	static FRESULT stat (const TCHAR* path, FILINFO* fno);								/* Get file status */
	static FRESULT chmod (const TCHAR* path, BYTE attr, BYTE mask);						/* Change attribute of a file/dir */
	static FRESULT utime (const TCHAR* path, const FILINFO* fno);						/* Change timestamp of a file/dir */
	static FRESULT chdir (const TCHAR* path);											/* Change current directory */
	static FRESULT chdrive (const TCHAR* path);											/* Change current drive */
	static FRESULT getcwd (TCHAR* buff, UINT len);										/* Get current directory */
	static FRESULT getfree (const TCHAR* path, DWORD* nclst, FATFS** fatfs);			/* Get number of free clusters on the drive */
	static FRESULT getlabel (const TCHAR* path, TCHAR* label, DWORD* vsn);				/* Get volume label */
	static FRESULT setlabel (const TCHAR* label);										/* Set volume label */
	static FRESULT mkfs (const TCHAR* path, BYTE opt, DWORD au, void* work, UINT len);	/* Create a FAT volume */
	static FRESULT fdisk (BYTE pdrv, const DWORD* szt, void* work);						/* Divide a physical drive into some partitions */

	FATFS fs;
};

class file {
public:
	FRESULT fopen (const TCHAR* path, BYTE mode);										/* Open or create a file */
	FRESULT fclose ();																	/* Close an open file object */
	FRESULT fread (void* buff, UINT btr, UINT* br);										/* Read data from the file */
	UINT fread (void* buff, UINT btr);													/* Read data from the file */
	FRESULT fwrite (const void* buff, UINT btw, UINT* bw);								/* Write data to the file */
	UINT fwrite (const void* buff, UINT btw);											/* Write data to the file */
	FRESULT flseek (FSIZE_t ofs);														/* Move file pointer of the file object */
	FRESULT ftruncate ();																/* Truncate the file */
	FRESULT fsync ();																	/* Flush cached data of the writing file */
	FRESULT fforward (UINT(*func)(const BYTE*,UINT), UINT btf, UINT* bf);				/* Forward data to the stream */
	FRESULT fexpand (FSIZE_t szf, BYTE opt);											/* Allocate a contiguous block to the file */

	FIL fil;
};

class dir {
public:
	FRESULT fopendir (const TCHAR* path);												/* Open a directory */
	FRESULT fclosedir ();																/* Close an open directory */
	FRESULT freaddir (FILINFO* fno);													/* Read a directory item */
	FRESULT ffindfirst (FILINFO* fno, const TCHAR* path, const TCHAR* pattern);			/* Find first file */
	FRESULT ffindnext (FILINFO* fno);													/* Find next file */

	DIR dr;
};

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "fat.cpp"
#endif
/*#####################################################*/
#endif /* LIB_LIB_FS_FAT_FAT_H_ */
