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

    uint16_t data_start_offset = p_boot->data_start;

    // Get the file and its first block in the FAT
    uint16_t current_block = file->starting_cluster; // The data offset is applied when mounting

    char *pointer_in_buf = (char *) buf;
    size_t bytes_read = 0;

    byte *temp_buffer = calloc(BLOCK_SIZE, sizeof(char));
    if(!temp_buffer) {
        perror("malloc");
        free(temp_buffer);
        return TUFS_ERROR;
    }

    // While the current block's data is not 0xFFFF (aka we're not at the last block),
    while(p_fat->table[current_block - data_start_offset] != 0xFFFF && bytes_read < nbyte) {
        // Read the data at this block on the disk.
        int s = block_read(current_block, temp_buffer);
        if(s == TUFS_ERROR) {
            fprintf(stderr, "Error reading block %d (0x%x)\n", current_block, current_block);
            free(temp_buffer);
            return TUFS_ERROR; // return early with an error
        }

        // Default with the entire block to read.
        size_t bytes_to_copy = BLOCK_SIZE;
        // If the number of bytes that were read + the number of bytes we want to copy
        // is larger than the total number of bytes requested, calculate how many bytes
        // are left over. This is a simple subtraction.
        if(bytes_read + bytes_to_copy > nbyte) {
            bytes_to_copy = nbyte - bytes_read;
        }

        // Write to the pointer location in buf
        memcpy(pointer_in_buf, temp_buffer, bytes_to_copy);
        pointer_in_buf += bytes_to_copy; // seek the pointer in the buf to the appropriate spot
        bytes_read += bytes_to_copy; // and increase the number of bytes we've read

        // Update the current block to point to the next block
        current_block = p_fat->table[current_block - data_start_offset];
    }

    free(temp_buffer);

    return TUFS_SUCCESS;
}