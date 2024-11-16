//
// Created by Samir Buch on 11/14/24.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "tufsio.h"
#include "tufsdef.h"
#include "disk/disk.h"

// create a new file
int fs_create(char *name) {
    int physical_block;
    // Loop through the FAT table to find an empty block
    for (physical_block = 0; physical_block < FAT_SIZE; physical_block++) {
        if (p_fat->block_status[physical_block] == 0) {
            // Found an empty block!
            break;
        }
    }

    // If we're at the end of the FAT table, there are no empty blocks
    if (physical_block == FAT_SIZE) {
        perror("disk full");
        // No empty blocks found
        return TUFS_ERROR;
    }

    // Create the file struct
    file_t *new_file;
    void *p = malloc(sizeof(file_t));
    if (!p) {
        perror("malloc");
        free(p);
        return TUFS_ERROR;
    }

    new_file = (file_t *) p;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Set various attributes about the file
//    new_file->name = name;
    strncpy(new_file->name, name, 15);
    new_file->attribute = 0; // Unused. not worried about this being correct
    new_file->create_time = tm.tm_hour << 11 | tm.tm_min << 5 | tm.tm_sec / 2;
    new_file->create_date = (tm.tm_year + 1900) << 9 | (tm.tm_mon + 1) << 5 | tm.tm_mday;
    new_file->last_access_date = new_file->create_date;
    new_file->last_modified_time = new_file->create_time;
    new_file->starting_cluster = 4096 + physical_block;
    new_file->file_size = 0;

    // Now that we have the completed struct, copy it into the root directory.
    // We'll start at the beginning of the root directory and loop through until we find an empty entry.
    for(int i = 0; i < MAX_FILES; i++) {
        if(p_root->files[i].name[0] == '\0') {
            // Found an empty entry!
            memcpy(&p_root->files[i], new_file, sizeof(file_t));
            break;
        }
    }

    // If we're at the end of the root directory, there are no empty entries
    if (physical_block == MAX_FILES) {
        // No empty entries found
        perror("No empty entries found in root directory");
        free(p);
        return TUFS_ERROR;
    }

    // We can free the pointer storing the file data since we've copied it
    // to disk by this point.
    free(p);

    return TUFS_SUCCESS;

}