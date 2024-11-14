//
// Created by Samir Buch on 11/12/24.
//

#include <stdio.h>
#include "disk/disk.h"
#include "commands.h"
#include "util/util.h"

int mount_disk(char **args) {
    if(!args[1]) {
        error("No disk name provided");
        return -1;
    }

    open_disk(args[1]);

    mount_fs();

    printf("fat index 0: 0x%x\n", p_fat->table[0]);
    printf("fat index 1: 0x%x\n", p_fat->table[1]);

    return 0;
}