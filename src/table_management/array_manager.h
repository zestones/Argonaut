#ifndef __ARRAY_MANAGER_H__
#define __ARRAY_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct array_manager_context
{
    int array_size;

    int index_lexeme_lexicographic;
    int index_type_declaration;

    int number_of_dimensions;
    int index_number_of_dimensions_representation;
} array_manager_context;

void construct_array_manager_context(int current_lexeme_lexicographic_index);
void declaration_array_start();
void array_add_dimension(int min, int max);
void declaration_array_end();

#endif // __ARRAY_MANAGER_H__