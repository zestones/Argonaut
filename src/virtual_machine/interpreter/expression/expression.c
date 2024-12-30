#include "../../../symbol_table/lexeme/lexeme_table.h"
#include "../../../utils/utils.h"

#include "../../stack_management/stack_management.h"
#include "../../core/address/address_calculation.h"
#include "../../core/execution.h"

#include "../func_proc/func_proc.h"
#include "../interpreter.h"
#include "expression.h"

vm_cell resolve_expression(Node *expression) {
    if (expression == NULL) return construct_vm_cell(NULL_VALUE, NULL);

    switch (expression->type) {
        case A_IDENTIFIER: {
            vm_cell cell = get_variable_cell(expression->index_declaration);
            if (!cell.is_initialized) {
                set_error_type(&error, UNINITIALIZED_VARIABLE);
                set_error_message(
                    &error,
                    "Uninitialized variable '%s' at %s.\n"
                    "  Ensure the variable is initialized before use.\n",
                    get_lexeme(expression->index_lexicographic),
                    get_formatted_location()
                );
                yerror(error);
                exit(EXIT_FAILURE);
            }

            return cell;
        }

        case A_INTEGER_LITERAL: {
            int value = atoi(get_lexeme(expression->index_lexicographic));
            return construct_vm_cell(INTEGER, &value);
        }

        case A_FLOAT_LITERAL: {
            double value = atof(get_lexeme(expression->index_lexicographic));
            return construct_vm_cell(REAL, &value);
        }

        case A_BOOLEAN_LITERAL: {
            int value = (strcmp(get_lexeme(expression->index_lexicographic), "true") == 0) ? 1 : 0;
            return construct_vm_cell(BOOLEAN, &value);
        }

        case A_CHARACTER_LITERAL: {
            char *lexeme = get_lexeme(expression->index_lexicographic);
            char value = lexeme[1];
            return construct_vm_cell(CHARACTER, &value);
        }

        case A_STRING_LITERAL: {
            char *lexeme = get_lexeme(expression->index_lexicographic);
            return construct_vm_cell(STRING, lexeme);
        }

        case A_ADD_OP:
        case A_SUB_OP:
        case A_MUL_OP:
        case A_DIV_OP: {
            vm_cell left = resolve_expression(expression->child);
            vm_cell right = resolve_expression(expression->child->sibling);

            return resolve_arithmetic_operation(expression->type, left, right);
        }

        case A_ARRAY_ACCESS:
            return get_array_cell(expression);

        case A_STRUCT_FIELD_ACCESS:
            return get_struct_cell(expression);

        case A_FUNC_PROC_CALL_STATEMENT: {
            execute_func_proc_call(expression);
            stack_frame current_frame = peek_execution_stack();
            vm_cell cell = current_frame.region_value;
            // Clear the region value after execution to continue to resolve statement list
            peek_execution_stack_as_mutable()->region_value = construct_vm_cell(NULL_VALUE, NULL); 
            return cell;
        }

        default:
            printf("Expression type not yet implemented.\n");
            break;
    } 

    return construct_vm_cell(NULL_VALUE, NULL);
}
