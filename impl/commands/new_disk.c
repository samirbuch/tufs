//
// Created by Samir Buch on 11/8/24.
//

#include <stdio.h>
#include "commands.h"
#include "disk/disk.h"
#include "util/util.h"
#include "tufsdef.h"

int new_disk(char **args) {
    char *disk_name = args[1];

    if (!disk_name) {
        error("No disk name provided");
        return TUFS_ERROR;
    }

    printf("Creating disk\n");

    if(init_fs(disk_name) == TUFS_ERROR) {
        error("Couldn't create disk: %s", disk_name);
        return TUFS_ERROR;
    }

    success("Created and initialized disk: %s", disk_name);

    return TUFS_SUCCESS;
}