//
// Created by Samir Buch on 11/14/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "disk.h"
#include "tufsdef.h"

struct tufs_fat *p_fat = NULL;

int mount_fs() {
    if(!handle || !active) {
        return TUFS_ERROR;
    }

    struct boot_sector *bs = malloc(sizeof(struct boot_sector));
    if(bs == NULL) {
        perror("malloc");
        return TUFS_ERROR;
    }

    block_read(0, (char *) bs);

    printf("fat1 loc %d\n", bs->fat1_start);
    printf("fat2 loc %d\n", bs->fat2_start);

    void *p = malloc(FAT_SIZE);
    if(!p) {
        perror("malloc");
        free(p);
        return TUFS_ERROR;
    }
    p_fat = (struct tufs_fat *) p;

    block_read(bs->fat1_start, (char *) p_fat);

    free(bs);

    return TUFS_SUCCESS;
}