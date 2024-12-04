//
// Created by Samir Buch on 11/16/24.
//

#include <stdio.h>
#include <math.h>
#include "disk.h"

int sync_fs() {
    // Write the FAT to disk
    uint16_t num_blocks_to_write = ceil((double) sizeof(struct tufs_fat) / BLOCK_SIZE);
    for (int i = 0; i < num_blocks_to_write; i++) {
        if (block_write(p_boot->fat1_start + i, (char *) p_fat + i * BLOCK_SIZE) == TUFS_ERROR) {
            perror("failed to write FAT");
            return TUFS_ERROR;
        }

        if (block_write(p_boot->fat2_start + i, (char *) p_fat + i * BLOCK_SIZE) == TUFS_ERROR) {
            perror("failed to write FAT");
            return TUFS_ERROR;
        }
    }

    // Write the root directory to disk
    if(block_write(p_boot->root_start, (char *) p_root) == TUFS_ERROR) {
        perror("failed to write root directory");
        return TUFS_ERROR;
    }

    return TUFS_SUCCESS;
}