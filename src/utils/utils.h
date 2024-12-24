#ifndef __UTILS_H__
#define __UTILS_H__

#include "../semantic_analysis/semantic_checks.h"
#include "errors.h"

extern Error error;

#define NULL_VALUE -1

static inline const char *format_int(void *data) {
    static char buffer[32];
    int value = *(int *) data; 
    
    sprintf(buffer, "%d", value);
    return buffer;
}

static inline const char *format_float(void *data) {
    static char buffer[32];
    float value = *(float *) data;

    sprintf(buffer, "%f", value);
    return buffer;
}


#endif // __UTILS_H__