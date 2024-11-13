/**
 * @file variable_manager.h
 * @brief Header file for managing variable declarations.
 * 
 * This file contains the definitions and function declarations for managing
 * variable and paramater declarations in a compilation process.
 */

#ifndef __VARIABLE_MANAGER_H__
#define __VARIABLE_MANAGER_H__


#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Starts the declaration of a variable.
 *
 * This function initiates the process of declaring a variable by taking the
 * lexicographic index of the variable's name and its type.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the variable's name.
 * @param index_type_lexicographic The lexicographic index of the variable's type.
 */
void declaration_variable_start(int index_lexeme_lexicographic, int index_type_lexicographic);

/**
 * @brief Starts the declaration of a parameter.
 *
 * This function initiates the process of declaring a parameter by taking the
 * lexicographic index of the parameter's name and its type.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the parameter's name.
 * @param index_type_lexicographic The lexicographic index of the parameter's type.
 */
void declaration_param_start(int index_lexeme_lexicographic, int index_type_lexicographic);

/**
 * @brief Validates the definition of a type.
 *
 * This function checks if a type has been defined in the current scope.
 * If the type has not been defined, an error is thrown.
 *
 * @param index_type_lexicographic The lexicographic index of the type's name.
 */
void validate_type_definition(int index_type_lexicographic);

/**
 * @brief Validates the definition of a variable.
 *
 * This function checks if a variable has been defined in the current scope.
 * If the variable has not been defined, an error is thrown.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the variable's name.
 */
void validate_variable_definition(int index_lexeme_lexicographic);

#endif // __VARIABLE_MANAGER_H__