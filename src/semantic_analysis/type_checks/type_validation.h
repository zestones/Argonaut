#ifndef __TYPE_VALIDATION_H__
#define __TYPE_VALIDATION_H__

#include "../validation_utils.h"

/**
 * @brief Retrieves the declaration index of an array or struct.
 *
 * This function retrieves the declaration index of an array or struct
 * given the lexicographic index of the type's name.
 *
 * @param index_type_lexicographic The lexicographic index of the type's name.
 * @return The declaration index of the array or struct.
 */
int get_arr_struct_declaration_index(int index_type_lexicographic);

/**
 * @brief Validates that the lexicographic index is a base type.
 * 
 * This function checks if the lexicographic index corresponds to a base type.
 * If the lexicographic index does not correspond to a base type, an error is thrown.
 * 
 * @param index_lexeme_lexicographic The lexicographic index of the base type's name.
 */
void check_base_type(int index_lexeme_lexicographic);

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
 * @brief Checks if a type has been redefined.
 *
 * This function checks if a type has been redefined in the current scope.
 * If the type has been redefined, an error is thrown.
 *
 * @param index_lexeme_lexicographic The lexicographic index of the type's name.
 * @param line The line where the error occurred.
 * @param column The column where the error occurred.
 * @param nature The nature of the type.
 */
void check_type_redefinition(int index_lexeme_lexicographic, Nature nature);

#endif // __TYPE_VALIDATION_H__