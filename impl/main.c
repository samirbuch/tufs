//
// Created by Samir Buch on 11/7/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <string.h>
#include "disk/disk.h"
#include "commands/commands.h"

#include "util/util.h"
#include "main.h"

char *command_names[] = {
        "init",
        "open",
        NULL
};

int (*command_functions[])(char **args) = {
        new_disk,
        mount_disk,
        NULL
};

// cursed? cursed. probably shoulda used extern. tough shit
static char *prompt;

int main(void) {
    // Initial prompt
    prompt = "(tufs repl) > ";

    while (1) {
        // Read input with the current prompt
        char *input = readline(prompt);

        if (!input) {
            // Handle EOF (Ctrl+D) or read error
            printf("\nExiting...\n");
            break;
        }

        if (*input) {
            // Add non-empty input to history
            add_history(input);
        } else {
            free(input);
            continue;
        }

        char **args = parse(input, " ");
        char *command = args[0];

        // Process the input as needed
        int command_index = index_of(command_names, command);
        if(command_index != -1) {
            command_functions[command_index](args);
        } else {
            error("Command not found \"%s\"", command);
        }

        // Free the input buffer allocated by readline
        free(input);

        // Free each of the args created by the parse function
        // (each argument is malloc'd)
        for(int i = 0; args[i] != NULL; i++) {
            free(args[i]);
        }
        // and the array itself is malloc'd.
        free(args);
    }

    return 0;
}

char *get_prompt() {
    return prompt;
}

// cursed but hey
void set_prompt(char *p) {
    prompt = p;
}