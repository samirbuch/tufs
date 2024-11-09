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
        }

        // Process the input as needed
        printf("You entered: \"%s\"\n", input);

        // Example: Change the prompt based on user input
        if (strcmp(input, "change") == 0) {
            set_prompt("changed_prompt> ");
        }

        // Free the input buffer allocated by readline
        free(input);
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