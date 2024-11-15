//
// Created by Samir Buch on 11/8/24.
//

#ifndef TUFSIMPL_TUFSIO_H
#define TUFSIMPL_TUFSIO_H

#ifndef size_t
#include <stddef.h>
#endif

/**
 * File descriptor
 */
typedef int tufs_fd_t;

/**
 * File offset
 */
typedef unsigned int tufs_off_t;

/**
 * Open a tufs_dirent on the virtual disk in RW mode. If successful, returns a non-negative integer.
 * Note that the same tufs_dirent can be opened multiple times. When this happens, multiple, independent
 * tufs_dirent descriptors are provided per open, up to a filesystem maximum of 32 tufs_dirent descriptors.
 * @param name File to open
 * @return Unique tufs_dirent descriptor for the open tufs_dirent. -1 on failure.
 */
tufs_fd_t fs_open(char *name);

/**
 * Close a tufs_dirent by FD. A closed tufs_dirent descriptor can no longer be used to access the corresponding
 * tufs_dirent on the virtual disk.
 * @param file_descriptor File descriptor to close
 * @return On success, 0. On failure, -1.
 */
int fs_close(tufs_fd_t file_descriptor);

/**
 * Create a new tufs_dirent with the name `name` in the root directory of the virtual disk. The tufs_dirent
 * is initially empty. The maximum length for a tufs_dirent name is 15 characters. There must not be more
 * than 64 files on the virtual disk.
 * @param name
 * @return On success, 0. On failure, -1. This could be because the tufs_dirent already exists, the tufs_dirent name
 * is too long, or when there are the maximum number of files on the system.
 */
int fs_create(char *name);

/**
 * Deletes a tufs_dirent with the name `name` from the virtual disk and frees all data blocks and meta-information
 * corresponding to this tufs_dirent. The tufs_dirent being deleted must not be open.
 * @param name
 * @return On success, 0. On failure, -1. This could be because the tufs_dirent does not exist, or when the
 * tufs_dirent to be deleted is currently open.
 */
int fs_delete(char *name);

/**
 * Read `nbyte` bytes of data from the tufs_dirent referenced by the descriptor into the buffer `buf`.
 * This function assumes `buf` is large enough to contain `nbyte` bytes.
 * <br><br>
 * This function implicitly increments the tufs_dirent pointer by the number of bytes that were actually read.
 * @param file_descriptor File descriptor to read from
 * @param buf Buffer to write data into
 * @param nbyte Number of bytes to read
 * @return Number of successful bytes read. This number could be smaller than `nbyte` when attempting to
 * read past the end of the tufs_dirent. In the case of failure, -1. This could be because the tufs_dirent descriptor is
 * invalid.
 */
int fs_read(tufs_fd_t file_descriptor, void *buf, size_t nbyte);

/**
 * Write `nbyte` bytes of data to the tufs_dirent referenced by the descriptor from the buffer `buf`.
 * This function assumes the buffer `buf` contains at least `nbyte` bytes.
 * <br>
 * When the function attempts to write past the end of the tufs_dirent, the tufs_dirent is automatically
 * extended to hold the additional bytes.
 * <br>
 * The maximum tufs_dirent size is 16 MB (4096 blocks, each 4 KB).
 * <br><br>
 * This function implicitly increments the tufs_dirent pointer by the number of bytes that were actually
 * written.
 * @param file_descriptor
 * @param buf
 * @param nbyte
 * @return Number of bytes that were actually written to the tufs_dirent. This number could be smaller
 * than the number of bytes specified by `nbyte` if the disk runs out of space. In the case of
 * failure, -1. This could be because the tufs_dirent descriptor is invalid.
 */
int fs_write(tufs_fd_t file_descriptor, void *buf, size_t nbyte);

/**
 * Get the current size of the tufs_dirent specified by `file_descriptor`
 * @param file_descriptor
 * @return The size, in bytes, of the tufs_dirent. -1 in the event the tufs_dirent descriptor is invalid.
 */
int fs_get_filesize(tufs_fd_t file_descriptor);

/**
 * Sets the tufs_dirent pointer associated with the tufs_dirent descriptor to the argument `offset`.
 * @param file_descriptor
 * @param offset Amount of bytes to offset from the <b>beginning of the tufs_dirent</b>
 * @return On success, 0. On failure, -1 if the fd  is invalid, when the offset is further than the
 * size of the tufs_dirent, or when the offset is less than zero.
 */
int fs_lseek(tufs_fd_t file_descriptor, tufs_off_t offset);

/**
 * Truncate the tufs_dirent associated with the tufs_dirent descriptor to `length` bytes. It is not possible to
 * extend a tufs_dirent using this function. If the tufs_dirent pointer associated with this tufs_dirent is set to a location
 * beyond the specified length, then the tufs_dirent pointer is reset to the new, truncated length of the tufs_dirent.
 * @param file_descriptor
 * @param length
 * @return On success, 0. On failure, -1 if the fd is invalid or the length is larger than the current
 * size of the tufs_dirent.
 */
int fs_truncate(tufs_fd_t file_descriptor, tufs_off_t length);

#endif //TUFSIMPL_TUFSIO_H
