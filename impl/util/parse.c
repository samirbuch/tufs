#include <stdlib.h>
#include <string.h>

/*
 * Credit to Gene Kwatny for the original parse function for use in the shell project.
 * Unfortunately, `strtok()` modifies the original string. I hate that. So I took the
 * liberty of rewriting the entire thing to use combinations of strspn and strcspn.
 *
 * :)
 */

char **parse(const char *line, const char delim[]) {
    char **array = NULL;
    int n = 0;
    const char *s = line;

    while (*s) {
        // Skip leading delimiters
        s += strspn(s, delim);

        if (*s == '\0') {
            break; // No more tokens
        }

        // Find length of the token
        size_t len = strcspn(s, delim);

        if (len > 0) {
            // Allocate memory for the array of pointers
            char **p = realloc(array, (n + 2) * sizeof(char *));
            if (p == NULL) {
                // Handle allocation failure
                goto error; // gross ugly gross yuck throwing up (it's fine shut up)
            }
            array = p;

            // Allocate memory for the token string (+1 for null terminator)
            char *token = malloc(len + 1);
            if (token == NULL) {
                // Handle allocation failure
                goto error;
            }

            // Copy the token into the allocated memory
            strncpy(token, s, len);
            token[len] = '\0'; // Null-terminate the string

            array[n++] = token; // Add the token to the array
            array[n] = NULL;    // Null-terminate the array

            s += len; // Move to the next character after the token
        }
    }

    if (array == NULL) {
        // If no tokens were found, create an empty array
        array = malloc(sizeof(char *));
        if (array == NULL) {
            return NULL; // Allocation failed
        }
        array[0] = NULL;
    }

    return array;

    // yuck goto label who fucking cares bro
    error:
    // Clean up in case of allocation failure
    if (array != NULL) {
        for (int i = 0; i < n; i++) {
            free(array[i]);
        }
        free(array);
    }
    return NULL;
}
