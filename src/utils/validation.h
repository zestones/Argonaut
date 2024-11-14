/**
 * @file validation.h
 * @brief This file contains the definitions and function declarations for validating
 * type and variable definitions in a compilation process.
 */

#ifndef __VALIDATION_H__
#define __VALIDATION_H__


#include "errors.h"
#include "utils.h"

/**
 * @brief Validates the definition of a type.
 *
 * This function checks if a type has been defined in the current scope.
 * If the type has not been defined, an error is thrown.
 *
 * @param index_type_lexicographic The lexicographic index of the type's name.
 * @param line The line where the error occurred.
 * @param column The column where the error occurred.
 */
void check_type_definition(int index_type_lexicographic);

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
 * @brief Checks if a type has been redefined.
 *
 * This function checks if a type has been redefined in the current scope.
 * If the type has been redefined, an error is thrown.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the type's name.
 * @param line The line where the error occurred.
 * @param column The column where the error occurred.
 */
void check_type_redefinition(int index_lexeme_lexicographic);

/**
 * @brief Checks if a function or procedure has been redefined.
 *
 * This function checks if a function or procedure has been redefined in the current scope.
 * If the function or procedure has been redefined, a warning is thrown.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the function or procedure's name.
 * @param type The type of the function or procedure.
 */
void check_func_proc_redefinition(int index_lexeme_lexicographic, char *type);

#endif // __VALIDATION_H__
