//
// Created by Samir Buch on 11/8/24.
//

#include <string.h>
#include "util.h"

int index_of(char **arr, char *str) {
    for (int i = 0; arr[i] != NULL; i++) {
        char *element = arr[i];
        if (strcmp(element, str) == 0)
            return i;
    }
    return -1;
}