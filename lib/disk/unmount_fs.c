//
// Created by Samir Buch on 11/14/24.
//

#include <stdlib.h>
#include "disk.h"

int unmount_fs() {
    // TODO: Implement saving in-memory data to disk. For now, this is fine.

    free(p_fat);
    p_fat = NULL;

    return TUFS_SUCCESS;
}