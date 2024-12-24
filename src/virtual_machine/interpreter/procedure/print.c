#include "../../../semantic_analysis/function_checks/common/format_specifiers.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../core/vm_cell.h"
#include "../../core/execution.h"

#include "../expression/expression.h"
#include "procedure.h"

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

static char *strip_quotes(const char *raw_format) {
    int format_len = strlen(raw_format);

    if (format_len >= 2 && raw_format[0] == '"' && raw_format[format_len - 1] == '"') {
        char *format = (char *)malloc(format_len - 1);

        strncpy(format, raw_format + 1, format_len - 2);
        format[format_len - 2] = '\0';
        return format;
    }

    return NULL;
}

static void process_format_string(const char *format, AST arg_list, char *buffer) {
    char *buffer_ptr = buffer;
    int format_idx = 0;
    int arg_index = 0;

    AST current_arg = arg_list->child;

    while (format[format_idx] != '\0') {
        if (format[format_idx] == '%' && 
           (format[format_idx + 1] == 'd' || format[format_idx + 1] == 'f' || format[format_idx + 1] == 's' || format[format_idx + 1] == 'c')) {
            if (current_arg == NULL) {
                printf("Error: Missing argument for format specifier '%c'.\n", format[format_idx + 1]);
                return;
            }

            char specifier = format[format_idx + 1];
            vm_cell cell = resolve_expression(current_arg->child);

            switch (specifier) {
                case 'd':
                    buffer_ptr += sprintf(buffer_ptr, "%d", cell.value.integer);
                    break;
                case 'f':
                    buffer_ptr += sprintf(buffer_ptr, "%f", cell.value.real);
                    break;
                case 's':
                    buffer_ptr += sprintf(buffer_ptr, "%s", strip_quotes(cell.value.string));
                    break;
                case 'c':
                    buffer_ptr += sprintf(buffer_ptr, "%c", cell.value.character);
                    break;
                default:
                    printf("Error: Unsupported format specifier '%c'.\n", specifier);
                    return;
            }

            current_arg = current_arg->sibling;
            arg_index++;
            format_idx += 2;
        } else if (format[format_idx] == '\\' && format[format_idx + 1] == 'n') {
            *buffer_ptr++ = '\n';
            format_idx += 2;
        } else {
            *buffer_ptr++ = format[format_idx++];
        }
    }

    *buffer_ptr = '\0';
}

void execute_print(AST ast) {
    if (ast == NULL || ast->child == NULL) return;

    AST arg_list = ast->child;
    AST format_node = arg_list->sibling;
    if (format_node == NULL) {
        printf("Error: Missing format string.\n");
        return;
    }

    const char *raw_format = get_lexeme(format_node->index_lexicographic);
    if (raw_format == NULL) {
        printf("Invalid format string.\n");
        return;
    }

    char *format = strip_quotes(raw_format);
    if (format == NULL) {
        printf("Malformed format string.\n");
        return;
    }

    int expected_arg_count = count_format_specifiers(format);
    int actual_arg_count = count_argument_list(arg_list);

    char buffer[BUFFER_SIZE] = {0};
    process_format_string(format, arg_list, buffer);

    printf("%s", buffer);
}


