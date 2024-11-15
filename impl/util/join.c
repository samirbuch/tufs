//
// Created by Samir Buch on 11/15/24.
//

#include <string.h>
#include <stdlib.h>
#include "util.h"

char *join(char **arr, const char delim[]) {
    // Calculate the length of the final string
    int len = 0;
    for (int i = 0; arr[i] != NULL; i++) {
        len += strlen(arr[i]);
        len += strlen(delim);
    }

    // Allocate memory for the final string
    char *result = malloc(len + 1);
    if (!result) {
        return NULL;
    }

    // Copy the strings into the final string
    int offset = 0;
    for (int i = 0; arr[i] != NULL; i++) {
        strcpy(result + offset, arr[i]);
        offset += strlen(arr[i]);
        strcpy(result + offset, delim);
        offset += strlen(delim);
    }

    // Null-terminate the string
    result[len] = '\0';

    return result;
}