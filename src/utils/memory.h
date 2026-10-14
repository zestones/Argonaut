/**
 * @file memory.h
 * @brief Checked memory-allocation helpers.
 */

#ifndef ARGONAUT_MEMORY_H
#define ARGONAUT_MEMORY_H

#include <stddef.h>

/**
 * @brief Allocates memory or terminates with a consistent diagnostic.
 *
 * @param size The number of bytes to allocate. A zero-sized request allocates one byte.
 * @return The allocated memory, which the caller must free.
 */
void *xmalloc(size_t size);

/**
 * @brief Resizes an allocation or terminates with a consistent diagnostic.
 *
 * @param pointer The allocation to resize, or NULL.
 * @param size The new size. A zero-sized request allocates one byte.
 * @return The resized allocation, which the caller must free.
 */
void *xrealloc(void *pointer, size_t size);

/**
 * @brief Duplicates a null-terminated string using checked allocation.
 *
 * @param text The string to duplicate.
 * @return The duplicated string, which the caller must free.
 */
char *xstrdup(const char *text);

#endif // ARGONAUT_MEMORY_H
