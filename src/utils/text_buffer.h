/**
 * @file text_buffer.h
 * @brief A growable buffer for building dynamically allocated strings.
 */

#ifndef ARGONAUT_TEXT_BUFFER_H
#define ARGONAUT_TEXT_BUFFER_H

#include <stddef.h>

/**
 * @brief A growable, null-terminated text buffer.
 */
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} TextBuffer;

/**
 * @brief Constructs an empty text buffer.
 *
 * @return A new buffer that allocates storage on its first append.
 */
TextBuffer construct_text_buffer(void);

/**
 * @brief Appends a null-terminated string to a text buffer.
 *
 * @param buffer The destination buffer.
 * @param text The string to append.
 */
void text_buffer_append(TextBuffer *buffer, const char *text);

/**
 * @brief Appends a fixed number of bytes to a text buffer.
 *
 * @param buffer The destination buffer.
 * @param text The bytes to append.
 * @param text_length The number of bytes to append.
 */
void text_buffer_append_bytes(TextBuffer *buffer, const char *text, size_t text_length);

/**
 * @brief Appends one character to a text buffer.
 *
 * @param buffer The destination buffer.
 * @param character The character to append.
 */
void text_buffer_append_character(TextBuffer *buffer, char character);

/**
 * @brief Transfers ownership of the built string to the caller and resets the buffer.
 *
 * @param buffer The buffer to release.
 * @return A null-terminated, malloc'ed string that the caller must free.
 */
char *release_text_buffer(TextBuffer *buffer);

/**
 * @brief Frees a text buffer's storage and leaves it empty and reusable.
 *
 * @param buffer The buffer to destroy.
 */
void destroy_text_buffer(TextBuffer *buffer);

#endif // ARGONAUT_TEXT_BUFFER_H
