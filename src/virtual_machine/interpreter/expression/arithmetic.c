#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../../utils/utils.h"
#include "../../core/execution.h"

#include "expression.h"

#include <limits.h>

/**
 * @brief Reports an integer overflow and stops: the language defines it as a run-time error,
 *        not as undefined behaviour or a silent wrap-around.
 */
static void overflow_error(char operation) {
    set_error_type(&error, RUN_TIME_ERROR);
    set_error_message(
        &error,
        "Integer overflow in '%c'.\n"
        "  The result does not fit in a 32-bit integer (%d to %d).",
        operation, INT_MIN, INT_MAX
    );
    yerror(error);
    exit(EXIT_FAILURE);
}


vm_cell resolve_negation(vm_cell operand) {
    if (!operand.is_initialized) {
        set_error_type(&error, RUN_TIME_ERROR);
        set_error_message(&error, "Uninitialized value error.\n  Ensure the operand is initialized before negating it.");
        yerror(error);
        exit(EXIT_FAILURE);
    }

    if (operand.type == INTEGER) {
        if (operand.value.integer == INT_MIN) overflow_error('-');
        int result = -operand.value.integer;
        return construct_vm_cell(INTEGER, &result);
    }
    if (operand.type == REAL) {
        double result = -operand.value.real;
        return construct_vm_cell(REAL, &result);
    }

    fprintf(stderr, "<arithmetic.c> - unary minus on a non-numeric value\n");
    exit(EXIT_FAILURE);
}

vm_cell resolve_arithmetic_operation(int operation, vm_cell left, vm_cell right) {
    if ((operation == A_DIV_OP || operation == A_MODULO_OP) &&
        ((right.type == INTEGER && right.value.integer == 0) || (right.type == REAL && right.value.real == 0.0))) {
        set_error_type(&error, RUN_TIME_ERROR);
        set_error_message(
            &error,
            "Division by zero error.\n"
            "  Ensure the right operand is not zero for division operations."
        );
     
        yerror(error);
        exit(EXIT_FAILURE);
    }

    // Check if the values are initialized
    if (!left.is_initialized || !right.is_initialized) {
        set_error_type(&error, RUN_TIME_ERROR);
        set_error_message(
            &error,
            "Uninitialized value error.\n"
            "  Ensure that both operands are initialized before performing arithmetic operations."
        );
     
        yerror(error);
        exit(EXIT_FAILURE);
    }

    // Then perform the operation
    switch (operation) {
        case A_ADD_OP:
            if (left.type == INTEGER && right.type == INTEGER) {
                int result;
                if (__builtin_add_overflow(left.value.integer, right.value.integer, &result)) overflow_error('+');
                return construct_vm_cell(INTEGER, &result);
            }
            else if (left.type == REAL && right.type == REAL) {
                double result = left.value.real + right.value.real;
                return construct_vm_cell(REAL, &result);
            }
            break;

        case A_SUB_OP:
            if (left.type == INTEGER && right.type == INTEGER) {
                int result;
                if (__builtin_sub_overflow(left.value.integer, right.value.integer, &result)) overflow_error('-');
                return construct_vm_cell(INTEGER, &result);
            }
            else if (left.type == REAL && right.type == REAL) {
                double result = left.value.real - right.value.real;
                return construct_vm_cell(REAL, &result);
            }
            break;

        case A_MUL_OP:
            if (left.type == INTEGER && right.type == INTEGER) {
                int result;
                if (__builtin_mul_overflow(left.value.integer, right.value.integer, &result)) overflow_error('*');
                return construct_vm_cell(INTEGER, &result);
            }
            else if (left.type == REAL && right.type == REAL) {
                double result = left.value.real * right.value.real;
                return construct_vm_cell(REAL, &result);
            }
            break;

        case A_DIV_OP:
            if (left.type == INTEGER && right.type == INTEGER) {
                if (left.value.integer == INT_MIN && right.value.integer == -1) overflow_error('/');
                int result = left.value.integer / right.value.integer;
                return construct_vm_cell(INTEGER, &result);
            }
            else if (left.type == REAL && right.type == REAL) {
                double result = left.value.real / right.value.real;
                return construct_vm_cell(REAL, &result);
            }
            break;

        case A_MODULO_OP:
            if (left.type == INTEGER && right.type == INTEGER) {
                // INT_MIN % -1 is 0 mathematically but undefined in C
                int result = (right.value.integer == -1) ? 0 : left.value.integer % right.value.integer;
                return construct_vm_cell(INTEGER, &result);
            }
            else if (left.type == REAL && right.type == REAL) {
                set_error_type(&error, RUN_TIME_ERROR);
                set_error_message(
                    &error,
                    "Modulo operation error.\n"
                    "  Modulo can only be performed on integers."
                );
                yerror(error);
                exit(EXIT_FAILURE);
            }
            break;
    }

    // The compiler should have already checked all issues within the argonaut code if an
    // arithmetic operation reach this line this means that the code executed containes compilation errors.
    // Thus the code here is unreachable for good argonaut code.
    __builtin_unreachable();
}
