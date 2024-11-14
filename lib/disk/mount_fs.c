//
// Created by Samir Buch on 11/14/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "disk.h"
#include "tufsdef.h"

int mount_fs(struct tufs_fat *p_fat) {
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

    block_read(bs->fat1_start, (char *) p_fat);

    free(bs);

    return TUFS_SUCCESS;
}