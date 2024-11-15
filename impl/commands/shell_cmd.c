//
// Created by Samir Buch on 11/15/24.
//

#include <stdlib.h>
#include "commands.h"
#include "util/logging/logging.h"
#include "tufsdef.h"
#include "util/util.h"

int shell_cmd(char **args) {
    if (args[1] == NULL) {
        error("No command provided to shell");
        return TUFS_ERROR;
    }

    // Join the arguments into a single string
    char *command = join(args + 1, " ");

    // Execute the command in the shell
    int status = system(command);
    free(command);

    if (status == -1) {
        error("Error executing command in shell");
        return TUFS_ERROR;
    }

    return TUFS_SUCCESS;
}