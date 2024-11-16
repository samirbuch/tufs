//
// Created by Samir Buch on 11/14/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "disk.h"
#include "tufsdef.h"

struct tufs_fat *p_fat = NULL;
struct tufs_root *p_root = NULL;

int mount_fs() {
    if(!handle || !active) {
        perror("disk not mounted");
        return TUFS_ERROR;
    }

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

    void *p = malloc(FAT_SIZE); // This is freed only when the disk is unmounted
    if(!p) {
        perror("malloc");
        free(p);
        return TUFS_ERROR;
    }
    p_fat = (struct tufs_fat *) p;

    if(block_read(bs->fat1_start, (char *) p_fat) == TUFS_ERROR) {
        perror("failed to read FAT");
        free(p_fat);
        free(bs);
        return TUFS_ERROR;
    }

    void *r = malloc(sizeof(struct tufs_root));
    if(!r) {
        perror("malloc");
        free(r);
        return TUFS_ERROR;
    }
    p_root = (struct tufs_root *) r;

    if(block_read(bs->root_start, (char *) p_root) == TUFS_ERROR) {
        perror("failed to read root directory");
        free(p_fat);
        free(p_root);
        free(bs);
        return TUFS_ERROR;
    }

    free(bs);

    return TUFS_SUCCESS;
}