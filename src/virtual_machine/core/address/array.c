#include "../../../data/region_table.h"
#include "../../../symbol_table/utility.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../interpreter/expression/expression.h"

#include "../../stack_management/stack_management.h"
#include "../execution.h"

#include "address_calculation.h"

/**
 * @brief Declaration index of the array type an access node refers to.
 *
 * The node is either a variable or parameter of an array type, or, for an array field inside a
 * structure, the array type itself (the compiler stores the type in the field node).
 */
static int array_type_of(const Node *access) {
    int declaration = access->index_declaration;
    int nature = get_declaration_nature(declaration);
    if (nature == TYPE_VAR || nature == TYPE_PARAM) return get_declaration_description(declaration);
    return declaration;
}

static void runtime_error(const char *message) {
    set_error_type(&error, RUN_TIME_ERROR);
    set_error_message(&error, "%s", message);
    yerror(error);
    exit(EXIT_FAILURE);
}

int array_element_offset(Node *access, int array_type, int *type_out) {
    int num_dimensions = get_array_dimension(array_type);
    int element_type = get_array_element_type(array_type);
    int element_size = get_declaration_execution(element_type);

    int offset = 0;
    int stride = element_size; // the first index varies fastest

    Node *index_node = (access->child != NULL) ? access->child->child : NULL;
    for (int dim = 0; dim < num_dimensions; dim++) {
        if (index_node == NULL) {
            set_error_type(&error, RUN_TIME_ERROR);
            set_error_message(
                &error,
                "Insufficient indices provided for array access.\n"
                "  Ensure that '%d' indices are specified.",
                num_dimensions
            );
            yerror(error);
            exit(EXIT_FAILURE);
        }

        vm_cell index_cell = resolve_expression(index_node->child);
        if (index_cell.type != INTEGER) {
            set_error_type(&error, RUN_TIME_ERROR);
            set_error_message(
                &error,
                "Invalid index type, index type should be integer.\n"
                "  Instead got type '%s'.",
                get_cell_type_string(index_cell)
            );
            yerror(error);
            exit(EXIT_FAILURE);
        }

        int index = index_cell.value.integer;
        int lower_bound = get_array_nth_dimension(array_type, 2 * dim);
        int upper_bound = get_array_nth_dimension(array_type, 2 * dim + 1);

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

        offset += (index - lower_bound) * stride;
        stride *= (upper_bound - lower_bound + 1);
        index_node = index_node->sibling;
    }

    if (index_node != NULL) runtime_error("Too many indices provided for array access.");

    // Optional trailing field access on a structure element: t[i].x
    Node *field = (access->child != NULL) ? access->child->sibling : NULL;
    if (field != NULL) {
        if (get_declaration_nature(element_type) != TYPE_STRUCT) {
            runtime_error("Field access on an array whose elements are not structures.");
        }
        return offset + struct_field_offset(field, element_type, type_out);
    }

    if (type_out != NULL) *type_out = element_type;
    return offset;
}

int get_array_address(Node *start_array_access, int base_address) {
    if (base_address == NULL_VALUE) {
        base_address = get_variable_address(start_array_access->index_declaration);
    }

    return base_address + array_element_offset(start_array_access, array_type_of(start_array_access), NULL);
}
