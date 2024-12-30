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
    switch (arg_list->type)
    {
        case A_IDENTIFIER:
            handle_variable_affectation(arg_list->index_declaration, cell);
            break;
        case A_ARRAY_ACCESS:
            handle_array_affectation(arg_list, cell);
            break; 
        case A_STRUCT_FIELD_ACCESS:
            handle_structure_affectation(arg_list, cell);
            break;
        
        default:
            printf("Error: Invalid argument list.\n");
            break;
        }
}

void execute_input(AST ast) {
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
    arg_list = arg_list->child;
    while (format[0] != '\0') {
        if (format[0] == '%' && 
           (format[1] == 'd' || format[1] == 'f' || format[1] == 's' || format[1] == 'c')) {
            char specifier = format[1];
            vm_cell cell;

            switch (specifier) {
                case 'd': {
                    int value;
                    scanf("%d", &value);
                    cell = construct_vm_cell(INTEGER, &value); 
                    break;
                }
                case 'f': {
                    double value;
                    scanf("%lf", &value);
                    cell = construct_vm_cell(REAL, &value); 
                    break;
                }
                case 's': {
                    scanf("%s", buffer);
                    cell = construct_vm_cell(STRING, &buffer);
                    break;
                }
                case 'c': {
                    char value;
                    scanf(" %c", &value);
                    cell = construct_vm_cell(CHARACTER, &value);
                    break;
                }
                default:
                    printf("Error: Unsupported format specifier '%c'.\n", specifier);
                    return;
            }

            resolve_argument_assignement(arg_list->child, cell); 
            arg_list = arg_list->sibling;
            format += 2;
        } else {
            format++;
        }
    }
}

