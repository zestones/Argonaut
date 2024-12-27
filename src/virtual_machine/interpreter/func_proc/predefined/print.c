#include "../../../../semantic_analysis/function_checks/common/format_specifiers.h"
#include "../../../../symbol_table/lexeme/lexeme_table.h"

#include "../../../core/vm_cell.h"
#include "../../../core/execution.h"

#include "../../expression/expression.h"
#include "../func_proc.h"

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024


/**
 * @brief Processes a format string and fills a buffer with formatted argument values.
 *
 * This function iterates through the format string and resolves the arguments specified in the 
 * argument list. It supports the following format specifiers:
 * - `%d` for integers.
 * - `%f` for floating-point numbers.
 * - `%s` for strings.
 * - `%c` for characters.
 *
 * The function matches each format specifier with the corresponding argument and appends 
 * the formatted value to the buffer. If an argument is missing for a specifier or an invalid 
 * format specifier is encountered, an error is printed and the function returns early.
 *
 * @param format A pointer to the format string specifying the argument types and layout.
 * @param arg_list A pointer to the AST node representing the argument list.
 * @param buffer A pointer to the buffer where the formatted result will be stored.
 */
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
        printf("%s", process_special_chars(strip_quotes(get_lexeme(arg_list->index_lexicographic))));
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

    printf("%s", process_special_chars(buffer));
}