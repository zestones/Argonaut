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
