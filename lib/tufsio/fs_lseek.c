//
// Created by Samir Buch on 12/2/24.
//

#include <stdio.h>
#include "libtufs.h"

int fs_lseek(tufs_fd_t file_descriptor, tufs_off_t offset) {
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

    // Check if the offset is less than zero
    if (offset < 0) { // Technically this should be an unsigned comparison
        fprintf(stderr, "Offset is less than zero\n");
        return TUFS_ERROR;
    }

    // Check if the offset is further than the size of the file
    if (offset > file->file_size) {
        fprintf(stderr, "Offset is further than the size of the file\n");
        return TUFS_ERROR;
    }

    // Set the file's data pointer index to the offset
    file->data_ptr_idx = offset;

    return TUFS_SUCCESS;
}