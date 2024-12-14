/**
 * @file region_table.h
 * @brief Header file for managing regions in the compiler.
 *
 * This file contains the definitions and function declarations for managing
 * regions in the compiler. A region is represented by the Region struct, which
 * includes information about its size, NIS (static nesting level), and AST
 * (Abstract Syntax Tree).
 */

#ifndef __REGION_TABLE_H__
#define __REGION_TABLE_H__


#include <stdlib.h>
#include <stdio.h>

#include "region_stack.h"
#include "../ast/ast.h"


#define MAX_REGION_COUNT 50 // Maximum number of regions that can be managed.


/**
 * @struct Region
 * @brief Represents a region in the compiler.
 *
 * This struct represents a region in the compiler. It includes information
 * about the size of the region, the static nesting level in the region, and
 * the Abstract Syntax Tree (AST) for the region.
 * 
 */
typedef struct Region {
    int size;           // Size of the region.
    int nis;            // Static nesting level in the region.
    AST ast;            // Abstract Syntax Tree
} Region;


/**
 * @brief Constructs a new region with the given size and NIS.
 *
 * @param size The size of the region.
 * @param nis The static nesting level in the region.
 * @return A new Region object.
 */
Region construct_region(int size, int nis);

/**
 * @brief Initializes the region table.
 *
 * This function sets up the region table for managing regions.
 */
void init_region_table();

/**
 * @brief Starts a new region.
 *
 * This function begins a new region in the region table.
 */
void start_region();

/**
 * @brief Ends the current region.
 *
 * This function ends the current region in the region table.
 */
void end_region();

/**
 * @brief Gets the size of the region at the specified index.
 *
 * @param index The index of the region.
 * @return The size of the region.
 */
int get_region_size(int index);

/**
 * @brief Gets the NIS of the current region.
 *
 * @return The NIS of the current region.
 */
int get_region_nis();

/**
 * @brief Gets the NIS of the current region.
 *
 * @return The NIS of the current region.
 */
int get_current_region_nis();

/**
 * @brief Gets the ID of the current region.
 *
 * @return The ID of the current region.
 */
int get_current_region_id();

/**
 * @brief Updates the size of the region at the specified index.
 *
 * @param index The index of the region.
 * @param size The new size of the region.
 */
void update_region_size(int index, int size);

/**
 * @brief Updates the AST of the region at the specified index.
 *
 * @param index The index of the region.
 * @param ast The new AST of the region.
 */
void update_region_ast(int index, AST ast);

/**
 * @brief Exports the region table to a file.
 *
 * @param filename The name of the file to write the region table to.
 */
void export_region_table(const char* filename);

/**
 * @brief Prints the region table.
 *
 * This function prints the current state of the region table.
 */
void print_region_table();


#endif // __REGION_TABLE_H__

