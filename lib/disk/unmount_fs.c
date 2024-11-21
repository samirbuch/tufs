//
// Created by Samir Buch on 11/14/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "disk.h"

int unmount_fs() {
    if(!handle || !active) {
        perror("disk not mounted");
        return TUFS_ERROR;
    }

    // Sync changes in memory to disk
    if(sync_fs() == TUFS_ERROR) {
        perror("failed to sync disk");
        return TUFS_ERROR;
    }

    free(p_fat);
    p_fat = NULL;
    free(p_root);
    p_root = NULL;
    free(p_boot);
    p_boot = NULL;
    // Zero out the file table entries
    for(int i = 0; i < MAX_FILES; i++) {
        if(file_table[i] != NULL) {
            free(file_table[i]);
            file_table[i] = NULL;
        }
    }

    return TUFS_SUCCESS;
}