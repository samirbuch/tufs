//
// Created by Samir Buch on 12/3/24.
//

#include <stdlib.h>
#include "commands.h"
#include "libtufs.h"
#include "util/logging/logging.h"

int truncate_file(char **args) {
    if (!active) {
        error("No disk mounted");
        return TUFS_ERROR;
    }

    if (args[1] == NULL) {
        error("Usage: truncate <file> <length>");
        return TUFS_ERROR;
    }

    if (args[2] == NULL) {
        error("Usage: truncate <file> <length>");
        return TUFS_ERROR;
    }

    // Open the file
    tufs_fd_t fd = fs_open(args[1]);
    if (fd == -1) {
        error("Failed to open file");
        return TUFS_ERROR;
    }

    // Get the length
    tufs_off_t length = strtol(args[2], NULL, 10);

    // Truncate the file
    if (fs_truncate(fd, length) == TUFS_ERROR) {
        error("Failed to truncate file");
        fs_close(fd);
        return TUFS_ERROR;
    }

    success("Truncated file \"%s\" to %d bytes", args[1], length);

    return TUFS_SUCCESS;
}