//
// Created by Samir Buch on 11/19/24.
//

#include <stdio.h>
#include "commands.h"
#include "util/logging/logging.h"
#include "libtufs.h"

int file_info(char **args) {
    if (!args[1]) {
        error("Missing file to get information of");
        return TUFS_ERROR;
    }

    tufs_fd_t fd = fs_open(args[1]);
    if (fd == TUFS_ERROR) {
        error("Failed to open file");
        return TUFS_ERROR;
    }

    file_t *file = file_table[fd];

    int size;
    if ((size = fs_get_filesize(fd)) == TUFS_ERROR) {
        error("Failed to get file size");
        return TUFS_ERROR;
    }

    printf("%s\n", args[1]);
    printf("- Size: %d bytes\n", size);
    printf("- Created at: %02d:%02d:%02d %d-%02d-%02d\n",
         file->create_time >> 11,
         (file->create_time & 0x07E0) >> 5,
         (file->create_time & 0x001F) * 2,
         (file->create_date >> 9) + 1920, // ???
         (file->create_date & 0x01E0) >> 5,
         (file->create_date & 0x001F)
    );
    printf("- Last modified: %02d:%02d:%02d %d-%02d-%02d\n",
         file->last_modified_time >> 11,
         (file->last_modified_time & 0x07E0) >> 5,
         (file->last_modified_time & 0x001F) * 2,
         (file->last_modified_date >> 9) + 1920,
         (file->last_modified_date & 0x01E0) >> 5,
         (file->last_modified_date & 0x001F)
    );
    printf("- Last accessed: %d-%d-%d\n",
         (file->last_access_date >> 9) + 1920, // again. wtf
         (file->last_access_date & 0x01E0) >> 5,
         (file->last_access_date & 0x001F)
    );
    printf("- Starting block: %d (0x%x)\n", file->starting_cluster, file->starting_cluster);
    printf("- Current pointer index: %d\n", file->data_ptr_idx);

    if (fs_close(fd) == TUFS_ERROR) {
        error("Failed to close file");
        return TUFS_ERROR;
    }

    return TUFS_SUCCESS;
}