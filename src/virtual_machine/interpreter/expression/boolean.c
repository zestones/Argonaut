#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../../utils/utils.h"
#include "../../core/execution.h"

#include "expression.h"

vm_cell resolve_boolean_expression(Node *expression) {
    if (expression == NULL) return construct_vm_cell(NULL_VALUE, NULL);

    switch (expression->type) {
        case A_IDENTIFIER: {
            // Resolve identifier value
            vm_cell cell = get_variable_cell(expression->index_declaration);
            if (!cell.is_initialized) {
                set_error_type(&error, RUN_TIME_ERROR);
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

        case A_BOOLEAN_LITERAL: {
            // Resolve boolean literal ('true' or 'false')
            int value = (strcmp(get_lexeme(expression->index_lexicographic), "true") == 0) ? 1 : 0;
            return construct_vm_cell(BOOLEAN, &value);
        }

        case A_EQUAL_OP: {
            // Handle equality check (==)
            vm_cell left = resolve_expression(expression->child);
            vm_cell right = resolve_expression(expression->child->sibling);

            if (left.type == BOOLEAN) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.boolean == right.value.boolean});
            } else if (left.type == INTEGER) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.integer == right.value.integer});
            } else if (left.type == REAL) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.real == right.value.real});
            } else if (left.type == CHARACTER)  {
                return construct_vm_cell(BOOLEAN, &(int){left.value.character == right.value.character});
            }
            break;
        }

        case A_NOT_EQUAL_OP: {
            // Handle inequality check (!=)
            vm_cell left = resolve_expression(expression->child);
            vm_cell right = resolve_expression(expression->child->sibling);

            if (left.type == BOOLEAN) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.boolean != right.value.boolean});
            } else if (left.type == INTEGER) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.integer != right.value.integer});
            } else if (left.type == REAL) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.real != right.value.real});
            } else if (left.type == CHARACTER)  {
                return construct_vm_cell(BOOLEAN, &(int){left.value.character != right.value.character});
            }
            break;
        }

        case A_AND_CONDITION: {
            // Handle logical AND (&&)
            vm_cell left = resolve_boolean_expression(expression->child);
            vm_cell right = resolve_boolean_expression(expression->child->sibling);
            return construct_vm_cell(BOOLEAN, &(int){left.value.boolean && right.value.boolean});
        }

        case A_OR_CONDITION: {
            // Handle logical OR (||)
            vm_cell left = resolve_boolean_expression(expression->child);
            vm_cell right = resolve_boolean_expression(expression->child->sibling);
            return construct_vm_cell(BOOLEAN, &(int){left.value.boolean || right.value.boolean});
        }

        case A_NOT_CONDITION: {
            // Handle logical NOT (!)
            vm_cell operand = resolve_boolean_expression(expression->child);
            return construct_vm_cell(BOOLEAN, &(int){!operand.value.boolean});
        }

        case A_LESS_THAN_OP: {
            vm_cell left = resolve_expression(expression->child);
            vm_cell right = resolve_expression(expression->child->sibling);

            if (left.type == INTEGER) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.integer < right.value.integer});
            } else if (left.type == REAL) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.real < right.value.real});
            }
            break;
        }

        case A_GREATER_THAN_OP: {
            vm_cell left = resolve_expression(expression->child);
            vm_cell right = resolve_expression(expression->child->sibling);

            if (left.type == INTEGER) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.integer > right.value.integer});
            } else if (left.type == REAL) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.real > right.value.real});
            }
            break;
        }

        case A_LESS_EQUAL_OP: {
            // Handle less than or equal (<=)
            vm_cell left = resolve_expression(expression->child);
            vm_cell right = resolve_expression(expression->child->sibling);

            if (left.type == INTEGER) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.integer <= right.value.integer});
            } else if (left.type == REAL) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.real <= right.value.real});
            }
            break;
        }

        case A_GREATER_EQUAL_OP: {
            // Handle greater than or equal (>=)
            vm_cell left = resolve_expression(expression->child);
            vm_cell right = resolve_expression(expression->child->sibling);

            if (left.type == INTEGER) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.integer >= right.value.integer});
            } else if (left.type == REAL) {
                return construct_vm_cell(BOOLEAN, &(int){left.value.real >= right.value.real});
            }
            break;
        }

        case A_CONDITION:
            return resolve_boolean_expression(expression->child);

        default:
            fprintf(stderr, "<boolean.c> - boolean expression type not implemented: %d\n", expression->type);
            exit(EXIT_FAILURE);
    }

    return construct_vm_cell(NULL_VALUE, NULL);
}
