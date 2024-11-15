//
// Created by Samir Buch on 11/8/24.
//

#ifndef TUFSIMPL_UTIL_H
#define TUFSIMPL_UTIL_H

#include "logging/logging.h"

/**
 * Returns the index of str in the array of strings arr
 * @param arr Array of strings, NULL terminated
 * @param str String to test for in arr
 * @return Index of str in arr, -1 if not found
 */
int index_of(char **arr, char *str);

/**
 * Break up `line` into individual strings, delimited by `delim`.
 * <br>
 * <br>
 * Original string must be heap-allocated, and the resulting strings and the entire
 * array must be freed when finished with use.
 * @param line
 * @param delim
 * @return
 */
char **parse(char *line, const char delim[]);

/**
 * Join an array of strings into a single string, separated by `delim`.
 * @param arr
 * @param delim
 * @return Allocated memory containing the joined string. Must be freed when finished!
 */
char *join(char **arr, const char delim[]);

#endif //TUFSIMPL_UTIL_H
