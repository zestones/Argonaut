#ifndef __TYPE_VALIDATION_H__
#define __TYPE_VALIDATION_H__

#include "../validation_utils.h"

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
 */
void check_type_redefinition(int index_lexeme_lexicographic);

#endif // __TYPE_VALIDATION_H__