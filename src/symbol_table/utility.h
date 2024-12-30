#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Exports a table to a file in a formatted manner.
 * 
 * This function writes a table to a file, appending rows formatted by a user-provided function. 
 * Each row includes an index and the formatted content of the table element.
 * 
 * @param filename The name of the file to write to.
 * @param table A pointer to the table data in memory.
 * @param table_size The number of elements in the table.
 * @param element_size The size (in bytes) of a single table element.
 * @param format_row A function pointer that formats a single table element into a string. 
 *                   The returned string must be dynamically allocated and will be freed after use.
 * @param begin_marker A marker string written at the beginning of the table in the file.
 * @param end_marker A marker string written at the end of the table in the file.
 * 
 * @details
 * - Opens the file in append mode and writes the `begin_marker` at the start.
 * - Iterates through the table elements, formats each row using the `format_row` function, and writes it to the file.
 * - Writes the `end_marker` at the end and closes the file.
 * 
 * @note If the file cannot be opened, the program terminates with an error.
 * 
 * @warning The `format_row` function must return dynamically allocated strings, as they will be freed internally.
 */
void export_table(const char* filename, 
                  void* table, 
                  size_t table_size, 
                  size_t element_size, 
                  char* (*format_row)(void*), 
                  const char* begin_marker, 
                  const char* end_marker);


/**
 * @brief Retrieves the type of elements stored in an array.
 * 
 * @param index_declaration_array The declaration index of the array.
 * @return The type of the array elements.
 */
int get_array_element_type(int index_declaration_array);

/**
 * @brief Retrieves the number of dimensions in an array.
 * 
 * @param index_declaration_array The declaration index of the array.
 * @return The number of dimensions of the array.
 */
int get_array_dimension(int index_declaration_array);

/**
 * @brief Retrieves the bounds of a specific dimension of an array.
 * 
 * @param index_declaration_array The declaration index of the array.
 * @param nth_dim The index of the dimension (0-based).
 * @return The size or range descriptor for the specified dimension.
 */
int get_array_nth_dimension(int index_declaration_array, int nth_dim);

/**
 * @brief Calculates the total size of an array based on its dimensions.
 * 
 * @param index_declaration_array The declaration index of the array.
 * @return The total number of elements in the array.
 * 
 * @details
 * - Iterates through all dimensions to calculate the product of their sizes.
 * - Checks that the lower bound of each dimension is not greater than the upper bound.
 * 
 * @note If a lower bound exceeds its upper bound, an error message is printed, and the program terminates.
 */
int get_array_size(int index_declaration_array);


/**
 * @brief Finds the index of a struct field by its lexicographic index.
 * 
 * @param declaration_type The type declaration index of the struct.
 * @param index_lexicographic The lexicographic index of the field to find.
 * @return The index of the field within the struct or `NULL_VALUE` if not found.
 */
int find_struct_field_index_lexicographic(int declaration_type, int index_lexicographic);

/**
 * @brief Retrieves the total number of fields in a struct.
 * 
 * @param struct_declaration The declaration index of the struct.
 * @return The number of fields in the struct.
 */
int get_struct_field_count(int struct_declaration);

/**
 * @brief Retrieves information about a specific field in a struct.
 * 
 * @param struct_declaration The declaration index of the struct.
 * @param nth_field The index of the field (0-based).
 * 
 * @return
 * - `get_struct_nth_field_execution`: The execution index of the nth field.
 * - `get_struct_nth_field_declaration`: The declaration index of the nth field.
 * - `get_struct_nth_field_lexicographic`: The lexicographic index of the nth field.
 */
int get_struct_nth_field_execution(int declaration_type, int nth);
int get_struct_nth_field_declaration(int struct_declaration, int nth_field); // TODO: rename type -> declaration
int get_struct_nth_field_lexicographic(int struct_declaration, int nth_field);


int get_func_return_type(int func_declaration);

#endif // __UTILITY_H__
