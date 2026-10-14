#include "../../../utils/utils.h"
#include "../../../utils/memory.h"
#include "format_specifiers.h"


char *strip_quotes(const char *raw_format) {
    size_t format_len = strlen(raw_format);

    if (format_len >= 2 && raw_format[0] == '"' && raw_format[format_len - 1] == '"') {
        size_t content_len = format_len - 2;
        char *format = xmalloc(content_len + 1);

        memcpy(format, raw_format + 1, content_len);
        format[content_len] = '\0';
        return format;
    }

    return NULL;
}

const char *find_next_format_specifier(const char *format) {
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && format[i + 1] == '%') {
            i++; // "%%" is a literal percent sign
        } else if (format[i] == '%' && is_format_specifier(format[i + 1])) {
            return format + i + 1;
        }
    }
    return NULL;
}

char get_nth_format_specifier(const char *format, int n) {
    const char *cursor = format;
    for (int count = 0; count <= n; count++) {
        const char *specifier = find_next_format_specifier(cursor);
        if (specifier == NULL) return '\0';
        if (count == n) return *specifier;
        cursor = specifier + 1;
    }
    return '\0';
}

int count_format_specifiers(const char *format) {
    int count = 0;
    const char *cursor = format;
    const char *specifier;
    while ((specifier = find_next_format_specifier(cursor)) != NULL) {
        count++;
        cursor = specifier + 1;
    }
    return count;
}

int is_format_specifier(char c) {
    return c == 'd' || c == 'f' || c == 's' || c == 'c' || c == 'b';
}

int is_compatible_with_format_specifier(int argument_type, char specifier) {
    switch (specifier) {
        case 'd': return argument_type == A_INTEGER_LITERAL || argument_type == A_BOOLEAN_LITERAL;
        case 'b': return argument_type == A_BOOLEAN_LITERAL;
        case 'f': return argument_type == A_FLOAT_LITERAL;
        case 's': return argument_type == STRING_LITERAL_TYPE;
        case 'c': return argument_type == A_CHARACTER_LITERAL;
        default: return 0;
    }
}

int count_argument_list(Node *argument_list) {
    int count = 0;
    Node *current = argument_list->child;
    while (current) {
        count++;
        current = current->sibling;
    }
    return count;
}
