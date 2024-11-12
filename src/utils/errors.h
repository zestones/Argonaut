#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../../lib/colors.h"

/**
 * @enum ErrorType
 * @brief Enumeration of error types.
 */
typedef enum {
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    TYPE_ERROR,
    INVALID_INPUT_ERROR
} ErrorType;

/**
 * @struct Error
 * @brief Structure to hold detailed error information.
 */
typedef struct {
    ErrorType type;      // The type of the error.
    int line;            // The line number where the error occurred.
    int column;          // The column number where the error occurred.
    char message[256];   // The error message.
} Error;

/**
 * @brief Initializes an Error struct with the given type, line, column, and formatted message.
 * 
 * @param error The Error struct to initialize.
 * @param type The type of the error.
 * @param line The line number where the error occurred.
 * @param column The column number where the error occurred.
 * @param format The format string (similar to printf).
 * @param ... The arguments for the format string.
 */
void init_error(Error *error, ErrorType type, int line, int column, const char *format, ...);

/**
 * @brief Prints a formatted warning message based on the provided Error struct.
 * 
 * @param error The Error struct containing warning details.
 */
void yywarn(const Error *error);

/**
 * @brief Prints a formatted error message based on the provided Error struct and exits the program.
 * 
 * @param error The Error struct containing error details.
 */
void yerror(const Error *error);

#endif // __ERRORS_H__
