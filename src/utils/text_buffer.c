#include "text_buffer.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_TEXT_BUFFER_CAPACITY 64

static void fail_text_buffer_overflow(void) {
    fprintf(stderr, "Error: Text buffer is too large\n");
    exit(EXIT_FAILURE);
}

static void reserve_text_buffer(TextBuffer *buffer, size_t required_capacity) {
    if (required_capacity <= buffer->capacity) return;

    size_t capacity = buffer->capacity ? buffer->capacity : INITIAL_TEXT_BUFFER_CAPACITY;
    while (capacity < required_capacity) {
        if (capacity > SIZE_MAX / 2) {
            capacity = required_capacity;
            break;
        }
        capacity *= 2;
    }

    char *data = realloc(buffer->data, capacity);
    if (data == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    buffer->data = data;
    buffer->capacity = capacity;
}

TextBuffer construct_text_buffer(void) {
    TextBuffer buffer = {NULL, 0, 0};
    return buffer;
}

void text_buffer_append_bytes(TextBuffer *buffer, const char *text, size_t text_length) {
    if (buffer->length == SIZE_MAX || text_length > SIZE_MAX - buffer->length - 1) fail_text_buffer_overflow();

    size_t required_capacity = buffer->length + text_length + 1;
    reserve_text_buffer(buffer, required_capacity);
    memcpy(buffer->data + buffer->length, text, text_length);
    buffer->length += text_length;
    buffer->data[buffer->length] = '\0';
}

void text_buffer_append(TextBuffer *buffer, const char *text) {
    text_buffer_append_bytes(buffer, text, strlen(text));
}

void text_buffer_append_character(TextBuffer *buffer, char character) {
    text_buffer_append_bytes(buffer, &character, 1);
}

char *release_text_buffer(TextBuffer *buffer) {
    if (buffer->data == NULL) text_buffer_append_bytes(buffer, "", 0);

    char *data = buffer->data;
    *buffer = construct_text_buffer();
    return data;
}

void destroy_text_buffer(TextBuffer *buffer) {
    free(buffer->data);
    *buffer = construct_text_buffer();
}
