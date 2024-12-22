#include "../../symbol_table/representation/representation_table.h"
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../symbol_table/utility.h"
#include "../../data/region_table.h"

#include "../stack_management/stack_management.h"
#include "address/address_calculation.h"
#include "execution.h"


static void declare_variable(int index_type_declaration) {
    if (is_declaration_base_type(index_type_declaration)) {
        allocate_execution_cells(get_declaration_description(index_type_declaration), get_declaration_execution(index_type_declaration));
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
            int field_type = get_struct_nth_field_type(index_type_declaration, i);
            declare_variable(field_type);
        }
    }
}

void handle_variable_declaration(int type, int index_lexicographic, int index_declaration) {
    if (type != A_VARIABLE_DECLARATION) return;

    int index_type_declaration = get_declaration_description(index_declaration);
    int execution_size = get_declaration_execution(index_type_declaration);
    declare_variable(index_type_declaration);
}

vm_cell get_variable_cell(int index_declaration) {
    int index = get_variable_address(index_declaration);
    return get_execution_cell(index);
}

void handle_variable_affectation(int index_declaration, vm_cell cell) {
    int address = get_variable_address(index_declaration);
    update_execution_cell(address, cell);
}

void handle_array_affectation(Node *array_index_list, vm_cell cell) {
    int address = get_array_address(array_index_list);
    update_execution_cell(address, cell);
}

void handle_structure_affectation(Node* struct_field_access, vm_cell cell) {
    int address = get_struct_field_address(struct_field_access);
    update_execution_cell(address, cell);
}