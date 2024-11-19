//
// Created by Samir Buch on 11/19/24.
//

#include "commands.h"
#include "libtufs.h"
#include "util/logging/logging.h"

int remove_file(char **args) {
    if(!active) {
        error("No disk mounted");
        return TUFS_ERROR;
    }

    if(!args[1]) {
        error("Missing file to remove");
        return TUFS_ERROR;
    }

    for(int i = 1; args[i] != NULL; i++) {
        if(fs_delete(args[i]) == TUFS_ERROR) {
            error("Failed to remove file: %s", args[i]);
            return TUFS_ERROR;
        }

        success("Removed file: %s", args[i]);
    }

    return TUFS_SUCCESS;
}