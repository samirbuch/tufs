//
// Created by Samir Buch on 11/12/24.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "disk/disk.h"
#include "commands.h"
#include "util/util.h"
#include "main.h"

int mount_disk(char **args) {
    if(!args[1]) {
        error("No disk name provided");
        return -1;
    }

    open_disk(args[1]);

    if(mount_fs() == TUFS_ERROR) {
        error("Error mounting disk \"%s\"", args[1]);
        return -1;
    }

    printf("fat index 0: 0x%x\n", p_fat->table[0]);
    printf("fat index 1: 0x%x\n", p_fat->table[1]);

//    sprintf(prompt, "(%s) > ", args[1]);
    char *p_str = calloc(64, sizeof(char));
    snprintf(p_str, 64, "(%s) > ", args[1]);
    set_prompt(p_str);
    free(p_str);

    success("Successfully mounted disk: \"%s\"", args[1]);

    return 0;
}