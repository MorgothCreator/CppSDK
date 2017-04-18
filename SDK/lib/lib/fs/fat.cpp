/*
 *  lib/fs/fat.cpp
 */
/*#####################################################*/

#include <string.h>
#include "fat.h"

/*#####################################################*/
fatfs::fatfs()
{
	memset(this, 0, sizeof(*this));
}

fatfs::~fatfs()
{

}

FRESULT fatfs::mount 	/* Mount a logical drive */
(
	const TCHAR* path, 	/* Logical drive number to be mounted/unmounted */
	BYTE opt			/* Mode option 0:Do not mount (delayed mount), 1:Mount immediately */
)
{
	return f_mount(&this->fs, path, opt);
}

FRESULT fatfs::unmount ()/* Unmount a logical drive */
{
	return f_mount(&this->fs, NULL, 1);
}

FRESULT fatfs::mkdir 	/* Create a sub directory */
(
	const TCHAR* path	/* Pointer to the directory path */
)
{
	return f_mkdir(path);
}

FRESULT fatfs::unlink 	/* Delete an existing file or directory */
(
	const TCHAR* path	/* Pointer to the file or directory path */
)
{
	return f_unlink(path);
}

FRESULT fatfs::rename 	/* Rename/Move a file or directory */
(
	const TCHAR* path_old,/* Pointer to the object name to be renamed */
	const TCHAR* path_new/* Pointer to the new name */
)
{
	return f_rename(path_old, path_new);
}

FRESULT fatfs::stat 	/* Get file status */
(
	const TCHAR* path, 	/* Pointer to the file path */
	filinfo* fno		/* Pointer to file information to return */
)
{
	return f_stat(path, fno);
}

FRESULT fatfs::chmod 	/* Change attribute of a file/dir */
(
	const TCHAR* path, 	/* Pointer to the file path */
	BYTE attr, 			/* Attribute bits */
	BYTE mask			/* Attribute mask to change */
)
{
	return f_chmod(path, attr, mask);
}

FRESULT fatfs::utime	/* Change timestamp of a file/dir */
(
	const TCHAR* path,	/* Pointer to the file/directory name */
	const filinfo* fno	/* Pointer to the time stamp to be set */
)
{
	return f_utime(path, fno);
}

FRESULT fatfs::chdir	/* Change current directory */
(
	const TCHAR* path	/* Pointer to the directory path */
)
{
	return f_chdir(path);
}

FRESULT fatfs::chdrive	/* Change current drive */
(
	const TCHAR* path	/* Drive number */
)
{
	return f_chdrive(path);
}

FRESULT fatfs::getcwd	/* Get current directory */
(
	TCHAR* buff,		/* Pointer to the directory path */
	UINT len			/* Size of path */
)
{
	return f_getcwd(buff, len);
}

FRESULT fatfs::getfree	/* Get number of free clusters on the drive */
(
	const TCHAR* path,	/* Path name of the logical drive number */
	DWORD* nclst,		/* Pointer to a variable to return number of free clusters */
	FATFS** fatfs		/* Pointer to return pointer to corresponding file system object */
)
{
	return f_getfree(path, nclst, fatfs);
}

FRESULT fatfs::getlabel	/* Get volume label */
(
	const TCHAR* path,	/* Path name of the logical drive number */
	TCHAR* label,		/* Pointer to a buffer to return the volume label */
	DWORD* vsn			/* Pointer to a variable to return the volume serial number */
)
{
	return f_getlabel(path, label, vsn);
}

FRESULT fatfs::setlabel	/* Set volume label */
(
	const TCHAR* label	/* Pointer to the volume label to set */
)
{
	return f_setlabel(label);
}

FRESULT fatfs::mkfs		/* Create a FAT volume */
(
	const TCHAR* path,	/* Logical drive number */
	BYTE opt,			/* Format option */
	DWORD au,			/* Size of allocation unit [byte] */
	void* work,			/* Pointer to working buffer */
	UINT len			/* Size of working buffer */
)
{
	return f_mkfs(path, opt, au, work, len);
}

FRESULT fatfs::fdisk	/* Divide a physical drive into some partitions */
(
	BYTE pdrv,			/* Physical drive number */
	const DWORD* szt,	/* Pointer to the size table for each partitions */
	void* work			/* Pointer to the working buffer */
)
{
	return f_fdisk(pdrv, szt, work);
}
/*#####################################################*/
FRESULT file::fopen		/* Open or create a file */
(
	const TCHAR* path,	/* Pointer to the file name */
	BYTE mode			/* Access mode and file open mode flags */
)
{
	return f_open(&fil, path, mode);
}

FRESULT file::fclose ()	/* Close an open file object */
{
	return f_close(&fil);
}

FRESULT file::fread		/* Read data from the file */
(
	void* buff,			/* Pointer to data buffer */
	UINT btr,			/* Number of bytes to read */
	UINT* br			/* Pointer to number of bytes read */
)
{
	return f_read(&fil, buff, btr, br);
}

UINT file::fread		/* Read data from the file */
(
	void* buff,			/* Pointer to data buffer */
	UINT btr			/* Number of bytes to read */
)
{
	UINT br;
	if(f_read(&fil, buff, btr, &br) != FR_OK)
		return 0;
	return br;
}

FRESULT file::fwrite	/* Write data to the file */
(
	const void* buff,	/* Pointer to the data to be written */
	UINT btw,			/* Number of bytes to write */
	UINT* bw			/* Pointer to number of bytes written */
)
{
	return f_write(&fil, buff, btw, bw);
}

UINT file::fwrite	/* Write data to the file */
(
	const void* buff,	/* Pointer to the data to be written */
	UINT btw			/* Number of bytes to write */
)
{
	UINT bw;
	if(f_write(&fil, buff, btw, &bw) != FR_OK)
		return 0;
	return bw;
}

FRESULT file::flseek	/* Move file pointer of the file object */
(
	FSIZE_t ofs			/* File pointer from top of file */
)
{
	return f_lseek(&fil, ofs);
}

FRESULT file::ftruncate ()	/* Truncate the file */
{
	return f_truncate(&fil);
}

FRESULT file::fsync ()		/* Flush cached data of the writing file */
{
	return f_sync(&fil);
}

FRESULT file::fforward		/* Forward data to the stream */
(
	UINT(*func)(const BYTE*,UINT),/* Pointer to the streaming function */
	UINT btf,				/* Number of bytes to forward */
	UINT* bf				/* Pointer to number of bytes forwarded */
)
{
	return f_forward(&fil, func, btf, bf);
}

FRESULT file::fexpand		/* Allocate a contiguous block to the file */
(
	FSIZE_t szf,			/* File size to be expanded to */
	BYTE opt				/* Operation mode 0:Find and prepare or 1:Find and allocate */
)
{
	return f_expand(&fil, szf, opt);
}
/*#####################################################*/
FRESULT dir::fopendir		/* Open a directory */
(
	const TCHAR* path		/* Pointer to the directory path */
)
{
	return f_opendir(&dr, path);
}

FRESULT dir::fclosedir ()	/* Close an open directory */
{
	return f_closedir(&dr);
}

FRESULT dir::freaddir		/* Read a directory item */
(
	filinfo* fno			/* Pointer to file information to return */
)
{
	return f_readdir(&dr, fno);
}

FRESULT dir::ffindfirst		/* Find first file */
(
	filinfo* fno,			/* Pointer to the file information structure */
	const TCHAR* path,		/* Pointer to the directory to open */
	const TCHAR* pattern	/* Pointer to the matching pattern */
)
{
	return f_findfirst(&dr, fno, path, pattern);
}

FRESULT dir::ffindnext		/* Find next file */
(
	filinfo* fno			/* Pointer to the file information structure */
)
{
	return f_findnext(&dr, fno);
}

