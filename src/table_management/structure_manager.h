#ifndef __STRUCTURE_MANAGER_H__
#define __STRUCTURE_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct structure_manager_context
{
    int index_number_of_fields_representation;
    int index_struct_name_lexicographic;

    int execution_offset;
    int number_of_fields;
} structure_manager_context;


void construct_structure_manager_context(int index_struct_name_lexicographic);

void declaration_structure_start();

void structure_add_field(int index_lexeme_lexicographic, int index_type_lexicographic);

void declaration_structure_end();


#endif // __STRUCTURE_MANAGER_H__