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

    // Set the file's size to the length
    file->file_size = length;

    // If the file's data pointer was greater than the length, reset it to the length
    if (file->data_ptr_idx > length) {
        file->data_ptr_idx = length;
    }

    return TUFS_SUCCESS;
}