#include "../../symbol_table/representation/representation_table.h" 
#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../type_system/type_system.h"
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"


static char get_nth_format_specifier(const char *format, int n) {
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

static int count_format_specifiers(const char *format) {
    int count = 0;
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && (format[i + 1] == 'd' || format[i + 1] == 'f' || format[i + 1] == 's')) {
            count++;
            i++;
        }
    }
    return count;
}

static int is_compatible_with_format_specifier(int argument_type, char specifier) {
    switch (specifier) {
        case 'd': return argument_type == A_INTEGER_LITERAL;
        case 'f': return argument_type == A_FLOAT_LITERAL;
        case 's': return argument_type == A_STRING_LITERAL || argument_type == A_CHARACTER_LITERAL;
        default:  return 0;
    }
}

static int count_argument_list(Node *argument_list) {
    int count = 0;
    Node *current = argument_list->child;
    while (current) {
        count++;
        current = current->sibling;
    }
    return count;
}

void check_print_proc_argument_list(Node *format_string, Node *argument_list) {
    // Step 1: Extract format specifiers from the format string
    const char *format = get_lexeme(format_string->index_lexicographic);
    int expected_count = count_format_specifiers(format);

    // Step 2: Validate argument count
    int actual_count = count_argument_list(argument_list);
    if (expected_count != actual_count) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Mismatch: Format string expects %d arguments, but %d were provided.", 
                          expected_count, actual_count);
        yerror(error);
        return;
    }

    // Step 3: Validate argument types
    Node *current_arg = argument_list->child;
    for (int i = 0; i < expected_count; ++i) {
        char specifier = get_nth_format_specifier(format, i);
        int argument_type = resolve_expression_type(current_arg->child);

        if (!is_compatible_with_format_specifier(argument_type, specifier)) {
            set_error_type(&error, TYPE_ERROR);
            set_error_message(&error, "Type mismatch: Argument %d has type '%s', but specifier '%c' requires a different type.",
                              i + 1, get_lexeme(argument_type), specifier);
            yerror(error);
            return;
        }

        current_arg = current_arg->sibling;
    }
}
