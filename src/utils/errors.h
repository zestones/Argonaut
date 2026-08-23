#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../../lib/colors.h"

#if defined(__GNUC__) || defined(__clang__)
#define ARGO_PRINTF_LIKE(format_index, first_argument) __attribute__((format(printf, format_index, first_argument)))
#define ARGO_NORETURN __attribute__((noreturn))
#else
#define ARGO_PRINTF_LIKE(format_index, first_argument)
#define ARGO_NORETURN
#endif

/**
 * @enum ErrorType
 * @brief Enumeration of error types.
 */
typedef enum {
    NO_ERROR,
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    TYPE_ERROR,
    LEXICAL_ERROR,
    RUN_TIME_ERROR,
} ErrorType;

/**
 * @struct Error
 * @brief Structure to hold detailed error information.
 */
typedef struct {
    ErrorType type;       // The type of the error.
    int line;             // The line number where the error occurred.
    int column;           // The column number where the error occurred.
    char message[500];    // The error message.
    const char *filename; // The name of the file where the error occurred.
} Error;

/**
 * @brief The global Error struct to hold the current error details.
 */
extern Error error;

/**
 * @brief Constructs an Error struct with the provided details.
 * 
 * @param type The type of the error.
 * @param line The line number where the error occurred.
 * @param column The column number where the error occurred.
 * @param format The format string for the error message.
 * @param ... The arguments to be formatted into the error message.
 * @return The constructed Error struct.
 */
Error construct_error(ErrorType type, int line, int column, const char *format, ...) ARGO_PRINTF_LIKE(4, 5);

/**
 * @brief Prints a formatted warning message based on the provided Error struct.
 * 
 * @param error The Error struct containing warning details.
 */
void yywarn(const Error diagnostic);

/**
 * @brief Prints a formatted error message based on the provided Error struct and exits the program.
 * 
 * @param error The Error struct containing error details.
 */
void yerror(const Error diagnostic);

/**
 * @brief Number of errors reported through yerror() so far.
 *
 * @return The error count (0 when the compilation is clean).
 */
int get_error_count(void);

/**
 * @brief Returns a formatted string containing the current file, line, and column.
 * 
 * @return The formatted string containing the current file, line, and column.
 */
char *get_formatted_location(void);

/**
 * @brief Sets the error message of the provided Error struct.
 * 
 * @param error The Error struct to set the message for.
 * @param format The format string for the error message.
 * @param ... The arguments to be formatted into the error message.
 */
void set_error_message(Error *target, const char *format, ...) ARGO_PRINTF_LIKE(2, 3);

/**
 * @brief Sets the error type of the provided Error struct.
 * 
 * @param error The Error struct to set the type for.
 * @param type The type of the error.
 */
void set_error_type(Error *target, ErrorType type);

/**
 * @brief Reports a run-time error and terminates execution.
 *
 * @param format The printf-like diagnostic format.
 * @param ... Values referenced by the format.
 */
void fatal_runtime_error(const char *format, ...) ARGO_PRINTF_LIKE(1, 2) ARGO_NORETURN;

/**
 * @brief Reports the fatal syntax diagnostic shared by argoc and argov.
 *
 * @param token The unexpected token text.
 * @param detail Optional parser detail; NULL omits the detail line.
 */
void fatal_syntax_error(const char *token, const char *detail) ARGO_NORETURN;

#endif // __ERRORS_H__
