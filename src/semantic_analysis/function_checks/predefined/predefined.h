#ifndef __PRINT_VALIDATION_H__
#define __PRINT_VALIDATION_H__

#include "../../validation_utils.h"
#include "../../../ast/ast.h"

/**
 * @brief Validates the arguments provided to an input-like procedure against a format string.
 * 
 * This function ensures the following:
 * 1. The number of arguments matches the number of format specifiers in the format string.
 * 2. Each argument is compatible with the corresponding format specifier.
 * 3. Input entities are properly defined before use.
 * 
 * If any validation fails, an appropriate error is logged, detailing the nature of the mismatch.
 * 
 * @param format_string A pointer to the `Node` representing the format string.
 * @param argument_list A pointer to the `Node` representing the list of arguments provided to the input procedure.
 */
void check_input_proc_argument_list(Node *format_string, Node *argument_list);

/**
 * @brief Validates the arguments provided to a print-like procedure against a format string.
 * 
 * This function performs two main validations:
 * 1. Checks whether the number of arguments matches the number of format specifiers in the format string.
 * 2. Ensures that each argument is compatible with its corresponding format specifier.
 * 
 * If any validation fails, an appropriate error is logged with details about the mismatch.
 * 
 * @param format_string A pointer to the `Node` representing the format string.
 * @param argument_list A pointer to the `Node` representing the list of arguments provided to the print procedure.
 */
void check_print_proc_argument_list(Node *format_string, Node *argument_list);

#endif // __PRINT_VALIDATION_H__