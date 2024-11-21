//
// Created by Samir Buch on 11/19/24.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "libtufs.h"
#include "util/logging/logging.h"

// A couple of different things need to happen here.
// 1. We need to read the file from the virtual filesystem.
// 2. If the file exists, write its data to a temporary file on the actual computer.
// 3. Open this file in an interactive editor (usually defined as $EDITOR), fallback nano.
// 4. Once the editor is closed, read the file back into memory.
// 5. Write the file back to the virtual filesystem.

int edit_file(char **args) {
    if (!active) {
        error("No disk mounted");
        return TUFS_ERROR;
    }

    if (args[1] == NULL) {
        error("Usage: edit <file>");
        return TUFS_ERROR;
    }

    // Open the file
    tufs_fd_t fd = fs_open(args[1]);
    if (fd == -1) {
        error("Failed to open file");
        return TUFS_ERROR;
    }

    // Read the file into memory

    // Find the file file_size
    int file_size = fs_get_filesize(fd);
    if (file_size == -1) {
        error("Failed to get file file_size");
        fs_close(fd);
        return TUFS_ERROR;
    }

    // Allocate enough memory for the file buffer
    char *buf = malloc(file_size);
    if (buf == NULL) {
        perror("malloc");
        fs_close(fd);
        return TUFS_ERROR;
    }

    // Read the file into the buffer
    if (fs_read(fd, buf, file_size) == TUFS_ERROR) {
        error("Failed to read file");
        free(buf);
        fs_close(fd);
        return TUFS_ERROR;
    }

    // Create a temporary file
    char *prefix = "/tmp";
    char *temp_file_path = calloc(strlen(prefix) + strlen(args[1]) + 2, sizeof(char));
    if (temp_file_path == NULL) {
        perror("calloc");
        free(buf);
        fs_close(fd);
        return TUFS_ERROR;
    }
    snprintf(temp_file_path, strlen(prefix) + strlen(args[1]) + 2, "%s/%s", prefix, args[1]);

    FILE* temp_file = fopen(temp_file_path, "w");
    if (temp_file == NULL) {
        perror("fopen");
        free(buf);
        free(temp_file_path);
        fs_close(fd);
        return TUFS_ERROR;
    }

    // Write the file to the temporary file
    if (fwrite(buf, 1, file_size, temp_file) != file_size) {
        perror("fwrite");
        free(buf);
        fs_close(fd);
        return TUFS_ERROR;
    }

    // Open the temporary file in an editor
    const char *editor = getenv("EDITOR");
    if (editor == NULL) {
        editor = "nano"; // shut up I prefer nano over vim idc
        // sorry I don't feel like typing gibberish just to quit the damn session?
        // "!wq" "editor mode" "visual mode" <---- statements dreamed up by the utterly deranged
    }

    char command[256];
    snprintf(command, 256, "%s %s", editor, temp_file_path);
    int status = system(command);
    if (status != 0) {
        error("Failed to open editor");
        free(buf);
        free(temp_file_path);
        fs_close(fd);
        return TUFS_ERROR;
    }

    // Get the size of the temp file after we're finished writing to it
    fseek(temp_file, 0, SEEK_END);


    fclose(temp_file);

    return TUFS_SUCCESS;

}