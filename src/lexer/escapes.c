#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "escapes.h"

static char *allocate(size_t size) {
    char *buffer = malloc(size);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return buffer;
}

char *decode_escapes(const char *text, size_t length, const char **bad) {
    char *decoded = allocate(length + 1);
    size_t j = 0;

    for (size_t i = 0; i < length; i++) {
        if (text[i] != '\\') {
            decoded[j++] = text[i];
            continue;
        }

        char next = (i + 1 < length) ? text[i + 1] : '\0';
        switch (next) {
            case 'n': decoded[j++] = '\n'; break;
            case 't': decoded[j++] = '\t'; break;
            case 'r': decoded[j++] = '\r'; break;
            case '\\': decoded[j++] = '\\'; break;
            case '\'': decoded[j++] = '\''; break;
            case '"': decoded[j++] = '"'; break;
            default:
                if (bad != NULL) *bad = text + i;
                free(decoded);
                return NULL;
        }
        i++;
    }

    decoded[j] = '\0';
    return decoded;
}

char *escape_text(const char *text) {
    char *escaped = allocate(2 * strlen(text) + 1);
    size_t j = 0;

    for (const char *p = text; *p != '\0'; p++) {
        switch (*p) {
            case '\\':
                escaped[j++] = '\\';
                escaped[j++] = '\\';
                break;
            case '\n':
                escaped[j++] = '\\';
                escaped[j++] = 'n';
                break;
            case '\r':
                escaped[j++] = '\\';
                escaped[j++] = 'r';
                break;
            case '\t':
                escaped[j++] = '\\';
                escaped[j++] = 't';
                break;
            default: escaped[j++] = *p; break;
        }
    }

    escaped[j] = '\0';
    return escaped;
}

char *unescape_text(const char *text) {
    const char *bad = NULL;
    char *decoded = decode_escapes(text, strlen(text), &bad);
    if (decoded == NULL) {
        // Not produced by escape_text(): keep the text as it is.
        decoded = allocate(strlen(text) + 1);
        memcpy(decoded, text, strlen(text) + 1);
    }
    return decoded;
}
