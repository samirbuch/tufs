//
// Created by Samir Buch on 11/20/24.
//

#include "tufsio.h"
#include "disk/disk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * Find a free block in the FAT table.
 * @return Block number of the free block. If no free blocks are found, return 0xFFFF.
 * Requires manual data offset application.
 */
uint16_t find_free_block();

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

    // Start writing from wherever the file's current pointer is. (file->data_ptr_idx)
    // Dividing and flooring the file->data_ptr_idx by BLOCK_SIZE will give us the number of blocks
    // to advance before we find the block that contains the file's current pointer.
    // For example, if data_ptr_idx is 5000, and BLOCK_SIZE is 4096, then floor(5000 / 4096) = 1.
    // Hence, advance 1 block from file->starting_cluster to find the block that contains the file's
    // current pointer.
    uint32_t blocks_to_advance = floor(file->data_ptr_idx / (double) BLOCK_SIZE);
    // We'll have to copy into the temporary buffer the entire contents of the block.
    // We'll write as much as we can into the current block, then assuming we need more space,
    // we'll find another free block in the FAT and write to that block. Repeat until
    // we reach nbyte.
    for (int i = 0; i < blocks_to_advance; i++) {
        current_block = p_fat->table[current_block - data_start_offset];
    }

    while (bytes_written < nbyte) {
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
            p_fat->table[current_block - data_start_offset] = 0xFFFF;
        }

        // Write to the pointer location in buf
        memcpy(temp_buffer, pointer_in_buf, bytes_to_copy);
        pointer_in_buf += bytes_to_copy; // seek the pointer in the buf to the appropriate spot
        bytes_written += bytes_to_copy; // and increase the number of bytes we've written

        // Write the data back to the block
        int s2 = block_write(current_block, temp_buffer);
        if (s2 == TUFS_ERROR) {
            fprintf(stderr, "Error writing block %d (0x%x)\n", current_block, current_block);
            free(temp_buffer);
            return TUFS_ERROR; // return early with an error
        }

        // If we've written all the bytes we need to write, we can break out of the loop
        if (bytes_written == nbyte) {
            break;
        }

        // If we haven't written all the bytes we need to write, we need to find another block
        // to write to. We'll find the next free block in the FAT.
        uint16_t next_block = find_free_block();
        if (next_block == 0xFFFF) {
            fprintf(stderr, "No free blocks available\n");
            free(temp_buffer);
            return TUFS_ERROR; // return early with an error
        }

        // Set the current block's value in the FAT to the next block
        p_fat->table[current_block - data_start_offset] = next_block;
        current_block = next_block;
    }

    // Now that we've written all we need to write, we can update the file's data pointer index
    // and the file's size.
    file->data_ptr_idx += nbyte;
    file->file_size += nbyte;

    // We can free the pointer storing the file data since we've copied it
    // to disk by this point.
    free(temp_buffer);

    return TUFS_SUCCESS;
}

uint16_t find_free_block() {
    int physical_block;
    // Loop through the FAT table to find an empty block
    for (physical_block = 0; physical_block < FAT_SIZE; physical_block++) {
        if (p_fat->block_status[physical_block] == EMPTY) {
            // Found an empty block!
            return physical_block;
        }
    }

    return 0xFFFF; // No empty blocks found
}