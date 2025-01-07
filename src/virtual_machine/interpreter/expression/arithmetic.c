#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../../utils/utils.h"
#include "../../core/execution.h"

#include "expression.h"


vm_cell resolve_arithmetic_operation(int operation, vm_cell left, vm_cell right) {
    if ((operation == A_DIV_OP || operation == A_MODULO_OP) && (right.type == INTEGER && right.value.integer == 0) ||
        (right.type == REAL && right.value.real == 0.0)) {
        set_error_type(&error, DIVISION_BY_ZERO);
        set_error_message(
            &error,
            "Division by zero error at %s.\n"
            "  Ensure the right operand is not zero for division operations.\n",
            get_formatted_location()
        );
     
        yerror(error);
        exit(EXIT_FAILURE);
    }

    // Check if the values are initialized
    if (!left.is_initialized || !right.is_initialized) {
        set_error_type(&error, UNINITIALIZED_VARIABLE);
        set_error_message(
            &error,
            "Uninitialized value error at %s.\n"
            "  Ensure that both operands are initialized before performing arithmetic operations.\n",
            get_formatted_location()
        );
     
        yerror(error);
        exit(EXIT_FAILURE);
    }

    // Then perform the operation
    switch (operation) {
        case A_ADD_OP:
            if (left.type == INTEGER && right.type == INTEGER) {
                int result = left.value.integer + right.value.integer;
                return construct_vm_cell(INTEGER, &result);
            }
            else if (left.type == REAL && right.type == REAL) {
                double result = left.value.real + right.value.real;
                return construct_vm_cell(REAL, &result);
            }
            break;

        case A_SUB_OP:
            if (left.type == INTEGER && right.type == INTEGER) {
                int result = left.value.integer - right.value.integer;
                return construct_vm_cell(INTEGER, &result);
            }
            else if (left.type == REAL && right.type == REAL) {
                double result = left.value.real - right.value.real;
                return construct_vm_cell(REAL, &result);
            }
            break;

        case A_MUL_OP:
            if (left.type == INTEGER && right.type == INTEGER) {
                int result = left.value.integer * right.value.integer;
                return construct_vm_cell(INTEGER, &result);
            }
            else if (left.type == REAL && right.type == REAL) {
                double result = left.value.real * right.value.real;
                return construct_vm_cell(REAL, &result);
            }
            break;

        case A_DIV_OP:
            if (left.type == INTEGER && right.type == INTEGER) {
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
                int result = left.value.integer % right.value.integer;
                return construct_vm_cell(INTEGER, &result);
            }
            else if (left.type == REAL && right.type == REAL) {
                printf("modulos can be performed on integers only\n");
            }
            break;
    }
}
        