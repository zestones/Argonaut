
#include "format_specifiers.h"

char get_nth_format_specifier(const char *format, int n) {
    int count = 0;
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && (format[i + 1] == 'd' || format[i + 1] == 'f' || format[i + 1] == 's')) {
            if (count == n) return format[i + 1];
            count++;
            i++;
        }
    }
    return '\0';
}

int count_format_specifiers(const char *format) {
    int count = 0;
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && (format[i + 1] == 'd' || format[i + 1] == 'f' || format[i + 1] == 's')) {
            count++;
            i++;
        }
    }
    return count;
}

int is_compatible_with_format_specifier(int argument_type, char specifier) {
    switch (specifier) {
        case 'd': return argument_type == A_INTEGER_LITERAL;
        case 'f': return argument_type == A_FLOAT_LITERAL;
        case 's': return argument_type == A_STRING_LITERAL || argument_type == A_CHARACTER_LITERAL;
        default:  return 0;
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