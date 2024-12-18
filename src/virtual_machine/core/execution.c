#include "../../symbol_table/representation/representation_table.h"
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../symbol_table/utility.h"
#include "../../data/region_table.h"

#include "../stack_management/stack_management.h"
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
            int field_type = get_struct_field_type(index_type_declaration, i);
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

// FIXME
static int get_index_variable(int index_declaration) {
    int region_declaration = get_declaration_region(index_declaration);
    int nis_declaration    = get_region_nis(region_declaration);
    int nis_utilisation    = get_region_nis(peek_region());
    int execution_offset   = get_declaration_execution(index_declaration);

    // BC + nis_utilisation - nis_declaration + execution_offset
    return nis_utilisation - nis_declaration + execution_offset;
}

// FIXME
void handle_variable_affectation(int type, int index_lexicographic, int index_declaration) {
    if (type != A_VARIABLE_ASSIGNMENT) return;

    int value = atoi(get_lexeme(index_lexicographic));
    int index = get_index_variable(index_declaration);
    int typed = get_declaration_description(index_declaration);

    fprintf(stdout, "Affectation(%s): %d -> %d\n", get_lexeme(typed), value, index);
    update_execution_cell(index, &value);
}