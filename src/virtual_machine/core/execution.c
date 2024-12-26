#include "../../symbol_table/representation/representation_table.h"
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../symbol_table/utility.h"
#include "../../data/region_table.h"

#include "../stack_management/frame/stack_frame.h"
#include "../stack_management/stack_management.h"
#include "address/address_calculation.h"
#include "execution.h"


static void declare_variable(int index_type_declaration) {
    if (is_declaration_base_type(index_type_declaration)) {
        allocate_cells_to_stack_frame(peek_execution_stack_as_mutable(), get_declaration_description(index_type_declaration), get_declaration_execution(index_type_declaration));
        return;
    }

    if (get_declaration_nature(index_type_declaration) == TYPE_ARRAY) {
        int element_type = get_array_element_type(index_type_declaration);
        int size = get_array_size(index_type_declaration);

        for (int i = 0; i < size; i++) declare_variable(element_type);
    } 
    else if (get_declaration_nature(index_type_declaration) == TYPE_STRUCT) {
        int field_count = get_struct_field_count(index_type_declaration);

        for (int i = 0; i < field_count; i++) {
            int field_type = get_struct_nth_field_declaration(index_type_declaration, i);
            declare_variable(field_type);
        }
    }
}

void handle_variable_declaration(int index_lexicographic, int index_declaration) {
    int index_type_declaration = get_declaration_description(index_declaration);
    declare_variable(index_type_declaration);
}

vm_cell get_variable_cell(int index_declaration) {
    int address;
    Nature nature = get_declaration_nature(index_declaration);
    if (nature == TYPE_VAR) {
        address = get_variable_address(index_declaration);
    } else if (nature == TYPE_PARAM) {
        address = get_parameter_address(index_declaration);
    } else {
        printf("ERREUR lors de la recuperation de la cellule pour la variable %d\n", index_declaration);
    }
    
    int region = get_declaration_region(index_declaration);

    stack_frame frame = *find_stack_frame_by_region_index(region);
    return get_cell_from_stack_frame(frame, address);
}

void handle_variable_affectation(int index_declaration, vm_cell cell) {
    int address = get_variable_address(index_declaration);
    int region = get_declaration_region(index_declaration);

    stack_frame *frame = find_stack_frame_by_region_index(region);
    update_cell_in_stack_frame(frame, address, cell);
}

void handle_array_affectation(Node *array_index_list, vm_cell cell) {
    int address = get_array_address(array_index_list, NULL_VALUE);
    update_cell_in_stack_frame(peek_execution_stack_as_mutable(), address, cell);
}

void handle_structure_affectation(Node* struct_field_access, vm_cell cell) {
    int address = get_struct_field_address(struct_field_access, NULL_VALUE, NULL_VALUE);
    update_cell_in_stack_frame(peek_execution_stack_as_mutable(), address, cell);
}

void handle_function_return_value(vm_cell cell) {
    stack_frame current_frame = peek_execution_stack();
    stack_frame *dynamic_link_frame = get_stack_frame_by_id(current_frame.dynamic_link);
    dynamic_link_frame->region_value = cell;
}