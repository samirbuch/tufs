//
// Created by Samir Buch on 11/16/24.
//

#include "libtufs.h"
#include "commands.h"
#include <stdio.h>
#include "util/logging/logging.h"

int list_files(char **args) {
    if(!active) {
        error("Disk is not mounted");
        return TUFS_ERROR;
    }

    for(int i = 0; i < MAX_FILES; i++) {
        if(p_root->files[i].name[0] != '\0') {
            printf("%s\n", p_root->files[i].name);
        }
    }

    return TUFS_SUCCESS;
}