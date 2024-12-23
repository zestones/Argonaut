#include "../../../data/region_table.h"
#include "../../../symbol_table/utility.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../interpreter/expression/expression.h"

#include "../../stack_management/stack_management.h"
#include "../execution.h"

#include "address_calculation.h"


int get_struct_field_address(Node *struct_field_access, int base_address, int index_declaration) {
    // Start from the root structure
    int struct_declaration = index_declaration;
    Node *current_field = struct_field_access;
    if (base_address == NULL_VALUE) {
        struct_declaration = get_declaration_description(struct_field_access->index_declaration);
        base_address = get_variable_address(struct_field_access->index_declaration);
        current_field = struct_field_access->child;
    }

    // Traverse the nested field access hierarchy
    int offset = 0;
    while (current_field != NULL) {
        // Find the index and execution offset for the current field
        int nth_field          = find_struct_field_index_lexicographic(struct_declaration, current_field->index_lexicographic);
        int field_execution    = get_struct_nth_field_execution(struct_declaration, nth_field);
        int index_declaration  = get_struct_nth_field_declaration(struct_declaration, nth_field);

        if (get_declaration_nature(index_declaration) == TYPE_ARRAY) {
            // Calculate the address for the array
            int array_offset = get_array_address(current_field, base_address);
            offset += array_offset + field_execution;
            break;
        }

        if (get_declaration_nature(index_declaration) == TYPE_STRUCT) {
            struct_declaration = index_declaration;
        }

        offset += field_execution; // Accumulate the offset
        current_field = current_field->child;
    }

    // Calculate the final address
    int final_address = base_address + offset;
    return final_address;
}
