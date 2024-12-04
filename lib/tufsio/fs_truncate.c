//
// Created by Samir Buch on 12/2/24.
//

#include <stdio.h>
#include "libtufs.h"

int fs_truncate(tufs_fd_t file_descriptor, tufs_off_t length) {
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

    // Check if the length is less than zero
    if (length < 0) {
        fprintf(stderr, "Length is less than zero\n");
        return TUFS_ERROR;
    }

    // Check if the length is further than the size of the file
    if (length > file->file_size) {
        fprintf(stderr, "Length is further than the size of the file\n");
        return TUFS_ERROR;
    }

    // Calculate the current number of blocks and the new number of blocks
    size_t current_blocks = (file->file_size + BLOCK_SIZE - 1) / BLOCK_SIZE; // Ceil division
    size_t new_blocks = (length + BLOCK_SIZE - 1) / BLOCK_SIZE;           // Ceil division

    // If the new size requires fewer blocks, free the excess blocks
    if (new_blocks < current_blocks) {
        uint16_t current_block = file->starting_cluster;

        // Traverse to the last block of the new size
        for (size_t i = 1; i < new_blocks && current_block != 0xFFFF; i++) {
            current_block = p_fat->table[current_block];
        }

        // If there are more blocks to free, unlink them from the chain
        if (current_block != 0xFFFF) {
            uint16_t next_block = p_fat->table[current_block];
            p_fat->table[current_block] = 0xFFFF; // Mark the end of the file

            // Free the remaining blocks
            while (next_block != 0xFFFF) {
                uint16_t temp = next_block;
                next_block = p_fat->table[next_block];
                p_fat->table[temp] = 0xFFFF; // Unlink the block
                p_fat->block_status[temp] = EMPTY; // Mark as free
            }
        }
    }

    // Set the file's size to the length
    file->file_size = length;

    // If the file's data pointer was greater than the length, reset it to the length
    if (file->data_ptr_idx > length) {
        file->data_ptr_idx = length;
    }

    return TUFS_SUCCESS;
}