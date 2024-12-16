#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Generic function to export a table.
 * 
 * @param filename Name of the file to export to.
 * @param table Array of data to export.
 * @param table_size Number of elements in the table.
 * @param format_row A function pointer to format a row of the table as a string.
 * @param begin_marker String marker for the beginning of the table.
 * @param end_marker String marker for the end of the table.
 */
void export_table(const char* filename, 
                  void* table, 
                  size_t table_size, 
                  size_t element_size, 
                  char* (*format_row)(void*), 
                  const char* begin_marker, 
                  const char* end_marker);


int get_array_element_type(int index_declaration_array);

int get_struct_field_index_declaration(int current_type_representation, int field_index);

int get_struct_field_execution(int current_type_representation, int field_index);

int get_struct_field_type(int struct_declaration, int field_index);

int get_struct_field_count(int struct_declaration);


#endif // __UTILITY_H__
