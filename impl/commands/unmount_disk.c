//
// Created by Samir Buch on 11/14/24.
//

#include "commands.h"
#include "disk/disk.h"
#include "util/logging/logging.h"
#include "main.h"

int unmount_disk(char **args) {
    if(!active) {
        error("There's no disk mounted.");
        return TUFS_ERROR;
    }

    if(unmount_fs() == TUFS_ERROR) {
        error("There was an error unmounting the disk.");
        return TUFS_ERROR;
    }

    if(close_disk() == TUFS_ERROR) {
        error("There was an error closing the disk.");
        return TUFS_ERROR;
    }

    success("Unmounted disk.");
//    sprintf(prompt, "(tufs repl) > ");
    set_prompt("(tufs repl) > ");

    return TUFS_SUCCESS;
}