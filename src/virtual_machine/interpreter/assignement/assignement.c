#include "../../../data/region_table.h"
#include "../../../symbol_table/utility.h"

#include "../../stack_management/stack_management.h"
#include "../../core/execution.h"

#include "../expression/expression.h"

static void array_assignment(Node *array_index_list, vm_cell cell) {
    int array_decl_index = get_declaration_description(array_index_list->index_declaration);
    if (get_declaration_nature(array_decl_index) != TYPE_ARRAY) {
        fprintf(stderr, "Error: Attempt to assign to a non-array type.\n");
        exit(EXIT_FAILURE);
    }

    int num_dimensions = get_array_dimension(array_decl_index);
    int base_address = get_index_variable(array_index_list->index_declaration); // Base address of the array in the stack
    printf("base_address: %d\n", base_address);

    int calculated_offset = 0;
    int dimension_product = 1;

    Node *array_access = array_index_list->child->child; // Starting at the list of indices
    printf("Number of dimensions: %d\n", num_dimensions);

    // Loop through each dimension and resolve the index
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

    // Assign the value to the calculated address in the execution stack
    update_execution_cell(final_address, cell);
}

void resolve_assignement(Node *assignement_start) {
    if (assignement_start == NULL) return;

    Node *assignement = assignement_start->child;
    if (assignement->type == A_VARIABLE_ASSIGNMENT) {
        vm_cell cell = resolve_expression(assignement->child);
        handle_variable_affectation(assignement->index_declaration, cell);
    } else if (assignement->type == A_ARRAY_ASSIGNMENT) {
        vm_cell cell = resolve_expression(assignement->child->sibling);
        fprintf_vm_cell(stdout, cell);

        array_assignment(assignement->child, cell);
    }
}