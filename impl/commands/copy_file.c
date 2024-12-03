//
// Created by Samir Buch on 12/3/24.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "commands.h"
#include "libtufs.h"
#include "util/logging/logging.h"

int internal_to_internal(char *source, char *dest);

int external_to_internal(char *source, char *dest);

int internal_to_external(char *source, char *dest);

int copy_file(char **args) {
    if (!active) {
        error("No disk active");
        return TUFS_ERROR;
    }

    if (!args[1]) {
        error("Usage: copy [-ei/ie] <source> <destination>");
        return TUFS_ERROR;
    }

    char *mode = args[1];
    char *source = args[2];
    char *destination = args[3];

    bool ei_flag = strncmp(mode, "-ei", 3) == 0;
    bool ie_flag = strncmp(mode, "-ie", 3) == 0;

    if(!ei_flag && !ie_flag) {
        // No mode was specified. We can assume the user wants to copy one virtual file
        // to another virtual file.

        source = args[1];
        destination = args[2];
        return internal_to_internal(source, destination);
    } else {
        if(ei_flag) {
            return external_to_internal(source, destination);
        } else {
            return internal_to_external(source, destination);
        }
    }

    // Final return statement not necessary.
    // Each function will return TUFS_SUCCESS or TUFS_ERROR.

}

int internal_to_internal(char *source, char *destination) {
    tufs_fd_t source_fd;
    tufs_fd_t dest_fd; // must not already exist

    if((source_fd = fs_open(source)) == TUFS_ERROR) {
        perror("copy");
        return TUFS_ERROR;
    }

    // We need this to error because we don't want to overwrite an existing file
    if((dest_fd = fs_open(destination)) != TUFS_ERROR) {
        error("Destination file already exists");
        fs_close(source_fd);
        fs_close(dest_fd);
        return TUFS_ERROR;
    }
    info("^ This was supposed to show an error message. Don't worry!");

    if(fs_create(destination) == TUFS_ERROR) {
        perror("copy");
        fs_close(source_fd);
        return TUFS_ERROR;
    }

    if((dest_fd = fs_open(destination)) == TUFS_ERROR) {
        perror("copy");
        fs_close(source_fd);
        return TUFS_ERROR;
    }

    int size = fs_get_filesize(source_fd);
    if(size == TUFS_ERROR) {
        perror("copy");
        fs_close(source_fd);
        fs_close(dest_fd);
        return TUFS_ERROR;
    }

    byte *buf = malloc(size);
    if(!buf) {
        perror("copy");
        fs_close(source_fd);
        fs_close(dest_fd);
        free(buf);
        return TUFS_ERROR;
    }

    if(fs_read(source_fd, buf, size) == TUFS_ERROR) {
        perror("copy");
        fs_close(source_fd);
        fs_close(dest_fd);
        free(buf);
        return TUFS_ERROR;
    }

    if(fs_write(dest_fd, buf, size) == TUFS_ERROR) {
        perror("copy");
        fs_close(source_fd);
        fs_close(dest_fd);
        free(buf);
        return TUFS_ERROR;
    }

    fs_close(source_fd);
    fs_close(dest_fd);
    free(buf);

    success("Copied %d bytes from \"%s\" to \"%s\"", size, source, destination);

    return TUFS_SUCCESS;
}

int external_to_internal(char *source, char *destination) {
    FILE *file = fopen(source, "r");
    if(!file) {
        perror("copy");
        return TUFS_ERROR;
    }

    tufs_fd_t dest_fd;
    if((dest_fd = fs_open(destination)) != TUFS_ERROR) {
        error("Destination file already exists");
        fclose(file);
        fs_close(dest_fd);
        return TUFS_ERROR;
    }
    info("^ This was supposed to show an error message. Don't worry!");

    if(fs_create(destination) == TUFS_ERROR) {
        perror("copy");
        fclose(file);
        return TUFS_ERROR;
    }

    if((dest_fd = fs_open(destination)) == TUFS_ERROR) {
        perror("copy");
        fclose(file);
        return TUFS_ERROR;
    }

    // Seek all the way to the end of the file to get the size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate a buffer to read the file into
    byte *buf = malloc(size);
    if(!buf) {
        perror("copy");
        fclose(file);
        fs_close(dest_fd);
        free(buf);
        return TUFS_ERROR;
    }

    // Actually read the file data into the buffer
    if(fread(buf, 1, size, file) != size) {
        perror("copy");
        fclose(file);
        fs_close(dest_fd);
        free(buf);
        return TUFS_ERROR;
    }

    // Write the buffer to the virtual disk
    if(fs_write(dest_fd, buf, size) == TUFS_ERROR) {
        perror("copy");
        fclose(file);
        fs_close(dest_fd);
        free(buf);
        return TUFS_ERROR;
    }

    fclose(file);
    fs_close(dest_fd);
    free(buf);

    success("Copied %d bytes from \"%s\" to \"%s\"", size, source, destination);

    return TUFS_SUCCESS;
}

int internal_to_external(char *source, char *destination) {
    tufs_fd_t source_fd;
    FILE *file = fopen(destination, "w");
    if(!file) {
        perror("copy");
        return TUFS_ERROR;
    }

    if((source_fd = fs_open(source)) == TUFS_ERROR) {
        perror("copy");
        fclose(file);
        return TUFS_ERROR;
    }

    int size = fs_get_filesize(source_fd);
    if(size == TUFS_ERROR) {
        perror("copy");
        fclose(file);
        fs_close(source_fd);
        return TUFS_ERROR;
    }

    // Allocate a buffer to read the file into
    byte *buf = malloc(size);
    if(!buf) {
        perror("copy");
        fclose(file);
        fs_close(source_fd);
        free(buf);
        return TUFS_ERROR;
    }

    // Actually read the file data into the buffer
    if(fs_read(source_fd, buf, size) == TUFS_ERROR) {
        perror("copy");
        fclose(file);
        fs_close(source_fd);
        free(buf);
        return TUFS_ERROR;
    }

    // Write the buffer to the file
    if(fwrite(buf, 1, size, file) != size) {
        perror("copy");
        fclose(file);
        fs_close(source_fd);
        free(buf);
        return TUFS_ERROR;
    }

    fclose(file);
    fs_close(source_fd);
    free(buf);

    success("Copied %d bytes from \"%s\" to \"%s\"", size, source, destination);

    return TUFS_SUCCESS;
}