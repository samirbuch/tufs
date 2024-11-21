//
// Created by Samir Buch on 11/20/24.
//

#include "tufsio.h"
#include "disk/disk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int fs_write(tufs_fd_t file_descriptor, void *buf, size_t nbyte) {
    // Check if the file descriptor is valid
    if (file_descriptor < 0 || file_descriptor >= MAX_FILES) {
        fprintf(stderr, "Invalid file descriptor\n");
        return TUFS_ERROR;
    }

    // Check if the file is actually open
    file_t *file = file_table[file_descriptor];
    if (file == NULL) {
        fprintf(stderr, "File is not open\n");
        return TUFS_ERROR;
    }

    uint16_t data_start_offset = p_boot->data_start;

    // Get the file and its first block in the FAT
    uint16_t current_block = file->starting_cluster; // The data offset is applied when mounting

    char *pointer_in_buf = (char *) buf;
    size_t bytes_written = 0;

    byte *temp_buffer = calloc(BLOCK_SIZE, sizeof(char));
    if (!temp_buffer) {
        perror("malloc");
        free(temp_buffer);
        return TUFS_ERROR;
    }

    // Start writing from wherever the file's current pointer is.
    // We'll have to copy into the temporary buffer the entire contents of the block.
    // We'll write as much as we can into the current block, then assuming we need more space,
    // we'll find another free block in the FAT and write to that block. Repeat until
    // we reach nbyte.

    // We first need to figure out *where* to write *from*. This is done by taking
    // a look at the file->data_pointer field. The only thing is, this is not really
    // great because it only tells us the position in the file, not the position on the disk,
    // or even which block to start writing from.
    // Unfortunately the only way I can think of to solve this is to traverse the blocks
    // allocated to this file, and use strstr to find if the data_pointer is in that block.
    // TODO: find a better way to figure this out.

    while(1) {
        byte *b = calloc(BLOCK_SIZE, sizeof(char));
        if (!b) {
            perror("malloc");
            free(b);
            return TUFS_ERROR;
        }

        int s = block_read(current_block, b);
        if (s == TUFS_ERROR) {
            fprintf(stderr, "Error reading block %d (0x%x)\n", current_block, current_block);
            free(b);
            return TUFS_ERROR; // return early with an error
        }

        if (strstr(b, file->data_pointer) != NULL) {
            // Found the block to start writing from
            break;
        }

        current_block = p_fat->table[current_block - data_start_offset];
        free(b);
    }

    // Once we have the block to start writing from, we can start actually writing.

    while(bytes_written < nbyte) {
        // Read the data at this block to the temporary buffer.
        int s = block_read(current_block, temp_buffer);
        if (s == TUFS_ERROR) {
            fprintf(stderr, "Error reading block %d (0x%x)\n", current_block, current_block);
            free(temp_buffer);
            return TUFS_ERROR; // return early with an error
        }

        // Default with the entire block to write.
        size_t bytes_to_copy = BLOCK_SIZE;
        // If the number of bytes that were written + the number of bytes we want to copy
        // is larger than the total number of bytes requested, calculate how many bytes
        // are left over. This is a simple subtraction.
        if (bytes_written + bytes_to_copy > nbyte) {
            bytes_to_copy = nbyte - bytes_written;

            // We know this is the last block. Set its value in the FAT to 0xFFFF.

        }


    }

    return TUFS_SUCCESS;
}