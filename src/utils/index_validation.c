#include "index_validation.h"

#include <stdio.h>
#include <stdlib.h>

#include "../../lib/colors.h"

void validate_table_index(const char *table_name, int index, int capacity) {
    if (index >= 0 && index < capacity) return;

    fprintf(stderr, COLOR_RED "<Error> %s index %d out of bounds (0 to %d).\n" COLOR_RESET, table_name, index,
            capacity - 1);
    exit(EXIT_FAILURE);
}
