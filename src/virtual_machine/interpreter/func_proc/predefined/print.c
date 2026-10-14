#include "../../../../semantic_analysis/function_checks/common/format_specifiers.h"
#include "../../../../symbol_table/lexeme/lexeme_table.h"
#include "../../../../utils/text_buffer.h"

#include "../../../core/vm_cell.h"
#include "../../../core/execution.h"

#include "../../expression/expression.h"
#include "../func_proc.h"

#include <stdio.h>
#include <stdlib.h>

static void fail_invalid_format(void) {
    fprintf(stderr, "Error: Invalid print format in loaded IR\n");
    exit(EXIT_FAILURE);
}

static void append_formatted_real(TextBuffer *buffer, double value) {
    char scratch[64] = {0};
    int length = snprintf(scratch, sizeof(scratch), "%f", value);
    if (length < 0 || (size_t) length >= sizeof(scratch)) fail_invalid_format();
    text_buffer_append_bytes(buffer, scratch, (size_t) length);
}


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
static void process_format_string(const char *format, AST arg_list, TextBuffer *buffer) {
    size_t format_idx = 0;

    AST current_arg = arg_list->child;

    while (format[format_idx] != '\0') {
        if (format[format_idx] == '%' && format[format_idx + 1] == '%') {
            text_buffer_append(buffer, "%");
            format_idx += 2;
            continue;
        }

        if (format[format_idx] == '%' && is_format_specifier(format[format_idx + 1])) {
            char specifier = format[format_idx + 1];
            if (current_arg == NULL || current_arg->child == NULL) fail_invalid_format();
            vm_cell cell = resolve_expression(current_arg->child);

            switch (specifier) {
                case 'd': {
                    char scratch[64] = {0};
                    int value = cell.type == BOOLEAN ? cell.value.boolean : cell.value.integer;
                    int length = snprintf(scratch, sizeof(scratch), "%d", value);
                    if (length < 0 || (size_t) length >= sizeof(scratch)) fail_invalid_format();
                    text_buffer_append_bytes(buffer, scratch, (size_t) length);
                    break;
                }
                case 'b': text_buffer_append(buffer, cell.value.boolean ? "true" : "false"); break;
                case 'f': append_formatted_real(buffer, cell.value.real); break;
                case 's': {
                    if (cell.value.string == NULL) fail_invalid_format();
                    char *text = strip_quotes(cell.value.string);
                    text_buffer_append(buffer, text ? text : cell.value.string);
                    free(text);
                    break;
                }
                case 'c':
                    text_buffer_append_character(buffer, cell.value.character);
                    break;
                    // No default case as the format is already checked for 'd', 'f', 's', or 'c'
            }

            current_arg = current_arg->sibling;
            format_idx += 2;
        } else {
            text_buffer_append_character(buffer, format[format_idx++]);
        }
    }
}

void execute_print(AST ast) {
    if (ast == NULL || ast->child == NULL) return;

    AST arg_list = ast->child;
    AST format_node = arg_list->sibling;
    if (format_node == NULL) {
        char *text = strip_quotes(get_lexeme(arg_list->index_lexicographic));
        if (text == NULL) fail_invalid_format();
        TextBuffer buffer = construct_text_buffer();
        Node *no_arguments = construct_node_default(A_ARGUMENT_LIST);
        process_format_string(text, no_arguments, &buffer);
        char *output = release_text_buffer(&buffer);
        fprintf(stdout, "%s", output);
        free_ast(no_arguments);
        free(output);
        free(text);
        return;
    }

    const char *raw_format = get_lexeme(format_node->index_lexicographic);
    char *format = strip_quotes(raw_format);
    if (format == NULL) fail_invalid_format();

    // Note: No validation checks are performed on the 'raw_format' string or the processed 'format' string
    // at this stage. It is assumed that these strings have already been validated by the compiler. If the
    // compiler detected any errors in the format string, the program should not reach this execution point.
    // This function trusts that the input format is correct and directly processes it for output.

    TextBuffer buffer = construct_text_buffer();
    process_format_string(format, arg_list, &buffer);

    char *output = release_text_buffer(&buffer);
    fprintf(stdout, "%s", output);

    free(output);
    free(format);
}
