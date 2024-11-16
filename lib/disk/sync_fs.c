//
// Created by Samir Buch on 11/16/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "disk.h"

int sync_fs() {
    struct boot_sector *bs = malloc(sizeof(struct boot_sector));
    if(bs == NULL) {
        perror("malloc");
        return TUFS_ERROR;
    }

    if(block_read(0, (char *) bs) == TUFS_ERROR) {
        perror("failed to read boot sector");
        free(bs);
        return TUFS_ERROR;
    }

    // Write the FAT to disk
    if(block_write(bs->fat1_start, (char *) p_fat) == TUFS_ERROR) {
        perror("failed to write FAT");
        free(bs);
        return TUFS_ERROR;
    }

    if(block_write(bs->fat2_start, (char *) p_fat) == TUFS_ERROR) {
        perror("failed to write FAT");
        free(bs);
        return TUFS_ERROR;
    }

    // Write the root directory to disk
    if(block_write(bs->root_start, (char *) p_root) == TUFS_ERROR) {
        perror("failed to write root directory");
        free(bs);
        return TUFS_ERROR;
    }

    free(bs);

    return TUFS_SUCCESS;
}