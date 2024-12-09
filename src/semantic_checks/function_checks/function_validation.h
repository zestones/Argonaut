#ifndef __FUNCTION_VALIDATION_H__
#define __FUNCTION_VALIDATION_H__

#include "../validation_utils.h"
#include "../../ast/ast.h"

/**
 * @brief Validates the definition of a function or procedure.
 *
 * This function checks if a function or procedure has been defined in the current scope.
 * If the function or procedure has not been defined, an error is thrown.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the function or procedure's name.
 */
void check_func_proc_definition(int index_lexeme_lexicographic);

/**
 * @brief Checks if a function or procedure has been redefined.
 *
 * This function checks if a function or procedure has been redefined in the current scope.
 * If the function or procedure has been redefined, an error is thrown.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the function or procedure's name.
 * @param type The type of the function or procedure.
 */
void check_func_proc_redefinition(int index_lexeme_lexicographic, char *type);

/**
 * @brief Validates the argument list of a function.
 *
 * This function checks if the arguments provided in the function call match the expected arguments.
 * If the arguments do not match, an error is thrown.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the function's name.
 * @param argument_list The argument list provided in the function call.
 */
void check_func_argument_list(int index_lexeme_lexicographic, Node *argument_list);

#endif // __FUNCTION_VALIDATION_H__