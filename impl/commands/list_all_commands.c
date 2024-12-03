//
// Created by Samir Buch on 12/2/24.
//

#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include "tufsdef.h"
#include "../main.h" // for command_names
#include "../util/util.h"

int list_all_commands(char **args) {
    printf("Available commands:\n");

    char *joined = join(command_names, ", ");
    printf("%s\n", joined);
    free(joined);

    return TUFS_SUCCESS;
}