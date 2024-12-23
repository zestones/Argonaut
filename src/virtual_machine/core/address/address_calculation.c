#include "../../../data/region_table.h"
#include "../../../symbol_table/utility.h"

#include "../../interpreter/expression/expression.h"

#include "../../stack_management/stack_management.h"
#include "../execution.h"

#include "address_calculation.h"

int get_variable_address(int index_declaration) {
    int region_declaration = get_declaration_region(index_declaration);
    int nis_declaration    = get_region_nis(region_declaration);
    int nis_utilisation    = get_region_nis(peek_region());
    int execution_offset   = get_declaration_execution(index_declaration);

    // BC + nis_utilisation - nis_declaration + execution_offset
    return nis_utilisation - nis_declaration + execution_offset;
}

// TODO: find a better way to init the base address
int get_array_address(Node *start_array_access, int base_address) {
    int array_decl_index = get_declaration_description(start_array_access->index_declaration);
    int num_dimensions = get_array_dimension(array_decl_index);
    
    if (base_address == NULL_VALUE) {
        base_address = get_variable_address(start_array_access->index_declaration);
    }

    int calculated_offset = 0;
    int dimension_product = 1;

    Node *array_access = start_array_access->child->child;

    for (int dim = 0; dim < num_dimensions; dim++) {
        if (array_access == NULL) {
            printf("Error: Insufficient indices provided for array assignment.\n");
            exit(EXIT_FAILURE);
        }

        if (array_access->type == A_ARRAY_INDEX) {
            // Resolve the expression for the current index in the current dimension
            vm_cell index_cell = resolve_expression(array_access->child);
            if (index_cell.type != INTEGER) {
                printf("Error: Index type should be integer, instead got: %d\n", index_cell.type);
                exit(EXIT_FAILURE);
            }

            int index = index_cell.value.integer;

            // Check bounds for the current dimension
            int lower_bound = get_array_nth_dimension(array_decl_index, 2 * dim);
            int upper_bound = get_array_nth_dimension(array_decl_index, 2 * dim + 1);
            printf("Index: %d, Lower: %d, Upper: %d\n", index, lower_bound, upper_bound);

            if (index < lower_bound || index > upper_bound) {
                printf("Error: Index %d out of bounds for dimension %d.\n", index, dim);
                exit(EXIT_FAILURE);
            }

            // Calculate the offset for the current dimension
            calculated_offset += (index - lower_bound) * dimension_product;
            dimension_product *= (upper_bound - lower_bound + 1);
        }

        // Move to the next index in the list (next dimension)
        array_access = array_access->sibling;
    }

    // Calculate the final address
    int final_address = base_address + calculated_offset;
    return final_address;
}

int get_struct_field_address(Node *struct_field_access) {
    // Start from the root structure
    int base_address       = get_variable_address(struct_field_access->index_declaration);
    int struct_declaration = get_declaration_description(struct_field_access->index_declaration);

    // Traverse the nested field access hierarchy
    int offset = 0;
    Node *current_field = struct_field_access->child;
    while (current_field != NULL) {
        // Find the index and execution offset for the current field
        int nth_field          = find_struct_field_index_lexicographic(struct_declaration, current_field->index_lexicographic);
        int field_execution    = get_struct_nth_field_execution(struct_declaration, nth_field);
        int index_declaration  = get_struct_nth_field_declaration(struct_declaration, nth_field);

        if (get_declaration_nature(index_declaration) == TYPE_ARRAY) {
            // Calculate the address for the array
            int array_address = get_array_address(current_field, base_address);
            offset += array_address + field_execution;
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
