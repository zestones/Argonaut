#include "format_specifiers.h"


char *process_special_chars(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    int input_len = strlen(input);
    char *output = (char *)malloc(input_len + 1);
    char *output_ptr = output; 
    for (int i = 0; i < input_len; i++) {
        if (input[i] == '\\' && i + 1 < input_len) {
            // Handle escape sequences
            switch (input[i + 1]) {
                case 'n':
                    *output_ptr++ = '\n';
                    i++; // Skip the 'n'
                    break;
                case 't':
                    *output_ptr++ = '\t';
                    i++; // Skip the 't'
                    break;
                case '\\':
                    *output_ptr++ = '\\';
                    i++; // Skip the second '\'
                    break;
                case '"':
                    *output_ptr++ = '"';
                    i++; // Skip the '"'
                    break;
                default:
                    *output_ptr++ = input[i];
                    break;
            }
        } else {
            *output_ptr++ = input[i];
        }
    }

    *output_ptr = '\0';
    return output;
}

char *strip_quotes(const char *raw_format) {
    int format_len = strlen(raw_format);

    if (format_len >= 2 && raw_format[0] == '"' && raw_format[format_len - 1] == '"') {
        char *format = (char *)malloc(format_len - 1);

        strncpy(format, raw_format + 1, format_len - 2);
        format[format_len - 2] = '\0';
        return format;
    }

    return NULL;
}

char get_nth_format_specifier(const char *format, int n) {
    int count = 0;
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && (format[i + 1] == 'd' || format[i + 1] == 'f' || format[i + 1] == 's' || format[i + 1] == 'c')) {
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
        if (format[i] == '%' && (format[i + 1] == 'd' || format[i + 1] == 'f' || format[i + 1] == 's' || format[i + 1] == 'c')) {
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
        case 's': return argument_type == A_STRING_LITERAL;
        case 'c': return argument_type == A_CHARACTER_LITERAL;
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