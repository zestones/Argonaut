#include "numeric_parser.h"

#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>

int parse_integer_text(const char *text, int *value) {
    if (text == NULL) return 0;

    char *end = NULL;
    errno = 0;
    long parsed = strtol(text, &end, 10);
    if (errno == ERANGE || end == text || *end != '\0' || parsed < INT_MIN || parsed > INT_MAX) return 0;

    if (value != NULL) *value = (int) parsed;
    return 1;
}

int parse_real_text(const char *text, double *value) {
    if (text == NULL) return 0;

    char *end = NULL;
    errno = 0;
    double parsed = strtod(text, &end);
    if (errno == ERANGE || end == text || *end != '\0' || !isfinite(parsed) || parsed < -FLT_MAX || parsed > FLT_MAX) {
        return 0;
    }

    if (value != NULL) *value = parsed;
    return 1;
}
