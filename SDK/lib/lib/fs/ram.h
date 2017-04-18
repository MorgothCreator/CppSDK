/*
 *  lib/fs/ram.h
 */

#ifndef RAM_FS_H_
#define RAM_FS_H_

#include <include/global.h>

#ifndef RAM_FS_FILENAME_MAX_LEN
#define RAM_FS_FILENAME_MAX_LEN	9
#endif

#ifndef RAM_FS_DRIVES_MAX
#define RAM_FS_DRIVES_MAX	8
#endif

/* File function return code (FRESULT) */

typedef enum {
	FR_OK = 0,				/* (0) Succeeded */
	FR_DISK_ERR,			/* (1) A hard error occurred in the low level disk I/O layer */
	FR_INT_ERR,				/* (2) Assertion failed */
	FR_NOT_READY,			/* (3) The physical drive cannot work */
	FR_NO_FILE,				/* (4) Could not find the file */
	FR_NO_PATH,				/* (5) Could not find the path */
	FR_INVALID_NAME,		/* (6) The path name format is invalid */
	FR_DENIED,				/* (7) Access denied due to prohibited access or directory full */
	FR_EXIST,				/* (8) Access denied due to prohibited access */
	FR_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
	FR_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
	FR_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
	FR_NOT_ENABLED,			/* (12) The volume has no work area */
	FR_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
	FR_MKFS_ABORTED,		/* (14) The f_mkfs() aborted due to any parameter error */
	FR_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
	FR_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
	FR_NOT_ENOUGH_CORE,		/* (17) Working buffer could not be allocated */
	FR_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > _FS_SHARE */
	FR_INVALID_PARAMETER,	/* (19) Given parameter is invalid */
	FR_DIR_NOT_EMPTY		/* (20) The directory is not empty */
} FRESULT;



#define _RAMFS_MALLOC(size) malloc(size)
#define _RAMFS_CALLOC(nele, size) calloc(nele, size)
#define _RAMFS_REALLOC(ptr, size) realloc(ptr, size)
#define _RAMFS_FREE(size) free(size)

namespace GI {
class ramfile {
public:
	unsigned int parent_entry_offset;
	unsigned int file_size;
	void *data;
	unsigned char file_attr;
	char name[RAM_FS_FILENAME_MAX_LEN];

	enum fa_e{
		FA_OPEN_EXISTING =	0x00,
		FA_READ =			0x01,
		FA_WRITE =			0x02,
		FA_CREATE_NEW =		0x04,
		FA_CREATE_ALWAYS =	0x08,
		FA_OPEN_ALWAYS =	0x10,
		FA__WRITTEN =		0x20,
		FA__DIRTY =			0x40,
		FA__ERROR =			0x80,
	};

	enum attr_access_e{
		ATTR_ACCES_gp =			4,
		ATTR_ACCES_gm =			(0x07 << ATTR_ACCES_gp),

		ATTR_ACCES_WRITABLE =	(0 << ATTR_ACCES_gp), /* The entry is empty */
		ATTR_ACCES_READONLY =	(1 << ATTR_ACCES_gp), /* The entry is empty */
		ATTR_ACCES_EXECUTABLE =	(2 << ATTR_ACCES_gp), /* The entry is empty */
		ATTR_ACCES_SYSTEM =		(3 << ATTR_ACCES_gp), /* The entry is empty */
	};

	enum attr_type_e{
		ATTR_TYPE_gp =			0,
		ATTR_TYPE_gm =			(0x0F << ATTR_TYPE_gp),

		ATTR_TYPE_EMPTY =		(0 << ATTR_TYPE_gp), /* The entry is empty */
		ATTR_TYPE_FILE =		(1 << ATTR_TYPE_gp), /* The entry points to a regular file */
		ATTR_TYPE_STRUCT =		(2 << ATTR_TYPE_gp), /* The entry points to a structure file, that is already allocated (like a link but with size) */
		ATTR_TYPE_STRUCT_FILE =	(3 << ATTR_TYPE_gp), /* The entry points to a structure file , that will be created*/
		ATTR_TYPE_FS =			(4 << ATTR_TYPE_gp), /* The entry point to a file system structure */
		ATTR_TYPE_ROOT =		(8 << ATTR_TYPE_gp), /* The entry point to a directory */
		ATTR_TYPE_DIR =			(9 << ATTR_TYPE_gp), /* The entry point to a directory */
		ATTR_TYPE_NOT_RET_DIR =	(10 << ATTR_ACCES_gp), /* The entry is empty */
		ATTR_TYPE_LINK =		(11 << ATTR_TYPE_gp), /* The entry is a link to another entry */
	};

	static FRESULT unlink (		/* Delete file/directory even filesystems.*/
		const char *path,		/* Pointer to the file name. */
		bool recursive			/* Recursive delete. */
	);


	static FRESULT createOpenFile (				/* FR_OK(0): successful, !=0: error code. */
		GI::ramfile **dj,			/* Pointer to the directory/file object. */
		const char *path,		/* Pointer to the file name. */
		GI::ramfile::fa_e mode,		/* Access mode and file open mode flags. */
		GI::ramfile::attr_type_e type,		/* If is a file, structure or fs structure here is the type.*/
		unsigned int addr,		/* If is a link here is the address. */
		GI::ramfile::attr_access_e permision,/* Here is the permission. */
		unsigned int filesize	/* If is a file, structure or fs structure here is the size. */
	);
	static FRESULT itemDelete (			/* FR_OK(0): successful, !=0: error code. */
		GI::ramfile **dj			/* Pointer to the directory object. */
	);
	static FRESULT itemRecursiveDelete (/* FR_OK(0): successful, !=0: error code. */
		GI::ramfile **dj			/* Pointer to the directory object. */
	);


private:
	static FRESULT registerFile (			/* FR_OK(0): successful, !=0: error code. */
		GI::ramfile **dj,			/* Pointer to the directory/file object. */
		const char *path,		/* Pointer to the file name. */
		unsigned char attr,		/* Attribute for new entry. */
		unsigned int addr,		/* If is a link or a structure here is the address. */
		unsigned int size		/* If is a file, this contain the initial size of the file, if is a link this contain the item address. */
	);
	static FRESULT findFile (				/* FR_OK(0): successful, !=0: error code. */
		GI::ramfile **dj,			/* Pointer to the directory/file object. */
		const char *path		/* Pointer to the file name. */
	);
	static FRESULT followPath (			/* FR_OK(0): successful, !=0: error code. */
		GI::ramfile **dj,			/* Pointer to the directory/file object. */
		const char *path,		/* Full-path string to find a file or directory. */
		GI::ramfile::fa_e mode,		/* Access mode and file open mode flags. */
		GI::ramfile::attr_type_e type,		/* If is a file, structure or fs structure here is the type. . */
		unsigned int addr,		/* If is a link here is the address. */
		GI::ramfile::attr_access_e permision,/* Here is the permission. */
		unsigned int filesize	/* If is a file, structure or fs structure here is the size. */
	);
};

class ramdisk{
public:
	ramdisk(		/* FR_OK(0): successful, !=0: error code. */
		const char *fs_name			/* File system name. */
	);
	~ramdisk();

	GI::ramfile *fs;
	char name[RAM_FS_FILENAME_MAX_LEN];
};
}

//extern avr_fs *ram_fs_root[];

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "ram.cpp"
#endif
/*#####################################################*/
#endif /* RAM_FS_H_ */
