//
// Created by Samir Buch on 11/19/24.
//

#include <stdio.h>
#include <string.h>
#include "tufsio.h"
#include "disk/disk.h"

int fs_rename(char *old_name, char *new_name) {
    // Check if the disk is mounted
    if (!active) {
        perror("disk not mounted");
        return TUFS_ERROR;
    }

    // Check if the file exists
    int i = 0;
    for(; i < MAX_FILES; i++) {
        if(strcmp(p_root->files[i].name, old_name) == 0)
            break;
    }

    if(i == MAX_FILES) {
        perror("no such file or directory");
        return TUFS_ERROR;
    }

    // Check if the new name already exists
    for(int j = 0; j < MAX_FILES; j++) {
        if(strcmp(p_root->files[j].name, new_name) == 0) {
            perror("file already exists");
            return TUFS_ERROR;
        }
    }

    // Rename the file
    strncpy(p_root->files[i].name, new_name, 15);

    return TUFS_SUCCESS;
}