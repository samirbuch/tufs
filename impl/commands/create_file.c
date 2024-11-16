//
// Created by Samir Buch on 11/15/24.
//

#include "tufsio/tufsio.h"
#include "commands.h"
#include "util/logging/logging.h"
#include "tufsdef.h"
#include "disk/disk.h"

int create_file(char **args) {
    if(!active) {
        error("There is no mounted disk.");
        return TUFS_ERROR;
    }

    if(!args[1]) {
        error("No file name provided");
        return TUFS_ERROR;
    }

    if(fs_create(args[1]) == TUFS_ERROR) {
        error("Error creating file \"%s\"", args[1]);
        return TUFS_ERROR;
    }

    success("Successfully created file: \"%s\"", args[1]);
    return TUFS_SUCCESS;
}