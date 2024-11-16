//
// Created by Samir Buch on 11/14/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "disk.h"

int unmount_fs() {
    // TODO: Implement saving in-memory data to disk. For now, this is fine.

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

    return TUFS_SUCCESS;
}