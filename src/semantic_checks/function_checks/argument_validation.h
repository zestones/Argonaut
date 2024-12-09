#ifndef __ARGUMENT_VALIDATION_H__
#define __ARGUMENT_VALIDATION_H__

#include "../validation_utils.h"
#include "../../ast/ast.h"

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
 * @brief Validates the type of an argument.
 * 
 * This function checks if the type of an argument matches the expected type.
 * If the types do not match, an error is thrown.
 * 
 * @param current_argument The current argument node.
 * @param expected_type The expected type of the argument.
 * @param argument_index The index of the argument in the argument list.
 * @param index_lexeme_lexicographic The lexicographic index of the function's name.
 */
void validate_each_argument(Node *argument_list, int index_representation_start, int parameter_count, int index_lexeme_lexicographic);

#endif // __ARGUMENT_VALIDATION_H__