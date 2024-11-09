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
typedef int fd_t;

/**
 * File offset
 */
typedef unsigned int off_t;

/**
 * Open a tufs_file on the virtual disk in RW mode. If successful, returns a non-negative integer.
 * Note that the same tufs_file can be opened multiple times. When this happens, multiple, independent
 * tufs_file descriptors are provided per open, up to a filesystem maximum of 32 tufs_file descriptors.
 * @param name File to open
 * @return Unique tufs_file descriptor for the open tufs_file. -1 on failure.
 */
fd_t fs_open(char *name);

/**
 * Close a tufs_file by FD. A closed tufs_file descriptor can no longer be used to access the corresponding
 * tufs_file on the virtual disk.
 * @param file_descriptor File descriptor to close
 * @return On success, 0. On failure, -1.
 */
int fs_close(fd_t file_descriptor);

/**
 * Create a new tufs_file with the name `name` in the root directory of the virtual disk. The tufs_file
 * is initially empty. The maximum length for a tufs_file name is 15 characters. There must not be more
 * than 64 files on the virtual disk.
 * @param name
 * @return On success, 0. On failure, -1. This could be because the tufs_file already exists, the tufs_file name
 * is too long, or when there are the maximum number of files on the system.
 */
int fs_create(char *name);

/**
 * Deletes a tufs_file with the name `name` from the virtual disk and frees all data blocks and meta-information
 * corresponding to this tufs_file. The tufs_file being deleted must not be open.
 * @param name
 * @return On success, 0. On failure, -1. This could be because the tufs_file does not exist, or when the
 * tufs_file to be deleted is currently open.
 */
int fs_delete(char *name);

/**
 * Read `nbyte` bytes of data from the tufs_file referenced by the descriptor into the buffer `buf`.
 * This function assumes `buf` is large enough to contain `nbyte` bytes.
 * <br><br>
 * This function implicitly increments the tufs_file pointer by the number of bytes that were actually read.
 * @param file_descriptor File descriptor to read from
 * @param buf Buffer to write data into
 * @param nbyte Number of bytes to read
 * @return Number of successful bytes read. This number could be smaller than `nbyte` when attempting to
 * read past the end of the tufs_file. In the case of failure, -1. This could be because the tufs_file descriptor is
 * invalid.
 */
int fs_read(fd_t file_descriptor, void *buf, size_t nbyte);

/**
 * Write `nbyte` bytes of data to the tufs_file referenced by the descriptor from the buffer `buf`.
 * This function assumes the buffer `buf` contains at least `nbyte` bytes.
 * <br>
 * When the function attempts to write past the end of the tufs_file, the tufs_file is automatically
 * extended to hold the additional bytes.
 * <br>
 * The maximum tufs_file size is 16 MB (4096 blocks, each 4 KB).
 * <br><br>
 * This function implicitly increments the tufs_file pointer by the number of bytes that were actually
 * written.
 * @param file_descriptor
 * @param buf
 * @param nbyte
 * @return Number of bytes that were actually written to the tufs_file. This number could be smaller
 * than the number of bytes specified by `nbyte` if the disk runs out of space. In the case of
 * failure, -1. This could be because the tufs_file descriptor is invalid.
 */
int fs_write(fd_t file_descriptor, void *buf, size_t nbyte);

/**
 * Get the current size of the tufs_file specified by `file_descriptor`
 * @param file_descriptor
 * @return The size, in bytes, of the tufs_file. -1 in the event the tufs_file descriptor is invalid.
 */
int fs_get_filesize(fd_t file_descriptor);

/**
 * Sets the tufs_file pointer associated with the tufs_file descriptor to the argument `offset`.
 * @param file_descriptor
 * @param offset Amount of bytes to offset from the <b>beginning of the tufs_file</b>
 * @return On success, 0. On failure, -1 if the fd  is invalid, when the offset is further than the
 * size of the tufs_file, or when the offset is less than zero.
 */
int fs_lseek(fd_t file_descriptor, off_t offset);

/**
 * Truncate the tufs_file associated with the tufs_file descriptor to `length` bytes. It is not possible to
 * extend a tufs_file using this function. If the tufs_file pointer associated with this tufs_file is set to a location
 * beyond the specified length, then the tufs_file pointer is reset to the new, truncated length of the tufs_file.
 * @param file_descriptor
 * @param length
 * @return On success, 0. On failure, -1 if the fd is invalid or the length is larger than the current
 * size of the tufs_file.
 */
int fs_truncate(fd_t file_descriptor, off_t length);

#endif //TUFSIMPL_TUFSIO_H
