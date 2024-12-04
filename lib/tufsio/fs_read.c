//
// Created by Samir Buch on 11/20/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tufsio.h"
#include "disk/disk.h"

int fs_read(tufs_fd_t file_descriptor, void *buf, size_t nbyte) {
    // Check if the file descriptor is valid
    if(file_descriptor < 0 || file_descriptor >= MAX_FILES) {
        fprintf(stderr, "Invalid file descriptor\n");
        return TUFS_ERROR;
    }

    // Check if the file is actually open
    file_t *file = file_table[file_descriptor];
    if(file == NULL) {
        fprintf(stderr, "File is not open\n");
        return TUFS_ERROR;
    }

    size_t bytes_left_in_file = file->file_size - file->data_ptr_idx;
    size_t bytes_to_read = (nbyte < bytes_left_in_file) ? nbyte : bytes_left_in_file;

    uint16_t data_start_offset = p_boot->data_start;

    // Get the file and its first block in the FAT
    uint16_t current_block = file->starting_cluster; // The data offset is applied when mounting

    byte *pointer_in_buf = (byte *) buf;
    size_t bytes_read = 0;

    byte *temp_buffer = calloc(BLOCK_SIZE, sizeof(char));
    if(!temp_buffer) {
        perror("malloc");
        return TUFS_ERROR;
    }

    // Start reading from wherever the file's current pointer is. (file->data_ptr_idx)
    // Dividing and flooring the file->data_ptr_idx by BLOCK_SIZE will give us the number of blocks
    // to advance before we find the block that contains the file's current pointer.
    // For example, if data_ptr_idx is 5000, and BLOCK_SIZE is 4096, then floor(5000 / 4096) = 1.
    // Hence, advance 1 block from file->starting_cluster to find the block that contains the file's
    // current pointer.
    uint32_t blocks_to_advance = file->data_ptr_idx / BLOCK_SIZE;
    tufs_off_t leftover_bytes_to_advance = file->data_ptr_idx % BLOCK_SIZE;

    for(uint32_t i = 0; i < blocks_to_advance; i++) {
        if(current_block == 0xFFFF) {
            fprintf(stderr, "Reached the end of the file before we expected.. this should never have happened!\n");
            free(temp_buffer);
            return TUFS_ERROR;
        }

        current_block = p_fat->table[current_block - data_start_offset];
    }

    if(bytes_to_read == 0) {
        free(temp_buffer);
        return TUFS_SUCCESS; // Nothing to read!
    }

    // While the current block's data is not 0xFFFF (aka we're not at the last block),
    while (bytes_read < bytes_to_read && current_block != 0xFFFF) {
        // Read the data at this block on the disk.
        if(block_read(current_block + data_start_offset, temp_buffer) == TUFS_ERROR) {
            fprintf(stderr, "Error reading block %d (0x%x)\n", current_block, current_block);
            free(temp_buffer);
            return TUFS_ERROR; // return early with an error
        }

        size_t bytes_available_in_block = BLOCK_SIZE;
        byte *source_ptr = temp_buffer;

        if(leftover_bytes_to_advance > 0) {
            source_ptr += leftover_bytes_to_advance;
            bytes_available_in_block -= leftover_bytes_to_advance;
            leftover_bytes_to_advance = 0;
        }

        size_t remaining_file_bytes = file->file_size - (file->data_ptr_idx + bytes_read);
        // Adjust bytes_available_in_block if it exceeds remaining_file_bytes
        if(bytes_available_in_block > remaining_file_bytes) {
            bytes_available_in_block = remaining_file_bytes;
        }

        // Default with the entire block to read.
        size_t bytes_to_copy = bytes_available_in_block;

        // Write to the pointer location in buf
        memcpy(pointer_in_buf, source_ptr, bytes_to_copy);
        pointer_in_buf += bytes_to_copy; // seek the pointer in the buf to the appropriate spot
        bytes_read += bytes_to_copy; // and increase the number of bytes we've read

        // Exit the loop if we've read all the bytes in this file
        if(bytes_read >= bytes_to_read || remaining_file_bytes == 0) {
            break;
        }

        // Advance the current block to the next block in the chain
        uint16_t next_block = p_fat->table[current_block];
        if(next_block == 0xFFFF) {
            // We've reached the end of the file
            break;
        }
        current_block = next_block;
    };

    // Advance the file's data pointer index by the number of bytes read
    file->data_ptr_idx += bytes_read;

    free(temp_buffer);

    return TUFS_SUCCESS;
}