//
// Created by Samir Buch on 11/20/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "commands.h"
#include "libtufs.h"
#include "util/logging/logging.h"

int display_file_contents(char **args) {
    if(!active) {
        error("No disk mounted");
        return TUFS_ERROR;
    }

    char *filename = args[1];
    if(!filename) {
        error("Missing file name");
        return TUFS_ERROR;
    }

    tufs_fd_t fd;
    if((fd = fs_open(filename)) == TUFS_ERROR) {
        error("Failed to open file \"%s\"", filename);
        return TUFS_ERROR;
    }

    size_t filesize;
    if((filesize = fs_get_filesize(fd)) == TUFS_ERROR) {
        error("Failed to get the file size for \"%s\"", filename);
        return TUFS_ERROR;
    }

    byte *buffer = calloc(filesize, sizeof(byte));
    if(!buffer) {
        error("Failed to allocate memory for file buffer");
        free(buffer);
        return TUFS_ERROR;
    }

    if(fs_read(fd, buffer, filesize) == TUFS_ERROR) {
        error("Failed to read file \"%s\"", filename);
        free(buffer);
        return TUFS_ERROR;
    }

    if(fs_close(fd) == TUFS_ERROR) {
        error("Failed to close file \"%s\"", filename);
        free(buffer);
        return TUFS_ERROR;
    }

    printf("%s\n", buffer);

    free(buffer);

    return TUFS_SUCCESS;
}