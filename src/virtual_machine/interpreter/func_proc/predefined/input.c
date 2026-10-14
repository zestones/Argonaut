#include "../../../../semantic_analysis/function_checks/common/format_specifiers.h"
#include "../../../../symbol_table/lexeme/lexeme_table.h"
#include "../../../../utils/memory.h"
#include "../../../../utils/numeric_parser.h"

#include "../../../core/vm_cell.h"
#include "../../../core/execution.h"

#include "../../expression/expression.h"
#include "../func_proc.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

static int read_non_whitespace(void) {
    int character;
    do {
        character = getchar();
    } while (character != EOF && isspace((unsigned char) character));
    return character;
}

/**
 * @brief Reads one whitespace-delimited word from stdin (at most size - 1 characters).
 *
 * @return 1 when a word was read, 0 at end of input or when the word is too long.
 */
static int read_word(char *word, size_t size) {
    int character = read_non_whitespace();
    size_t length = 0;
    int overflow = 0;

    if (character == EOF) return 0;

    while (character != EOF && !isspace((unsigned char) character)) {
        if (length + 1 < size) word[length++] = (char) character;
        else overflow = 1;
        character = getchar();
    }
    word[length] = '\0';
    return !overflow;
}

static int read_character(char *value) {
    int character = read_non_whitespace();
    if (character == EOF) return 0;
    *value = (char) character;
    return 1;
}

/**
 * @brief Reports a failed read (end of input or text that does not match the specifier) and exits.
 *
 * @param specifier The conversion specifier that could not be satisfied ('d', 'f', 's' or 'c').
 */
static void input_failure(char specifier) {
    fatal_runtime_error("Input error: could not read a value for '%%%c'.\n"
                        "  The input ended or does not match the expected format.\n",
                        specifier);
}

/**
 * @brief Resolves the assignment of a value to an argument in a function or procedure call.
 *
 * This function handles the assignment of a resolved value (`vm_cell`) to the appropriate 
 * location in the argument list, whether it is a variable, array element, or structure field.
 *
 * @details
 * The argument list can contain different types of nodes, and the function determines the 
 * appropriate action based on the node type:
 * - **A_IDENTIFIER**: Assign the value to a variable.
 * - **A_ARRAY_ACCESS**: Assign the value to an element of an array.
 * - **A_STRUCT_FIELD_ACCESS**: Assign the value to a field within a structure.
 *
 * If the argument list contains an invalid type, the function prints an error message.
 *
 * @param arg_list A pointer to the AST node representing the argument to be assigned.
 * @param cell A `vm_cell` containing the value to be assigned to the argument.
 */
static void resolve_argument_assignement(AST arg_list, vm_cell cell) {
    switch (arg_list->type) {
        case A_IDENTIFIER: handle_variable_affectation(arg_list->index_declaration, cell); break;
        case A_ARRAY_ACCESS: handle_array_affectation(arg_list, cell); break;
        case A_STRUCT_FIELD_ACCESS: handle_structure_affectation(arg_list, cell); break;

        default: fprintf(stderr, "<input.c> - Invalid argument list.\n"); break;
    }
}

void execute_input(AST ast) {
    if (ast == NULL || ast->child == NULL) return;

    AST arg_list = ast->child;
    AST format_node = arg_list->sibling;

    const char *raw_format = get_lexeme(format_node->index_lexicographic);
    char *stripped_format = strip_quotes(raw_format);
    const char *format = stripped_format;

    // Note: No validation checks are performed on the 'raw_format' string or the processed 'format' string
    // at this stage. It is assumed that these strings have already been validated by the compiler. If the
    // compiler detected any errors in the format string, the program should not reach this execution point.
    // This function trusts that the input format is correct and directly processes it for output.

    char buffer[BUFFER_SIZE] = {0};
    arg_list = arg_list->child;
    while (format[0] != '\0') {
        if (format[0] == '%' && format[1] == '%') {
            char value;
            if (!read_character(&value) || value != '%') input_failure('%');
            format += 2;
            continue;
        }

        if (format[0] == '%' && is_format_specifier(format[1])) {
            char specifier = format[1];
            vm_cell cell;

            switch (specifier) {
                case 'd': {
                    char word[64];
                    int value;
                    if (!read_word(word, sizeof(word)) || !parse_integer_text(word, &value)) input_failure(specifier);
                    cell = construct_vm_cell(INTEGER, &value);
                    break;
                }
                case 'f': {
                    char word[128];
                    double value;
                    if (!read_word(word, sizeof(word)) || !parse_real_text(word, &value)) input_failure(specifier);
                    cell = construct_vm_cell(REAL, &value);
                    break;
                }
                case 's': {
                    if (!read_word(buffer, sizeof(buffer))) input_failure(specifier);
                    char *value = xstrdup(buffer);
                    cell = construct_vm_cell(STRING, value); // cells do not own their text
                    break;
                }
                case 'c': {
                    char value;
                    if (!read_character(&value)) input_failure(specifier);
                    cell = construct_vm_cell(CHARACTER, &value);
                    break;
                }
                case 'b': {
                    char word[8];
                    int value;
                    if (!read_word(word, sizeof(word))) input_failure(specifier);
                    if (strcmp(word, "true") == 0 || strcmp(word, "1") == 0) value = 1;
                    else if (strcmp(word, "false") == 0 || strcmp(word, "0") == 0) value = 0;
                    else input_failure(specifier);
                    cell = construct_vm_cell(BOOLEAN, &value);
                    break;
                }
                    // No default case as the format is already checked for 'd', 'f', 's', or 'c'
            }

            resolve_argument_assignement(arg_list->child, cell);
            arg_list = arg_list->sibling;
            format += 2;
        } else {
            format++;
        }
    }

    free(stripped_format);
}
