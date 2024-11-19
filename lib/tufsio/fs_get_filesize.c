//
// Created by Samir Buch on 11/19/24.
//

#include <stdio.h>
#include "tufsio.h"
#include "disk/disk.h"

int fs_get_filesize(tufs_fd_t file_descriptor) {
    // Check if the disk is mounted
    if (!active) {
        perror("disk not mounted");
        return TUFS_ERROR;
    }

    // Check if the file descriptor is valid
    if (file_descriptor < 0 || file_descriptor >= MAX_FILES) {
        perror("invalid file descriptor");
        return TUFS_ERROR;
    }

    // Check if the file is open
    if (file_table[file_descriptor] == NULL) {
        perror("file not open");
        return TUFS_ERROR;
    }

    return file_table[file_descriptor]->file_size;
}