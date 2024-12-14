/**
 * @file representation_table.h
 * @brief Header file for the representation table module.
 *
 * This module provides functions to initialize, insert, update, retrieve, and print
 * representations in a table. The table can hold up to a maximum of 1000 representations.
 */

#ifndef REPRESENTATION_TABLE_H
#define REPRESENTATION_TABLE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_REPRESENTATION_COUNT 1000


/**
 * @brief Initializes the representation table.
 *
 * This function initializes the representation table, setting up any necessary
 * data structures and preparing the table for use.
 */
void init_representation_table();

/**
 * @brief Inserts a representation into the table.
 *
 * @param value The value of the representation to insert.
 * @return The index at which the representation was inserted, or -1 if the table is full.
 */
int insert_representation(int value);

/**
 * @brief Updates a representation in the table.
 *
 * @param index The index of the representation to update.
 * @param value The new value of the representation.
 */
void update_representation(int index, int value);

/**
 * @brief Retrieves the value of a representation from the table.
 *
 * @param index The index of the representation to retrieve.
 * @return The value of the representation at the specified index.
 */
int get_representation_value(int index);

/**
 * @brief Exports the representation table to a file.
 *
 * @param filename The name of the file to write the representation table to.
 */
void export_representation_table(const char* filename);

/**
 * @brief Prints the contents of the representation table.
 *
 * This function prints the contents of the representation table to the standard output.
 */
void print_representation_table();

#endif // REPRESENTATION_TABLE_H
