//
// Created by Samir Buch on 11/14/24.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "disk.h"
#include "tufsio/tufsio.h"

struct tufs_fat *p_fat = NULL;
struct tufs_root *p_root = NULL;
struct boot_sector *p_boot = NULL;
file_t *file_table[MAX_FILES] = {0};

int mount_fs() {
    if (!handle || !active) {
        perror("disk not opened");
        return TUFS_ERROR;
    }

    struct boot_sector *bs = malloc(sizeof(struct boot_sector) >= BLOCK_SIZE ? sizeof(struct boot_sector) : BLOCK_SIZE);
    if (bs == NULL) {
        perror("malloc");
        return TUFS_ERROR;
    }

    if (block_read(0, (char *) bs) == TUFS_ERROR) {
        perror("failed to read boot sector");
        free(bs);
        return TUFS_ERROR;
    }

    p_boot = bs;

    void *p = malloc(sizeof(struct tufs_fat)); // This is freed only when the disk is unmounted
    if (!p) {
        perror("malloc");
        free(p);
        return TUFS_ERROR;
    }
    p_fat = (struct tufs_fat *) p;

    uint16_t num_blocks_to_read = ceil((double) sizeof(struct tufs_fat) / BLOCK_SIZE);
    for (int i = 0; i < num_blocks_to_read; i++) {
        if (block_read(bs->fat1_start + i, (char *) p_fat + i * BLOCK_SIZE) == TUFS_ERROR) {
//            perror("failed to read FAT");
            fprintf(stderr, "failed to read FAT block %d\n", bs->fat1_start + i);
            free(p_fat);
            free(p_root);
            free(bs);
            return TUFS_ERROR;
        }
    }

    void *r = malloc(sizeof(struct tufs_root) >= BLOCK_SIZE ? sizeof(struct tufs_root) : BLOCK_SIZE);
    if (!r) {
        perror("malloc");
        free(r);
        return TUFS_ERROR;
    }
    p_root = (struct tufs_root *) r;

    if (block_read(bs->root_start, (char *) p_root) == TUFS_ERROR) {
        perror("failed to read root directory");
        free(p_fat);
        free(p_root);
        free(bs);
        return TUFS_ERROR;
    }

    return TUFS_SUCCESS;
}