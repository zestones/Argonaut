#ifndef __UTILS_H__
#define __UTILS_H__

#include "../semantic_analysis/semantic_checks.h"
#include "errors.h"

extern Error error;

#define NULL_VALUE -1

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
    
    sprintf(buffer, "%d", value);
    return buffer;
}

#endif // __UTILS_H__