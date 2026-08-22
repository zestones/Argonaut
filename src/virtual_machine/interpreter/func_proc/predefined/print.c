#include "../../../../semantic_analysis/function_checks/common/format_specifiers.h"
#include "../../../../symbol_table/lexeme/lexeme_table.h"

#include "../../../core/vm_cell.h"
#include "../../../core/execution.h"

#include "../../expression/expression.h"
#include "../func_proc.h"

#include <stdio.h>
#include <string.h>

/**
 * @brief A growable text buffer: the formatted output can be longer than any fixed size.
 */
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} text_buffer;

static void append_text(text_buffer *buffer, const char *text) {
    size_t text_length = strlen(text);
    if (buffer->length + text_length + 1 > buffer->capacity) {
        size_t capacity = buffer->capacity ? buffer->capacity : 256;
        while (buffer->length + text_length + 1 > capacity) capacity *= 2;

        char *data = realloc(buffer->data, capacity);
        if (!data) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        buffer->data = data;
        buffer->capacity = capacity;
    }

    memcpy(buffer->data + buffer->length, text, text_length + 1);
    buffer->length += text_length;
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
static void process_format_string(const char *format, AST arg_list, text_buffer *buffer) {
    int format_idx = 0;
    char scratch[64];

    AST current_arg = arg_list->child;

    while (format[format_idx] != '\0') {
        if (format[format_idx] == '%' && (format[format_idx + 1] == 'd' || format[format_idx + 1] == 'f' || format[format_idx + 1] == 's' || format[format_idx + 1] == 'c')) {

            char specifier = format[format_idx + 1];
            vm_cell cell = resolve_expression(current_arg->child);

            switch (specifier) {
                case 'd':
                    snprintf(scratch, sizeof(scratch), "%d", cell.value.integer);
                    append_text(buffer, scratch);
                    break;
                case 'f':
                    snprintf(scratch, sizeof(scratch), "%f", cell.value.real);
                    append_text(buffer, scratch);
                    break;
                case 's': {
                    char *text = strip_quotes(cell.value.string);
                    append_text(buffer, text ? text : cell.value.string);
                    free(text);
                    break;
                }
                case 'c':
                    scratch[0] = cell.value.character;
                    scratch[1] = '\0';
                    append_text(buffer, scratch);
                    break;
                // No default case as the format is already checked for 'd', 'f', 's', or 'c'
            }

            current_arg = current_arg->sibling;
            format_idx += 2;
        } else {
            scratch[0] = format[format_idx++];
            scratch[1] = '\0';
            append_text(buffer, scratch);
        }
    }
}

void execute_print(AST ast) {
    if (ast == NULL || ast->child == NULL) return;

    AST arg_list = ast->child;
    AST format_node = arg_list->sibling;
    if (format_node == NULL) {
        char *text = strip_quotes(get_lexeme(arg_list->index_lexicographic));
        char *output = process_special_chars(text);
        fprintf(stdout, "%s", output);
        free(output);
        free(text);
        return;
    }

    const char *raw_format = get_lexeme(format_node->index_lexicographic);
    char *format = strip_quotes(raw_format);
    
    // Note: No validation checks are performed on the 'raw_format' string or the processed 'format' string
    // at this stage. It is assumed that these strings have already been validated by the compiler. If the
    // compiler detected any errors in the format string, the program should not reach this execution point.
    // This function trusts that the input format is correct and directly processes it for output.

    text_buffer buffer = {NULL, 0, 0};
    append_text(&buffer, "");
    process_format_string(format, arg_list, &buffer);

    char *output = process_special_chars(buffer.data);
    fprintf(stdout, "%s", output);

    free(output);
    free(buffer.data);
    free(format);
}