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
    NO_ERROR,
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    TYPE_ERROR,
    LEXICAL_ERROR,
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
    char message[500];   // The error message.
    char *filename;      // The name of the file where the error occurred.
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
Error construct_error(ErrorType type, int line, int column, const char *format, ...);

/**
 * @brief Prints a formatted warning message based on the provided Error struct.
 * 
 * @param error The Error struct containing warning details.
 */
void yywarn(const Error error);

/**
 * @brief Prints a formatted error message based on the provided Error struct and exits the program.
 * 
 * @param error The Error struct containing error details.
 */
void yerror(const Error error);

/**
 * @brief Returns a formatted string containing the current file, line, and column.
 * 
 * @return The formatted string containing the current file, line, and column.
 */
char* get_formatted_location();

/**
 * @brief Sets the error message of the provided Error struct.
 * 
 * @param error The Error struct to set the message for.
 * @param format The format string for the error message.
 * @param ... The arguments to be formatted into the error message.
 */
void set_error_message(Error *error, const char *format, ...);

/**
 * @brief Sets the error type of the provided Error struct.
 * 
 * @param error The Error struct to set the type for.
 * @param type The type of the error.
 */
void set_error_type(Error *error, ErrorType type);

#endif // __ERRORS_H__
