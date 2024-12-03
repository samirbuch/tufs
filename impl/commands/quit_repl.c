//
// Created by Samir Buch on 11/15/24.
//

#include <stdlib.h>
#include "commands.h"
#include "util/logging/logging.h"
#include "libtufs.h"

int quit_repl(char **args) {
    if (active) {
        info("Unmounting...");
        if (unmount_fs() == TUFS_ERROR) {
            error("There was an error unmounting the disk. You may experience data loss if you quit forcefully.");
            return TUFS_ERROR;
        }

        close_disk();
    }

    info("Quitting TUFS REPL...");

    exit(0);
}