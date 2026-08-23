#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void fail_memory_allocation(void) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    exit(EXIT_FAILURE);
}

void *xmalloc(size_t size) {
    void *memory = malloc(size == 0 ? 1 : size);
    if (memory == NULL) fail_memory_allocation();
    return memory;
}

void *xrealloc(void *pointer, size_t size) {
    void *memory = realloc(pointer, size == 0 ? 1 : size);
    if (memory == NULL) fail_memory_allocation();
    return memory;
}

char *xstrdup(const char *text) {
    size_t size = strlen(text) + 1;
    char *copy = xmalloc(size);
    memcpy(copy, text, size);
    return copy;
}
