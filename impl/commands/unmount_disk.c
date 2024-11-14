//
// Created by Samir Buch on 11/14/24.
//

#include "commands.h"
#include "disk/disk.h"
#include "util/logging/logging.h"
#include "main.h"

int unmount_disk(char **args) {
    if(unmount_fs() == TUFS_ERROR) {
        error("There was an error unmounting the disk.");
        return TUFS_ERROR;
    }

    success("Unmounted disk.");
//    sprintf(prompt, "(tufs repl) > ");
    set_prompt("(tufs repl) > ");

    return TUFS_SUCCESS;
}