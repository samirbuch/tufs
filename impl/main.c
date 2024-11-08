//
// Created by Samir Buch on 11/7/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <string.h>
#include "disk.h"

char *rl_gets(const char *prompt);

int main(void) {
    while(1) {
        char *line = rl_gets("tufs> ");

        if(strcmp(line, "exit") == 0) {
            break;
        }
    }

    return EXIT_SUCCESS;
}

/* A static variable for holding the line. */
static char *line_read = (char *) NULL;

/* Read a string, and return a pointer to it.
   Returns NULL on EOF. */
char *rl_gets(const char *prompt) {
    /* If the buffer has already been allocated,
       return the memory to the free pool. */
    if (line_read) {
        free(line_read);
        line_read = (char *) NULL;
    }

    /* Get a line from the user. */
    line_read = readline(prompt);

    /* If the line has any text in it,
       save it on the history. */
    if (line_read && *line_read)
        add_history(line_read);

    return (line_read);
}