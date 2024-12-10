#ifndef __VARIABLE_VALIDATION_H__
#define __VARIABLE_VALIDATION_H__

#include "../validation_utils.h"

/**
 * @brief Validates the definition of a variable.
 *
 * This function checks if a variable has been defined in the current scope.
 * If the variable has not been defined, an error is thrown.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the variable's name.
 * @param line The line where the error occurred.
 * @param column The column where the error occurred.
 */
void check_variable_definition(int index_lexeme_lexicographic);

/**
 * @brief Checks if a variable has been redefined.
 *
 * This function checks if a variable has been redefined in the current scope.
 * If the variable has been redefined, an error is thrown.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the variable's name.
 * @param line The line where the error occurred.
 * @param column The column where the error occurred.
 */
void check_variable_redefinition(int index_lexeme_lexicographic);

/**
 * @brief Checks if an assinement to a variable is valid.
 *
 * This function checks the type of the expression being assigned to a variable.
 * If the assignment is invalid, an error is thrown.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the variable's name.
 * @param expression The expression being assigned to the variable.
 */
void check_variable_assignment(int index_lexeme_lexicographic, Node *expression);

#endif // __VARIABLE_VALIDATION_H__