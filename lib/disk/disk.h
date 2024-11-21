#ifndef LIBTUFS_DISK_H
#define LIBTUFS_DISK_H

#include "tufsdef.h"

/******************************************************************************/

/******************************************************************************/

extern int active;  /* is the virtual disk open (active) */
extern int handle;  /* handle to virtual disk  */

extern struct tufs_fat *p_fat;
extern struct tufs_root *p_root;
extern struct boot_sector *p_boot;

/**
 * Array of open files. Index corresponds to file descriptor.
 */
extern file_t *file_table[MAX_FILES];

/**
 * Create an empty, virtual disk
 * @param name Name of the virtual disk
 * @return
 */
int make_disk(char *name);

/**
 * Open a virtual disk
 * @param name Name of the virtual disk
 * @return
 */
int open_disk(char *name);

/**
 * Close the active virtual disk
 * @return
 */
int close_disk();

/**
 * Write a block of size BLOCK_SIZE to the virtual disk
 * @param block Block to write to
 * @param buf Data to write
 * @return
 */
int block_write(int block, char *buf);

/**
 * Read a block of size BLOCK_SIZE from the virtual disk
 * @param block Block to read from
 * @param buf Buffer to store the data
 * @return
 */
int block_read(int block, char *buf);

/******************************************************************************/

/**
 * Create the disk file, open it, add the necessary metadata, close.
 * @param name Name of the disk file to initialize
 * @return 0 on success, -1 on failure.
 */
int init_fs(char *name);

/**
 * Mount the virtual file system; load filesystem metadata into memory
 * @return 0 on success, -1 on failure
 */
int mount_fs();

/**
 * Synchronize the filesystem metadata in memory with the physical disk.
 * @return 0 on success, -1 on failure.
 */
int sync_fs();

/**
 * Unmount the virtual file system; unload the filesystem metadata from memory.
 * Write any changes to the physical disk.
 * @return 0 on success, -1 on failure.
 */
int unmount_fs();

#endif


