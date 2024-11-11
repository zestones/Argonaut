/**
 * @file region_stack.h
 * @brief Header file for managing a stack of regions.
 *
 * This file contains the function declarations for managing a stack of regions.
 * It provides functions to initialize the stack, push and pop regions, peek at
 * the top region, get the size of the stack, retrieve the stack, and print the stack.
*/

#ifndef __STACK_REGION_H__
#define __STACK_REGION_H__


#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "../utils/stack.h"


/**
* @brief Initializes the region stack.
*
* This function initializes the region stack, preparing it for use.
*/
void init_stack_region();

/**
 * @brief Pushes a region onto the stack.
 *
 * @param region_index The index of the region to push onto the stack.
 */
void push_region(int region_index);

/**
 * @brief Pops a region from the stack.
 *
 * @return The index of the region that was popped from the stack.
 */
int pop_region();

/**
 * @brief Peeks at the top region of the stack without removing it.
 *
 * @return The index of the region at the top of the stack.
 */
int peek_region();

/**
 * @brief Gets the size of the region stack.
 *
 * @return The number of regions currently in the stack.
 */
int get_region_stack_size();

/**
 * @brief Retrieves the region stack.
 *
 * @return The region stack.
 */
Stack get_region_stack();

/**
 * @brief Prints the contents of the region stack.
 *
 * This function prints the contents of the region stack to the standard output.
 */
void print_stack_region();

#endif // __STACK_REGION_H__

