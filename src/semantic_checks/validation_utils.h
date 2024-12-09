#ifndef __VALIDATION_UTILS_H__
#define __VALIDATION_UTILS_H__

#include <stdio.h>
#include <stdlib.h>

#include "../utils/errors.h"
#include "../utils/utils.h"

/**
 * @brief Retrieves the string representation of a type.
 *
 * This function returns the string representation of a type based on the given type index.
 *
 * @param type The type index.
 * @return The string representation of the type.
 */
static inline char *get_type_string(int type) {
    switch (type) {
        case 0: return "int";
        case 1: return "float";
        case 2: return "bool";
        case 3: return "char";
        default: return "unknown";
    }
}

#endif // __VALIDATION_UTILS_H__