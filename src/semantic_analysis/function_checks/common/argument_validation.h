#ifndef __ARGUMENT_VALIDATION_H__
#define __ARGUMENT_VALIDATION_H__

#include "../../../symbol_table/declaration/declaration_table.h"
#include "../../validation_utils.h"
#include "../../../ast/ast.h"

/**
 * @brief Validates the number of arguments provided in a function call.
 * 
 * This function checks if the number of arguments provided in a function call
 * matches the number of parameters expected by the function.
 * If the number of arguments does not match, an error is thrown.
 * 
 * @param argument_list The argument list provided in the function call.
 * @param parameter_count The number of parameters expected by the function.
 */
void validate_argument_count(Node *argument_list, int parameter_count);

/**
 * @brief Validates the type of each argument in a function or procedure call.
 * 
 * This function iterates over the provided arguments and validates their types against
 * the expected parameter types for a function or procedure. It logs an error for each
 * type mismatch encountered.
 * 
 * @param argument_list A pointer to the `Node` representing the list of arguments.
 * @param parameter_count The total number of expected parameters for the function or procedure.
 * @param index_lexeme_lexicographic The lexeme index of the function or procedure.
 * @param nature The nature of the callable (e.g., `TYPE_FUNC` for functions, `TYPE_PROC` for procedures).
 */
void validate_each_argument(Node *argument_list, int parameter_count, int index_lexeme_lexicographic, Nature nature);

#endif // __ARGUMENT_VALIDATION_H__