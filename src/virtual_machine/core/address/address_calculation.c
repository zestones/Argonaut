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

int get_array_address(Node *array_index_list) {
    int array_decl_index = get_declaration_description(array_index_list->index_declaration);
    if (get_declaration_nature(array_decl_index) != TYPE_ARRAY) {
        fprintf(stderr, "Error: Attempt to assign to a non-array type.\n");
        exit(EXIT_FAILURE);
    }

    int num_dimensions = get_array_dimension(array_decl_index);
    int base_address = get_variable_address(array_index_list->index_declaration);

    int calculated_offset = 0;
    int dimension_product = 1;

    Node *array_access = array_index_list->child->child;

    for (int dim = 0; dim < num_dimensions; dim++) {
        if (array_access == NULL) {
            fprintf(stderr, "Error: Insufficient indices provided for array assignment.\n");
            exit(EXIT_FAILURE);
        }

        printf("Node : %s\n", NodeTypeStrings[array_access->type]);

        if (array_access->type == A_ARRAY_INDEX) {
            // Resolve the expression for the current index in the current dimension
            vm_cell index_cell = resolve_expression(array_access->child);
            if (index_cell.type != INTEGER) {
                fprintf(stdout, "Error: Index type should be integer, instead got: %d\n", index_cell.type);
                exit(EXIT_FAILURE);
            }

            int index = index_cell.value.integer; // Assume the index is an integer
            printf("Index value for dimension %d: %d\n", dim, index);

            // Check bounds for the current dimension
            int lower_bound = get_array_nth_dimension(array_decl_index, 2 * dim); // Accessing bounds
            int upper_bound = get_array_nth_dimension(array_decl_index, 2 * dim + 1); // Accessing bounds

            printf("Lower bound: %d - Upper bound: %d\n", lower_bound, upper_bound);

            if (index < lower_bound || index > upper_bound) {
                fprintf(stderr, "Error: Index %d out of bounds for dimension %d.\n", index, dim);
                exit(EXIT_FAILURE);
            }

            // Calculate the offset for the current dimension
            calculated_offset += (index - lower_bound) * dimension_product;
            dimension_product *= (upper_bound - lower_bound + 1);

            // Move to the next index in the list (next dimension)
        }

        array_access = array_access->sibling;
    }

    // Calculate the final address
    int final_address = base_address + calculated_offset;
    printf("Final address: %d\n", final_address);

    return final_address;
}