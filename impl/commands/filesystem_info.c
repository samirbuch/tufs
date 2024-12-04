//
// Created by Samir Buch on 11/19/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "commands.h"
#include "libtufs.h"
#include "util/logging/logging.h"

int filesystem_info(char **args) {
    if (!active) {
        error("No disk mounted");
        return TUFS_ERROR;
    }

    struct boot_sector *bs = malloc(sizeof(struct boot_sector) > BLOCK_SIZE ? sizeof(struct boot_sector) : BLOCK_SIZE);
    if (bs == NULL) {
        perror("malloc");
        return TUFS_ERROR;
    }

    if (block_read(0, (char *) bs) == TUFS_ERROR) {
        perror("failed to read boot sector");
        free(bs);
        return TUFS_ERROR;
    }

    printf("Disk information:\n");
    printf("Boot sector\n---------\n");
    printf("- FAT1 Start: %d (0x%x)\n",
         bs->fat1_start, bs->fat1_start * 4096);
    printf("- FAT2 Start: %d (0x%x)\n",
         bs->fat2_start, bs->fat2_start * 4096);
    printf("- Root Start: %d (0x%x)\n",
         bs->root_start, bs->root_start * 4096);
    printf("- Data Start: %d (0x%x)\n",
         bs->data_start, bs->data_start * 4096);

    free(bs);

    // Get the current open file descriptors
    printf("\nOpen files:\n---------\n");
    if(file_table[0] == NULL) {
        printf("(no open files)\n");
    } else {
        for (int i = 0; i < MAX_FILES; i++) {
            if (file_table[i] != NULL) {
                printf("- fd %d: %s\n", i, file_table[i]->name);
            }
        }
    }

    return TUFS_SUCCESS;
}