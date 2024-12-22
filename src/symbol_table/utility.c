#include "representation/representation_table.h"
#include "declaration/declaration_table.h"
#include "utility.h"

void export_table(const char* filename, 
                  void* table, 
                  size_t table_size, 
                  size_t element_size, 
                  char* (*format_row)(void*), 
                  const char* begin_marker, 
                  const char* end_marker) {
                    
    FILE* file = fopen(filename, "a");
    if (!file) {
        perror("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s\n", begin_marker);

    for (size_t i = 0; i < table_size; i++) {
        void* element = (char*)table + (i * element_size);
        char* row_data = format_row(element);
        
        if (row_data) {
            fprintf(file, "|%zu|%s|\n", i, row_data);
            free(row_data);
        }
    }

    fprintf(file, "%s\n", end_marker);
    fclose(file);
}


int get_array_element_type(int index_declaration_array) {
    int index_representation = get_declaration_description(index_declaration_array);
    return get_representation_value(index_representation);
}

int get_array_dimension(int index_declaration_array) {
    int index_representation = get_declaration_description(index_declaration_array);
    return get_representation_value(index_representation + 1);
}

int get_array_nth_dimension(int index_declaration_array, int nth_dim) {
    int index_representation = get_declaration_description(index_declaration_array);
    return get_representation_value(index_representation + 2 + nth_dim);
}

int get_array_size(int index_declaration_array) {
    int dimension = get_array_dimension(index_declaration_array);
    int size = 1;

    for (int i = 0; i < dimension; i++) {
        int lower_bound = get_array_nth_dimension(index_declaration_array, 2 * i);
        int upper_bound = get_array_nth_dimension(index_declaration_array, 2 * i + 1);

        if (lower_bound > upper_bound) {
            fprintf(stderr, "Error: lower bound cannot be greater than upper bound.\n");
            exit(EXIT_FAILURE);
        }

        // Calculate the size for the current dimension
        size *= (upper_bound - lower_bound + 1);
    }

    return size;
}

int get_struct_field_index_declaration(int current_type_representation, int field_index) {
    return get_representation_value(current_type_representation + 2 + (field_index * 3));
}

int get_struct_field_execution(int current_type_representation, int field_index) {
    return get_representation_value(current_type_representation + 3 + (field_index * 3));
}

int get_struct_field_type(int struct_declaration, int field_index) {
    int struct_representation = get_declaration_description(struct_declaration);
    int field_type = get_struct_field_index_declaration(struct_representation, field_index);  
    return field_type;
}

int get_struct_field_count(int struct_declaration) {
    int struct_representation = get_declaration_description(struct_declaration);
    return get_representation_value(struct_representation);
}