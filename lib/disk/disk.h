#ifndef LIBTUFS_DISK_H
#define LIBTUFS_DISK_H

/******************************************************************************/
#define DISK_BLOCKS  8192      /* number of blocks on the disk                */
#define BLOCK_SIZE   4096      /* block size on "disk"                        */

/******************************************************************************/

extern int active;  /* is the virtual disk open (active) */
extern int handle;      /* tufs_file handle to virtual disk       */

/**
 * Create an empty, virtual disk tufs_file
 * @param name Name of the virtual disk
 * @return
 */
int make_disk(char *name);

/**
 * Open a virtual disk tufs_file
 * @param name Name of the virtual disk
 * @return
 */
__attribute__((unused)) int open_disk(char *name);

/**
 * Close the active virtual disk tufs_file
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

#endif


