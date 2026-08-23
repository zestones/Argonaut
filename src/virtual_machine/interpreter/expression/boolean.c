#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../../utils/utils.h"
#include "../../core/execution.h"

#include "expression.h"

static int compare_values(int operation, double left, double right) {
    switch (operation) {
        case A_EQUAL_OP: return left == right;
        case A_NOT_EQUAL_OP: return left != right;
        case A_LESS_THAN_OP: return left < right;
        case A_GREATER_THAN_OP: return left > right;
        case A_LESS_EQUAL_OP: return left <= right;
        default: return left >= right;
    }
}

vm_cell resolve_comparison(int operation, vm_cell left, vm_cell right) {
    if (!left.is_initialized || !right.is_initialized) {
        fatal_runtime_error(
            "Uninitialized value error.\n  Ensure that both operands are initialized before comparing them.");
    }

    double left_value;
    double right_value;
    if (left.type == INTEGER || left.type == BOOLEAN) {
        left_value = left.value.integer;
        right_value = right.value.integer;
    } else if (left.type == REAL) {
        left_value = left.value.real;
        right_value = right.value.real;
    } else if (left.type == CHARACTER) {
        left_value = left.value.character;
        right_value = right.value.character;
    } else {
        // The compiler only accepts int, float, bool and char operands.
        fprintf(stderr, "<boolean.c> - comparison not supported for this operand type\n");
        exit(EXIT_FAILURE);
    }

    int result = compare_values(operation, left_value, right_value);
    return construct_vm_cell(BOOLEAN, &result);
}

vm_cell resolve_boolean_expression(Node *expression) {
    vm_cell cell = resolve_expression(expression);

    if (cell.type != BOOLEAN || !cell.is_initialized) {
        fatal_runtime_error("Condition error.\n  The condition did not evaluate to an initialized boolean.");
    }

    return cell;
}
