#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../../utils/utils.h"
#include "../../core/execution.h"

#include "expression.h"

vm_cell resolve_comparison(int operation, vm_cell left, vm_cell right) {
    int result;

    if (!left.is_initialized || !right.is_initialized) {
        set_error_type(&error, RUN_TIME_ERROR);
        set_error_message(&error, "Uninitialized value error.\n  Ensure that both operands are initialized before comparing them.");
        yerror(error);
        exit(EXIT_FAILURE);
    }

    if (left.type == INTEGER || left.type == BOOLEAN) {
        int a = left.value.integer, b = right.value.integer;
        switch (operation) {
            case A_EQUAL_OP:         result = a == b; break;
            case A_NOT_EQUAL_OP:     result = a != b; break;
            case A_LESS_THAN_OP:     result = a < b;  break;
            case A_GREATER_THAN_OP:  result = a > b;  break;
            case A_LESS_EQUAL_OP:    result = a <= b; break;
            default:                 result = a >= b; break;
        }
    } else if (left.type == REAL) {
        float a = left.value.real, b = right.value.real;
        switch (operation) {
            case A_EQUAL_OP:         result = a == b; break;
            case A_NOT_EQUAL_OP:     result = a != b; break;
            case A_LESS_THAN_OP:     result = a < b;  break;
            case A_GREATER_THAN_OP:  result = a > b;  break;
            case A_LESS_EQUAL_OP:    result = a <= b; break;
            default:                 result = a >= b; break;
        }
    } else if (left.type == CHARACTER) {
        char a = left.value.character, b = right.value.character;
        switch (operation) {
            case A_EQUAL_OP:         result = a == b; break;
            case A_NOT_EQUAL_OP:     result = a != b; break;
            case A_LESS_THAN_OP:     result = a < b;  break;
            case A_GREATER_THAN_OP:  result = a > b;  break;
            case A_LESS_EQUAL_OP:    result = a <= b; break;
            default:                 result = a >= b; break;
        }
    } else {
        // The compiler only accepts int, float, bool and char operands.
        fprintf(stderr, "<boolean.c> - comparison not supported for this operand type\n");
        exit(EXIT_FAILURE);
    }

    return construct_vm_cell(BOOLEAN, &result);
}

vm_cell resolve_boolean_expression(Node *expression) {
    vm_cell cell = resolve_expression(expression);

    if (cell.type != BOOLEAN || !cell.is_initialized) {
        set_error_type(&error, RUN_TIME_ERROR);
        set_error_message(&error, "Condition error.\n  The condition did not evaluate to an initialized boolean.");
        yerror(error);
        exit(EXIT_FAILURE);
    }

    return cell;
}
