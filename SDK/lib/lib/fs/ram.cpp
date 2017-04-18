/*
 *  lib/fs/ram.cpp
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
//#include <avr/pgmspace.h>
#include <lib/fs/ram.h>


static volatile bool ram_fs_lock = false;
static volatile unsigned int ram_fs_modifyed = 0;
static GI::ramdisk **ramfs = {NULL};
static bool system_permision_request = false;


static u32 checkIfFsExist(const char *name)
/* Check if a FS with the same name already exist.
 * Return (u32)-1 if not exist, Return entry number if exist
 */
{
	u32 cnt = 0;
	while(ramfs[cnt] != NULL)
	{
		if(ramfs[cnt] != (GI::ramdisk *)-1)
		{
			if(!strncmp(ramfs[cnt]->fs->name, name, RAM_FS_FILENAME_MAX_LEN))
				return cnt;
		}
	}
	return (u32)-1;
}

static u32 getEmptyFsSlot(const char *name)
/* Return number of a free slot, if are not free slots expand the entry table.
 * Return (u32)-1 if out of memory, return entry number if succesfully allocated.
 */
{
	u32 cnt = 0;
	while(ramfs[cnt] != NULL)
	{
		if(ramfs[cnt]->fs == NULL)
		{
			break;
		}
	}
	if(!ramfs)
		ramfs = (GI::ramdisk **)calloc(1, sizeof(GI::ramdisk **));
	if(!ramfs)
		return (u32)-1;
	GI::ramdisk **_ram_fs_root = NULL;
	if(ramfs[cnt] == NULL)
	{
		_ram_fs_root = (GI::ramdisk **)realloc(ramfs, sizeof(GI::ramdisk *) * (cnt + 2));
		_ram_fs_root[cnt + 1] = NULL;
	}
	else
		_ram_fs_root = ramfs;
	if(_ram_fs_root)
	{
		ramfs = _ram_fs_root;
		_ram_fs_root[cnt]->fs = (GI::ramfile *)_RAMFS_CALLOC(1, sizeof(GI::ramfile));
		if(!_ram_fs_root[cnt]->fs)
			return (u32)-1;
		GI::ramfile *ram_header = ramfs[cnt]->fs;
		ram_header->data = (unsigned char *)ram_header;
		ram_header->parent_entry_offset = 0;
		ram_header->file_attr = GI::ramfile::ATTR_TYPE_ROOT;
		ram_header->file_size = sizeof(GI::ramfile);
		strncpy(ram_header->name, name, RAM_FS_FILENAME_MAX_LEN);/* Write on new directory name of parent directory. */
		return cnt;
	}
	return (u32)-1;
}
/*-----------------------------------------------------------------------*/
/* Public functions for RAMDISK                                          */
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
/* Fs initialize                                                         */
/*-----------------------------------------------------------------------*/
GI::ramdisk::ramdisk /* Create a new FS container into ram.*/
(
	const char *fs_name			/* File system name. */
)
{
	memset(this, 0, sizeof(*this));
	if(checkIfFsExist(fs_name) == (u32)-1)
		return;
	u32 drive;
	if((drive = getEmptyFsSlot(fs_name)) == (u32)-1)
		return;
	return;
}

/*-----------------------------------------------------------------------*/
/* Destroy FS                                                            */
/*-----------------------------------------------------------------------*/

GI::ramdisk::~ramdisk() /* Delete file/directory even filesystems.*/
{
	u32 entry_nr;
	if((entry_nr = checkIfFsExist(name)) == (u32)-1)
		return;
	FRESULT res1 = FR_OK, res2 = FR_OK;
	GI::ramfile *fs = ramfs[entry_nr]->fs;
	res1 = GI::ramfile::createOpenFile(&fs, name, GI::ramfile::FA_OPEN_EXISTING, GI::ramfile::ATTR_TYPE_EMPTY, 0, GI::ramfile::ATTR_ACCES_READONLY, 0);
	/* if an error has occurred navigate to given path, return the error. */
	if(res1 != FR_OK)
	{
		return;
	}
	res1 = GI::ramfile::itemRecursiveDelete(&fs);
	res2 = GI::ramfile::itemDelete(&fs);
	/* If given path has been deleted successfully return the recursive delete result */
	/* If given path delete has been denied signify that is the root path and cannot be deleted, return  the recursive delete result */
	if(res1 == FR_DENIED || res2 == FR_OK)
	{
		return;
	}
	delete ramfs[entry_nr]->fs;
	ramfs[entry_nr]->fs = NULL;
}


/*-----------------------------------------------------------------------*/
/* FILE                                                                  */
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
/* Delete file/directory with given path                                 */
/*-----------------------------------------------------------------------*/

FRESULT GI::ramfile::unlink /* Delete file/directory even filesystems.*/
(
	const char *path,	/* Pointer to the file name. */
	bool recursive		/* Recursive delete. */
)
{
	FRESULT res1 = FR_OK, res2 = FR_OK;
	GI::ramfile *fs;
	res1 = createOpenFile(&fs, path, FA_OPEN_EXISTING, ATTR_TYPE_EMPTY, 0, ATTR_ACCES_READONLY, 0);
	/* if an error has occurred navigate to given path, return the error. */
	if(res1 != FR_OK)
	{
		return res1;
	}
	if(recursive)
	{
		res1 = itemRecursiveDelete(&fs);
	}
	else
		res1 = FR_OK;
	res2 = itemDelete(&fs);
	/* If given path has been deleted successfully return the recursive delete result */
	/* If given path delete has been denied signify that is the root path and cannot be deleted, return  the recursive delete result */
	if(res1 == FR_DENIED || res2 == FR_OK)
	{
		return res1;
	}
	/* if an error has occurred when try to delete given path, return that error. */
	return res2;

}




/*-----------------------------------------------------------------------*/
/* Private functions                                                     */
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
/* Create a file/directory in current directory                          */
/*-----------------------------------------------------------------------*/

FRESULT GI::ramfile::registerFile (
	GI::ramfile **dj,			/* Pointer to the directory/file object. */
	const char *path,		/* Pointer to the file name. */
	unsigned char attr,		/* Attribute for new entry. */
	unsigned int addr,		/* If is a link or a structure here is the address. */
	unsigned int size		/* If is a file, this contain the initial size of the file, if is a link this contain the item address. */
)
{
	if(ram_fs_lock)
		return FR_LOCKED;
	if(strlen(path) > RAM_FS_FILENAME_MAX_LEN)
		return FR_INVALID_NAME;
	ram_fs_lock = true;/* Lock the ram_fs, to avoid interference with another modification done by another thread. */
	GI::ramfile *new_dir = (GI::ramfile *)addr;/* Write the linked item address, if is a link, this will be used for new entry. */
	if((attr & ATTR_TYPE_gm) != ATTR_TYPE_LINK &&
		(attr & ATTR_TYPE_gm) != ATTR_TYPE_STRUCT)/* If is a link or a structure no new memory is allocated. */
	{
		if((attr & ATTR_TYPE_gm) == ATTR_TYPE_DIR || (attr & ATTR_TYPE_gm) == ATTR_TYPE_NOT_RET_DIR)
		{
			new_dir = (GI::ramfile *)_RAMFS_CALLOC(1, sizeof(GI::ramfile));/* Allocate memory for new directory. */
		} 
		else
		{
			new_dir = (GI::ramfile *)_RAMFS_CALLOC(1, size);/* Allocate memory for new file. */
		}
	}
	if(!new_dir)
	{
		ram_fs_lock = false;/* Unlock the ram_fs */
		return FR_NOT_ENOUGH_CORE;
	}
	GI::ramfile *actual_dir = *dj;
	/* Search an old allocated entry that now is empty */
	GI::ramfile *cur_dj_search_entry = actual_dir + 1;/* Skip write actual directory address to the first entry of current directory, because point to parent directory. */
	bool empty_entry_discovered = false;
	for (; cur_dj_search_entry < (actual_dir->file_size / sizeof(GI::ramfile)) + actual_dir; cur_dj_search_entry += 1)/* Check if is a old entry allocated but empty. */
	{
		if((cur_dj_search_entry->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_EMPTY)
		{
			empty_entry_discovered = true;
			break;
		}
	}/* If no empty already allocated entry is not found, relocate the current directory with the new size and actualize the references to actual directory*/
	if(!empty_entry_discovered)
	{
		actual_dir = (GI::ramfile *)_RAMFS_REALLOC(actual_dir, actual_dir->file_size + sizeof(GI::ramfile));/* Reallocate memory for current directory, with one entry more memory.*/
		if(!actual_dir)
		{
			_RAMFS_FREE(new_dir);
			ram_fs_lock = false;/* Unlock the ram_fs */
			return FR_NOT_ENOUGH_CORE;
		}
		unsigned char search_drive = 0;
		for(; search_drive < RAM_FS_DRIVES_MAX; search_drive++)
		{
			if(*dj == ramfs[search_drive]->fs)
			{
				ramfs[search_drive]->fs = actual_dir;/* The root directory has been relocated, actualize the global root directory address. */
				actual_dir->data = (unsigned char *)actual_dir;/* If is root, need to actualize the root directory address, because the first entry will point to himself. */
			}
		}
		GI::ramfile *parent_entry = (GI::ramfile *)(actual_dir->parent_entry_offset + (char *)(actual_dir->data));/* Calculate parent entry address, if is root directory the parent will be himself. */
		ram_fs_modifyed++;/* This is a semaphore that will modify the value every time the ram_fs will be modified. */
		parent_entry->data = (unsigned char *)actual_dir;/* Write the new address of actual directory to parent entry. */
		GI::ramfile *cur_dj = actual_dir + 1;/* Skip write actual dir address to the first entry of current directory, because point to parent directory. */
		for (; cur_dj < (actual_dir->file_size / sizeof(GI::ramfile)) + actual_dir; cur_dj += 1)/* Write the new address of actual directory to all children directory's of current directory. */
		{
			unsigned char file_attr = cur_dj->file_attr & ATTR_TYPE_gm;
			if(file_attr != ATTR_TYPE_FILE && file_attr != ATTR_TYPE_STRUCT)
			{
				GI::ramfile *pointed_dj = (GI::ramfile *)cur_dj->data;
				pointed_dj->data = (unsigned char *)actual_dir;
			}
		
		}
	}
	if((attr & ATTR_TYPE_gm) == ATTR_TYPE_DIR ||
		(attr & ATTR_TYPE_gm) == ATTR_TYPE_NOT_RET_DIR)
	{
		new_dir->data = (unsigned char *)actual_dir;/* Write on new directory the address of actual directory. */
		new_dir->parent_entry_offset = actual_dir->file_size;/* Write on new directory the offset address of parent entry. */
		new_dir->file_size = sizeof(GI::ramfile);/* Write on new directory the size of new directory. */
		new_dir->file_attr = ATTR_TYPE_DIR;/* Write on new directory attribute of new directory. */
		strcpy(new_dir->name, "..");/* Write on new directory name of parent directory. */
	}
	GI::ramfile *new_entry = (GI::ramfile *)actual_dir + (actual_dir->file_size / sizeof(GI::ramfile));/* Calculate the position of new entry on current directory. */
	if(empty_entry_discovered)
	{/* In case of an already allocated entry that is empty. */
		new_entry = cur_dj_search_entry;/* New entry address is the first empty discovered entry. */
	}
	else
	{/* In case of A new entry has been allocated. */
		actual_dir->file_size = actual_dir->file_size + sizeof(GI::ramfile);/* Actualize the size of current directory. */
	}
	strncpy(new_entry->name, path, RAM_FS_FILENAME_MAX_LEN);/* Write the name of new directory. */
	new_entry->file_attr = attr;/* Write attribute of new directory. */
	new_entry->data = (unsigned char *)new_dir;/* Write the name of new item. */
	if((attr & ATTR_TYPE_gm) == ATTR_TYPE_DIR ||
		(attr & ATTR_TYPE_gm) == ATTR_TYPE_NOT_RET_DIR ||
			(attr & ATTR_TYPE_gm) == ATTR_TYPE_LINK)
	{
		new_entry->file_size = 0;/* This is a directory or a link, the size is 0. */
	} 
	else 
	{
		new_entry->file_size = size;/* This is a file, write the size. */
	}
	*dj = new_entry;/* Return new created entry address */
	ram_fs_lock = false;/* Unlock the ram_fs */
	return FR_OK;
}





/*-----------------------------------------------------------------------*/
/* Delete a pointed file/directory                                       */
/*-----------------------------------------------------------------------*/

FRESULT GI::ramfile::itemDelete (
		GI::ramfile **dj			/* Pointer to the directory object. */
)
{
	GI::ramfile *entry_to_delete = *dj;
	if(((entry_to_delete->file_attr & ATTR_ACCES_gm) == ATTR_ACCES_READONLY ||
		(entry_to_delete->file_attr & ATTR_ACCES_gm) == ATTR_ACCES_SYSTEM ||
			(entry_to_delete->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_NOT_RET_DIR) &&
				system_permision_request == false)
	{
		return FR_WRITE_PROTECTED;
	}
	if((entry_to_delete->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_ROOT)
	{
		return FR_DENIED;/* This is a root directory and cannot be deleted */
	}
	GI::ramfile *file_to_delete = (GI::ramfile *)entry_to_delete->data;
	if((entry_to_delete->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_DIR ||
		(entry_to_delete->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_NOT_RET_DIR)
	{
		/* This is a directory, delete current directory and pointed entry */
		GI::ramfile *cur_dj_search_entry = file_to_delete + 1;/* Skip first entry of actual directory. */
		for (; cur_dj_search_entry < (file_to_delete->file_size / sizeof(GI::ramfile)) + file_to_delete; cur_dj_search_entry += 1)/* Search if directory is not empty */
		{
			if((cur_dj_search_entry->file_attr & ATTR_TYPE_gm) != ATTR_TYPE_EMPTY)
			{
				return FR_DIR_NOT_EMPTY;
			}
		}
	}
	//avr_fs *entry_to_delete = (avr_fs *)file_to_delete->data + (file_to_delete->parent_entry_offset / sizeof(avr_fs));
	if((entry_to_delete->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_DIR ||
		(entry_to_delete->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_NOT_RET_DIR ||
			(entry_to_delete->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_FILE ||
				(entry_to_delete->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_STRUCT_FILE)
	{
#if (RAM_FS_DEBUG == 1)
		memset(file_to_delete, 0, file_to_delete->file_size);
#endif
		_RAMFS_FREE(file_to_delete);
	}
	entry_to_delete->file_attr &= ~ATTR_TYPE_gm;
	entry_to_delete->data = 0;
	return FR_OK;
}




/*-----------------------------------------------------------------------*/
/* Delete a pointed file/directory                                       */
/*-----------------------------------------------------------------------*/

FRESULT GI::ramfile::itemRecursiveDelete (
		GI::ramfile **dj			/* Pointer to the directory/file object. */
)
{
	FRESULT res = FR_OK;
	GI::ramfile *tmp_dj = *dj;
	/* This is a directory, delete current directory and pointed entry */
	GI::ramfile *cur_dj_search_entry = *dj + 1;/* Skip first entry of actual directory. */
	for (; cur_dj_search_entry < (tmp_dj->file_size / sizeof(GI::ramfile)) + tmp_dj; cur_dj_search_entry += 1)/* Search if directory is not empty */
	{
		res = itemDelete(&cur_dj_search_entry);
		if(res != FR_OK)
		{
			GI::ramfile *_cur_dj_search_entry = (GI::ramfile *)cur_dj_search_entry->data;
			res = itemRecursiveDelete(&_cur_dj_search_entry);
			if(res != FR_OK)
			{
				return res;
			}
			res = itemDelete(&cur_dj_search_entry);
			if(res != FR_OK)
			{
				return res;
			}
		}
	}
	return res;
}






/*-----------------------------------------------------------------------*/
/* Find a item entry name                                                */
/*-----------------------------------------------------------------------*/

FRESULT GI::ramfile::findFile (
	GI::ramfile **dj,			/* Pointer to the directory/file object. */
	const char *path		/* Pointer to the file name. */
)
{
	GI::ramfile *cur_dj = *dj;
	GI::ramfile *ymp_cur_dj = *dj;
	for (;(GI::ramfile *)cur_dj < (ymp_cur_dj->file_size/sizeof(GI::ramfile)) + ymp_cur_dj; cur_dj += 1)
	{
		if(strncmp(path, ((GI::ramfile *)cur_dj)->name, RAM_FS_FILENAME_MAX_LEN) == 0 && (((GI::ramfile *)cur_dj)->file_attr & ATTR_TYPE_gm) != ATTR_TYPE_EMPTY)
		{
			*dj = (GI::ramfile *)cur_dj;
			return FR_OK;
		}
	}
	return FR_NO_PATH;
}




/*-----------------------------------------------------------------------*/
/* Follow a file path                                                    */
/*-----------------------------------------------------------------------*/

FRESULT GI::ramfile::followPath (	/* FR_OK(0): successful, !=0: error code. */
	GI::ramfile **dj,			/* Pointer to the directory/file object. */
	const char *path,		/* Full-path string to find a file or directory. */
	GI::ramfile::fa_e mode,		/* Access mode and file open mode flags. */
	GI::ramfile::attr_type_e type,		/* If is a file, structure or fs structure here is the type. . */
	unsigned int addr,		/* If is a link here is the address. */
	GI::ramfile::attr_access_e permision,/* here is the permision. */
	unsigned int filesize	/* If is a file, structure or fs structure here is the size. */
)
{
	FRESULT res = FR_OK;
	char *segment_start = (char *)_RAMFS_MALLOC(strlen(path) + 1);
	char *segment_end;
	char *dir_back = segment_start;
	strcpy(segment_start, path);
#if (RAM_FS_DEBUG == 1)
	int size_of_tmp_str = strlen(dir_back);
#endif
	GI::ramfile *cur_dj = *dj;

	for (;;) {
		segment_end = strpbrk (segment_start, "/\\"); /* Find end of path segment. */
		if(!segment_end)
		{
			if(strlen(segment_start))
			{
				/* Find file */
				res = findFile(&cur_dj, segment_start);
				if(res == FR_OK)
				{
					*dj = cur_dj;
				}
				else
				{
					if(mode == FA_CREATE_ALWAYS || mode == FA_OPEN_ALWAYS)
					{/* Verify if current directory is write protected */
						if(((cur_dj->file_attr & ATTR_ACCES_gm) == ATTR_ACCES_READONLY ||
							(cur_dj->file_attr & ATTR_ACCES_gm) == ATTR_ACCES_SYSTEM) &&
								system_permision_request == false)
						{
							res = FR_WRITE_PROTECTED;
						}
						else
						{
							if((cur_dj->file_attr & ATTR_TYPE_gm) != ATTR_TYPE_DIR ||
								(cur_dj->file_attr & ATTR_TYPE_gm) != ATTR_TYPE_NOT_RET_DIR)/* Verify if current entry is a directory entry */
							{
#if (RAM_FS_DEBUG == 1)
								memset(dir_back, 0, size_of_tmp_str);
#endif
								if(dir_back)
									_RAMFS_FREE(dir_back);
								return FR_NO_PATH;
							}
							//cur_dj = (avr_fs *)cur_dj->data;
							res = registerFile(&cur_dj, segment_start, (type & ATTR_TYPE_gm) | (permision & ATTR_ACCES_gm), addr, filesize);/* Create a non directory entry or file. */
							*dj = cur_dj;
						}
					}
				}
#if (RAM_FS_DEBUG == 1)
				memset(dir_back, 0, size_of_tmp_str);
#endif
				if(dir_back)
					_RAMFS_FREE(dir_back); /* File has been founded or created. */
				return res;
			} 
			else
			{
#if (RAM_FS_DEBUG == 1)
				memset(dir_back, 0, size_of_tmp_str);
#endif
				if(dir_back)
					_RAMFS_FREE(dir_back); /* Directory has been founded. */
				//*dj = cur_dj;
				*dj = (GI::ramfile *)cur_dj->data;
				return FR_OK;
			}
		}
		if((cur_dj->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_FILE ||
			(cur_dj->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_STRUCT ||
				(cur_dj->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_FS)/* If a fs is available, this will be removed. */
		{
#if (RAM_FS_DEBUG == 1)
			memset(dir_back, 0, size_of_tmp_str);
#endif
			if(dir_back)
				_RAMFS_FREE(dir_back);
			return FR_NO_PATH;
		}
		*segment_end = 0;
		if(!strcmp(segment_start, "..") && (cur_dj->file_attr & ATTR_TYPE_gm) == ATTR_TYPE_NOT_RET_DIR)
		{
			if(dir_back)
				_RAMFS_FREE(dir_back);
			*dj = cur_dj;
			return FR_NO_PATH;
		}
		/* Find directory */
		res = findFile(&cur_dj, segment_start);
		if(res != FR_OK)
		{
			res = FR_NO_PATH;
			if(mode == FA_CREATE_ALWAYS || mode == FA_OPEN_ALWAYS)
			{
				if(((cur_dj->file_attr & ATTR_ACCES_gm) == ATTR_ACCES_READONLY ||
					(cur_dj->file_attr & ATTR_ACCES_gm) == ATTR_ACCES_SYSTEM) &&
						system_permision_request == false)
				{
					res = FR_WRITE_PROTECTED;
				}
				else
				{
					if((cur_dj->file_attr & ATTR_TYPE_gm) != ATTR_TYPE_DIR &&
						(cur_dj->file_attr & ATTR_TYPE_gm) != ATTR_TYPE_NOT_RET_DIR &&
							(cur_dj->file_attr & ATTR_TYPE_gm) != ATTR_TYPE_ROOT)/* If is not a directory or a root can't navigate from here. */
					{
#if (RAM_FS_DEBUG == 1)
						memset(dir_back, 0, size_of_tmp_str);
#endif
						if(dir_back)
							_RAMFS_FREE(dir_back);
						return FR_NO_PATH;
					}
					res = registerFile(&cur_dj, segment_start, ATTR_TYPE_DIR | (permision & ATTR_ACCES_gm), addr, filesize);/* Create a directory. */
				}
			}
			if(res != FR_OK)
			{
#if (RAM_FS_DEBUG == 1)
				memset(dir_back, 0, size_of_tmp_str);
#endif
				if(dir_back)
					_RAMFS_FREE(dir_back);
				return res;
			}
		}
		segment_start = segment_end + 1;
		if(!cur_dj->data)
		{
			if(dir_back) _RAMFS_FREE(dir_back);
			return FR_INVALID_OBJECT;
		}
		if(strlen(segment_start))
		{
			cur_dj = (GI::ramfile *)cur_dj->data;
		}
	}
#if (RAM_FS_DEBUG == 1)
	memset(dir_back, 0, size_of_tmp_str);
#endif
	if(dir_back)
		_RAMFS_FREE(dir_back);
	return res;
}




/*-----------------------------------------------------------------------*/
/* Open or Create a File                                                 */
/*-----------------------------------------------------------------------*/

FRESULT GI::ramfile::createOpenFile (
	GI::ramfile **dj,			/* Pointer to the directory/file object. */
	const char *path,		/* Pointer to the file name. */
	GI::ramfile::fa_e mode,		/* Access mode and file open mode flags. */
	GI::ramfile::attr_type_e type,	/* If is a file, structure or fs structure here is the type. . */
	unsigned int addr,		/* If is a link here is the address. */
	GI::ramfile::attr_access_e permision,/* here is the permision. */
	unsigned int filesize	/* If is a file, structure or fs structure here is the size. */
)
{
	char *segment_end = strchr (path, ':'); /* Find end of path segment. */
	if(!segment_end)
		return FR_INVALID_NAME;
	u32 entry = checkIfFsExist(path);

	if(entry == (u32)-1)
		return FR_INVALID_DRIVE;
	*dj = ramfs[entry]->fs;
	path = segment_end + 1;/* Strip it and start from the root directory */

	unsigned char _mode = mode;
	_mode &= (int)FA_READ | FA_WRITE | FA_CREATE_ALWAYS | FA_OPEN_ALWAYS | FA_CREATE_NEW;
	GI::ramfile *fs;
	return followPath(&fs, path, (GI::ramfile::fa_e)_mode, type, addr, permision, filesize);	/* Follow the file path. */
}




