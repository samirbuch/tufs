//
// Created by Samir Buch on 11/19/24.
//

#include <stdio.h>
#include <string.h>
#include "tufsio.h"
#include "disk/disk.h"

tufs_fd_t fs_open(char *name) {
    // Check if the disk is mounted
    if (!active) {
        perror("disk not mounted");
        return TUFS_ERROR;
    }

    // Look for the file in the root directory
    int i = 0;
    for(; i < MAX_FILES; i++) {
        if(strcmp(p_root->files[i].name, name) == 0)
            break;
    }

    if(i == MAX_FILES) {
        perror("no such file or directory");
        return TUFS_ERROR;
    }

    // Look for the lowest available index in the file table
    int fd = 0;
    for(; fd < MAX_FILES; fd++) {
        if(file_table[fd] == NULL)
            break;
    }

    if(fd == MAX_FILES) {
        perror("too many files open");
        return TUFS_ERROR;
    }

    // Copy the file into the file table
    file_table[fd] = &p_root->files[i];

    return fd;
}