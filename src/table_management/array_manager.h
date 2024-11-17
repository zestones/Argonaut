/**
 * @file
 * @brief Header file for managing arrays.
 *
 * This file contains the definitions and function declarations for managing arrays.
 * It provides functions to construct the array manager context, start and end array
 * declarations, and add dimensions to the array.
 */

#ifndef __ARRAY_MANAGER_H__
#define __ARRAY_MANAGER_H__


#include <stdio.h>
#include <stdlib.h>


/**
 * @struct array_manager_context
 * @brief A structure to manage the context of an array.
 * 
 * This structure holds information about the array such as its size,
 * the lexicographic index of its name, the number of dimensions, and
 * the index representation of the number of dimensions.
 */
typedef struct array_manager_context
{
    int array_size;                                 // Size of the array.
    int index_array_name_lexicographic;             // Lexicographic index of the array name.

    int number_of_dimensions;                       // Number of dimensions of the array.
    int index_number_of_dimensions_representation;  // Index representation of the number of dimensions.
} array_manager_context;

/**
 * @brief Constructs the array manager context.
 * 
 * Initializes the array manager context with the given lexicographic index
 * of the current array name.
 * 
 * @param current_array_name_lexicographic_index The lexicographic index of the current array name.
 */
void construct_array_manager_context(int current_array_name_lexicographic_index);

/**
 * @brief Starts the declaration of an array.
 * 
 * This function should be called at the beginning of an array declaration.
 */
void declaration_array_start();

/**
 * @brief Adds a dimension to the array.
 * 
 * This function adds a dimension to the array with the specified minimum and maximum values.
 * 
 * @param min The minimum value of the dimension.
 * @param max The maximum value of the dimension.
 */
void array_add_dimension(int min, int max);

/**
 * @brief Ends the declaration of an array.
 * 
 * This function should be called at the end of an array declaration.
 */
void declaration_array_end();

#endif // __ARRAY_MANAGER_H__