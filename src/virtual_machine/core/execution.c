#include "../../symbol_table/representation/representation_table.h"
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../symbol_table/utility.h"
#include "../../data/region_table.h"

#include "../stack_management/stack_management.h"
#include "execution.h"


void declare_variable(int type, int execution_size) {
    for (int i = 0; i < execution_size; i++) {
        vm_cell cell = construct_vm_cell(type, NULL);
        push_execution_stack(type, &cell);
    }
}

void declare_array_variable(int array_type, int execution_size) {
    int element_type = get_array_element_type(array_type);
    
    if (get_declaration_nature(element_type) == TYPE_STRUCT) {
        int num_fields = get_struct_field_count(element_type);
        while (execution_size > 0) {
            int field_type = get_struct_field_type(element_type, execution_size % num_fields);
            push_execution_stack(field_type, NULL);
            execution_size--;
        }
    } else {
        declare_variable(element_type, execution_size);
    }
}

void handle_declaration(Node *declaration) {
    if (declaration == NULL || declaration->type != A_VARIABLE_DECLARATION) return;

    int index_lexicographic = declaration->index_lexicographic;
    int index_declaration = declaration->index_declaration;

    int index_type_declaration = get_declaration_description(index_declaration);
    int execution_size = get_declaration_execution(index_type_declaration);

    if (is_declaration_base_type(index_type_declaration)) {
        declare_variable(index_type_declaration, execution_size);
    } 
    else if (get_declaration_nature(index_type_declaration) == TYPE_ARRAY) {
        declare_array_variable(index_type_declaration, execution_size);  
    }
    else if (get_declaration_nature(index_type_declaration) == TYPE_STRUCT) {
        int struct_type = get_struct_field_index_declaration(index_type_declaration, 0);  
        declare_variable(struct_type, execution_size);
    }
}