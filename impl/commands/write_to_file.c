//
// Created by Samir Buch on 11/20/24.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "commands.h"
#include "libtufs.h"
#include "util/logging/logging.h"
#include "util/util.h"

int write_to_file(char **args) {
    if(!active) {
        error("No disk active");
        return TUFS_ERROR;
    }

    char *filename = args[1];
    if(!filename) {
        error("No filename provided. Usage: write <file> <...data>");
        return TUFS_ERROR;
    }

    char *joined = join(args + 2, " ");
    size_t len = strlen(joined);

    tufs_fd_t fd;
    if((fd = fs_open(filename)) == TUFS_ERROR) {
        perror("write");
        free(joined);
        return TUFS_ERROR;
    }

    if(fs_write(fd, joined, len) == TUFS_ERROR) {
        perror("write");
        free(joined);
        return TUFS_ERROR;
    }

    success("Wrote %d bytes to file \"%s\"", len, filename);
    free(joined);

    return TUFS_SUCCESS;
}