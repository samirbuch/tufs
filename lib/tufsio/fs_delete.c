//
// Created by Samir Buch on 11/19/24.
//

#include <stdio.h>
#include <string.h>
#include "tufsio.h"
#include "disk/disk.h"

int fs_delete(char *name) {
    // Check if the file exists
    int i = 0;
    for(; i < MAX_FILES; i++) {
        if(strcmp(p_root->files[i].name, name) == 0)
            break;
    }

    if(i == MAX_FILES) {
        perror("no such file or directory");
        return TUFS_ERROR;
    }

    // Check if the file is open
    for(int j = 0; j < MAX_FILES; j++) {
        if(file_table[j] == &p_root->files[i]) {
            perror("file is in use");
            return TUFS_ERROR;
        }
    }

    // Clear the file from the root directory
    memset(&p_root->files[i], 0, sizeof(file_t));

    return 0;
}