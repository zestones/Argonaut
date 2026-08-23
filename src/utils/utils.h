#ifndef __UTILS_H__
#define __UTILS_H__

#include "../semantic_analysis/semantic_checks.h"
#include "errors.h"

extern Error error;

#define NULL_VALUE -1

/**
 * @brief Type of a string literal in the type checker.
 *
 * Expression types are declaration indices (the base types int, float, bool and char are the
 * declarations 0 to 3, which the literal node types mirror). String literals have no
 * declaration, so they get a value that can never be a declaration index.
 */
#define STRING_LITERAL_TYPE -2

/**
 * @brief Formats an integer value as a string.
 * 
 * This function takes a pointer to an integer, converts it into a string representation,
 * and returns the formatted string. The result is stored in a static buffer, which means
 * subsequent calls will overwrite the previous result.
 * 
 * @param data A pointer to the integer value to be formatted.
 * 
 * @return A string representation of the integer value.
 * 
 * @note The function uses a static buffer, which means it is not thread-safe and should not
 *       be used in concurrent contexts where multiple calls could overwrite the buffer.
 */
static inline const char *format_int(void *data) {
    static char buffer[32];
    int value = *(int *) data;

    snprintf(buffer, sizeof(buffer), "%d", value);
    return buffer;
}

#endif // __UTILS_H__
