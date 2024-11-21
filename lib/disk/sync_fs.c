//
// Created by Samir Buch on 11/16/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "disk.h"

int sync_fs() {
    // Write the FAT to disk
    if(block_write(p_boot->fat1_start, (char *) p_fat) == TUFS_ERROR) {
        perror("failed to write FAT");
        return TUFS_ERROR;
    }

    if(block_write(p_boot->fat2_start, (char *) p_fat) == TUFS_ERROR) {
        perror("failed to write FAT");
        return TUFS_ERROR;
    }

    // Write the root directory to disk
    if(block_write(p_boot->root_start, (char *) p_root) == TUFS_ERROR) {
        perror("failed to write root directory");
        return TUFS_ERROR;
    }

    return TUFS_SUCCESS;
}