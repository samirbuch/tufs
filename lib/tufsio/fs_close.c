//
// Created by Samir Buch on 11/19/24.
//

#include <stdio.h>
#include "tufsio.h"
#include "tufsdef.h"
#include "disk/disk.h"

int fs_close(tufs_fd_t fd) {
    // Check to make sure the fd is actually in the file table
    if (fd < 0 || fd >= MAX_FILES) {
        perror("invalid file descriptor");
        return TUFS_ERROR;
    }
    if (file_table[fd] == NULL) {
        perror("file not open");
        return TUFS_ERROR;
    }

    // Clear the file table entry
    file_table[fd] = NULL;

    return TUFS_SUCCESS;
}