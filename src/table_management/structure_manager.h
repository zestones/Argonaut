/**
 * @file structure_manager.h
 * @brief Header file for managing structure declarations and their fields.
 *
 * This file contains the declarations and context management functions
 * for handling structures and their fields in a compilation process.
 */

#ifndef __STRUCTURE_MANAGER_H__
#define __STRUCTURE_MANAGER_H__


#include <stdio.h>
#include <stdlib.h>


/**
 * @struct structure_manager_context
 * @brief Context for managing structure declarations.
 *
 * This structure holds the context information needed for managing
 * structure declarations, including indices for field representation
 * and structure name, as well as execution offset and number of fields.
 */
typedef struct structure_manager_context
{
    int index_number_of_fields_representation; // Index for the number of fields representation.
    int index_struct_name_lexicographic;       // Index for the structure name in lexicographic order.

    int execution_offset;                      // Execution offset for the structure.
    int number_of_fields;                      // Number of fields in the structure.
} structure_manager_context;

/**
 * @brief Constructs the structure manager context.
 *
 * This function initializes the structure manager context with the given
 * index for the structure name in lexicographic order.
 *
 * @param index_struct_name_lexicographic Index for the structure name in lexicographic order.
 */
void construct_structure_manager_context(int index_struct_name_lexicographic);

/**
 * @brief Starts the declaration of a structure.
 *
 * This function marks the beginning of a structure declaration.
 */
void declaration_structure_start();

/**
 * @brief Adds a field to the current structure declaration.
 *
 * This function adds a field to the current structure being declared,
 * using the provided indices for the lexeme and type in lexicographic order.
 *
 * @param index_lexeme_lexicographic Index for the lexeme in lexicographic order.
 * @param index_type_lexicographic Index for the type in lexicographic order.
 */
void structure_add_field(int index_lexeme_lexicographic, int index_type_lexicographic);

/**
 * @brief Ends the declaration of a structure.
 *
 * This function marks the end of a structure declaration.
 */
void declaration_structure_end();


#endif // __STRUCTURE_MANAGER_H__