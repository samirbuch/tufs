//
// Created by Samir Buch on 11/8/24.
//

#ifndef TUFSIMPL_LOGGING_H
#define TUFSIMPL_LOGGING_H

/**
 * Log info to the terminal. printf formatting
 */
void info(const char *format, ...);

/**
 * Log success message to the terminal. printf formatting.
 */
void success(const char *format, ...);

/**
 * Log error info to the terminal. printf formatting
 */
void error(const char *format, ...);

#endif //TUFSIMPL_LOGGING_H
