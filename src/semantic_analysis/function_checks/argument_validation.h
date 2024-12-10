#ifndef __ARGUMENT_VALIDATION_H__
#define __ARGUMENT_VALIDATION_H__

#include "../../symbol_table/declaration/declaration_table.h"
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
 * @brief Validates the type of each argument provided in a function call.
 * 
 * @param parameter_count The number of parameters expected by the function.
 * @param parameter_count The number of parameters expected by the function.
 * @param index_lexeme_lexicographic The index of the lexeme in the lexicographically sorted lexeme table.
 * @param nature The nature of the function or procedure.
 */
void validate_each_argument(Node *argument_list, int parameter_count, int index_lexeme_lexicographic, Nature nature);

#endif // __ARGUMENT_VALIDATION_H__