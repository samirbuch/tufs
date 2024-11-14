//
// Created by Samir Buch on 11/8/24.
//

#include <stdio.h>
#include <stdarg.h>

void info(const char *format, ...) {
    printf("\033[34minfo: ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\033[0m\n");
}

void success(const char *format, ...) {
    printf("\033[32msuccess: ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\033[0m\n");
}

void error(const char *format, ...) {
    printf("\033[31merror: ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\033[0m\n");
}
