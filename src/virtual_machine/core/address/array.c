#include "../../../data/region_table.h"
#include "../../../symbol_table/utility.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../interpreter/expression/expression.h"

#include "../../stack_management/stack_management.h"
#include "../execution.h"

#include "address_calculation.h"

int get_array_address(Node *start_array_access, int base_address) {
    int array_decl_index = get_declaration_description(start_array_access->index_declaration);
    int num_dimensions = get_array_dimension(array_decl_index);
    int array_type_declaration = get_array_element_type(array_decl_index);
    
    if (base_address == NULL_VALUE) {
        base_address = get_variable_address(start_array_access->index_declaration);
    }

    int calculated_offset = 0;
    int element_size = get_declaration_execution(get_array_element_type(array_decl_index));
    int dimension_product = element_size;

    Node *array_access = start_array_access->child->child;

    for (int dim = 0; dim < num_dimensions; dim++) {
        if (array_access == NULL) {
            set_error_type(&error, RUN_TIME_ERROR);
            set_error_message(
                &error,
                "Insufficient indices provided for array assignment.\n"
                "  Ensure that '%d' indices are specified.",
                num_dimensions
            );
            yerror(error);
            exit(EXIT_FAILURE);
        }

        if (array_access->type == A_ARRAY_INDEX) {
            // Resolve the expression for the current index in the current dimension
            vm_cell index_cell = resolve_expression(array_access->child);
            if (index_cell.type != INTEGER) {
                set_error_type(&error, RUN_TIME_ERROR);
                set_error_message(
                    &error,
                    "Invalid index type, index type should be integer.\n"
                    "  Instead got type '%s'.",
                    get_cell_type_string(index_cell));
                yerror(error);
                exit(EXIT_FAILURE);
            }

            int index = index_cell.value.integer;

            // Check bounds for the current dimension
            int lower_bound = get_array_nth_dimension(array_decl_index, 2 * dim);
            int upper_bound = get_array_nth_dimension(array_decl_index, 2 * dim + 1);

            if (index < lower_bound || index > upper_bound) {
                set_error_type(&error, RUN_TIME_ERROR);
                set_error_message(
                    &error,
                    "Index %d out of bounds for dimension %d.\n"
                    "  Allowed range: [%d, %d].",
                    index, dim, lower_bound, upper_bound
                );
                yerror(error);

                exit(EXIT_FAILURE);
            }

            // Calculate the offset for the current dimension
            calculated_offset += (index - lower_bound) * dimension_product;
            dimension_product *= (upper_bound - lower_bound + 1) * element_size;
        }

        // Move to the next index in the list (next dimension)
        array_access = array_access->sibling;
    }

    // Calculate the final address
    int final_address = base_address + calculated_offset;
    if (get_declaration_nature(array_type_declaration) == TYPE_STRUCT && start_array_access->child->sibling != NULL) {
        // Compute struct field offset for array elements
        int struct_offset = get_struct_field_address(
            start_array_access->child->sibling,
            base_address,
            array_type_declaration
        );
        
        // Adjust the final address
        // The struct field address is relative to the base address
        // We need to adjust it to be relative to the array element address
        final_address += (struct_offset - base_address);
    }

    return final_address;
}