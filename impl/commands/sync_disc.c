//
// Created by Samir Buch on 11/16/24.
//

#include "commands.h"
#include "libtufs.h"
#include "util/logging/logging.h"

int sync_disk(char **args) {
    if(sync_fs() == TUFS_ERROR) {
        error("There was an error syncing the disk.");
        return TUFS_ERROR;
    }

    success("Synced disk.");

    return TUFS_SUCCESS;
}