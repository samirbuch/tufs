//
// Created by Samir Buch on 11/20/24.
//

#include "tufsio.h"
#include "disk/disk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    // Get the LOGICAL starting block (to add data offset LATER)
    uint16_t current_block = file->starting_cluster;

    char *pointer_in_buf = (char *)buf;
    size_t bytes_written = 0;

    byte *temp_buffer = calloc(BLOCK_SIZE, sizeof(char));
    if (!temp_buffer) {
        perror("calloc");
        return TUFS_ERROR;
    }

    // Calculate blocks to advance and leftover bytes
    uint32_t blocks_to_advance = file->data_ptr_idx / BLOCK_SIZE;
    size_t leftover_bytes_to_advance = file->data_ptr_idx % BLOCK_SIZE;

    // Advance to the correct block
    for (uint32_t i = 0; i < blocks_to_advance; i++) {
        if (current_block == 0xFFFF) {
            // Need to allocate a new block
            uint16_t next_block = find_free_block();
            if (next_block == 0xFFFF) {
                fprintf(stderr, "No free blocks available\n");
                free(temp_buffer);
                return TUFS_ERROR;
            }
            p_fat->table[current_block] = next_block;
            current_block = next_block;
        } else {
            current_block = p_fat->table[current_block];
        }
    }

    // Start writing data
    while (bytes_written < nbyte) {
        // Read existing data from current block
        if (block_read(current_block + data_start_offset, temp_buffer) == TUFS_ERROR) {
            fprintf(stderr, "Error reading block %d (0x%x)\n", current_block + data_start_offset, current_block + data_start_offset);
            free(temp_buffer);
            return TUFS_ERROR;
        }

        // Initialize destination pointer and bytes available in block
        size_t bytes_available_in_block = BLOCK_SIZE;
        byte *dest_ptr = temp_buffer;

        // Handle leftover bytes to advance (only applies to the first block)
        if (leftover_bytes_to_advance > 0) {
            dest_ptr += leftover_bytes_to_advance;
            bytes_available_in_block -= leftover_bytes_to_advance;
            leftover_bytes_to_advance = 0;
        }

        // Determine how many bytes to copy
        size_t bytes_to_copy = bytes_available_in_block;
        if (bytes_to_copy > nbyte - bytes_written) {
            bytes_to_copy = nbyte - bytes_written;
        }

        // Copy data from pointer_in_buf to dest_ptr
        memcpy(dest_ptr, pointer_in_buf, bytes_to_copy);
        pointer_in_buf += bytes_to_copy;
        bytes_written += bytes_to_copy;

        // Write the data back to the block
        if (block_write(current_block + data_start_offset, temp_buffer) == TUFS_ERROR) {
            fprintf(stderr, "Error writing block %d (0x%x)\n", current_block + data_start_offset, current_block + data_start_offset);
            free(temp_buffer);
            return TUFS_ERROR;
        }

        p_fat->block_status[current_block] = USED;

        // If we've written all the bytes we need to write, set FAT entry to 0xFFFF
        if (bytes_written >= nbyte) {
            p_fat->table[current_block] = 0xFFFF;
            break;
        }

        // Get the next block in the chain
        uint16_t next_block = p_fat->table[current_block];

        // If there is no next block, allocate a new one
        if (next_block == 0xFFFF) {
            next_block = find_free_block();
            if (next_block == 0xFFFF) {
                fprintf(stderr, "No free blocks available\n");
                free(temp_buffer);
                return TUFS_ERROR;
            }
            p_fat->table[current_block] = next_block;
            p_fat->table[next_block] = 0xFFFF; // Initialize next block's FAT entry
        }

        current_block = next_block;
    }

    // Update the file's data pointer index and file size
    file->data_ptr_idx += bytes_written;
    if (file->data_ptr_idx > file->file_size) {
        file->file_size = file->data_ptr_idx;
    }

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