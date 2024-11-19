//
// Created by Samir Buch on 11/19/24.
//

#include "commands.h"
#include "libtufs.h"
#include "util/logging/logging.h"

int rename_file(char **args) {
    if (!active) {
        error("No disk mounted");
        return TUFS_ERROR;
    }

    if (args[1] == NULL || args[2] == NULL) {
        error("Usage: rename <old_name> <new_name>");
        return TUFS_ERROR;
    }

    if (fs_rename(args[1], args[2]) == TUFS_ERROR) {
        error("Failed to rename file");
        return TUFS_ERROR;
    }

    success("Renamed file %s to %s", args[1], args[2]);

    return TUFS_SUCCESS;
}