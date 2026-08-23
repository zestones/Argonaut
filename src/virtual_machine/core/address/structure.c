#include "../../../data/region_table.h"
#include "../../../symbol_table/utility.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../interpreter/expression/expression.h"

#include "../../stack_management/stack_management.h"
#include "../execution.h"

#include "address_calculation.h"

int struct_field_offset(Node *field, int struct_type, int *type_out) {
    int offset = 0;
    int type = struct_type;

    while (field != NULL) {
        int nth_field = find_struct_field_index_lexicographic(struct_type, field->index_lexicographic);
        if (nth_field == NULL_VALUE) {
            set_error_type(&error, RUN_TIME_ERROR);
            set_error_message(&error, "Corrupted IR: '%s' is not a field of structure '%s'.",
                              get_lexeme(field->index_lexicographic),
                              get_lexeme(get_declaration_lexicographic_index(struct_type)));
            yerror(error);
            exit(EXIT_FAILURE);
        }

        type = get_struct_nth_field_declaration(struct_type, nth_field);
        offset += get_struct_nth_field_execution(struct_type, nth_field);

        if (get_declaration_nature(type) == TYPE_ARRAY) {
            // An array field: either indexed (s.t[i], the node is an A_ARRAY_ACCESS) or used as
            // a whole (s.t passed as an argument).
            if (field->type == A_ARRAY_ACCESS) {
                return offset + array_element_offset(field, type, type_out);
            }
            break;
        }

        if (get_declaration_nature(type) == TYPE_STRUCT) struct_type = type;
        field = field->child;
    }

    if (type_out != NULL) *type_out = type;
    return offset;
}

int get_struct_field_address(Node *struct_field_access, int base_address, int index_declaration) {
    Node *field = struct_field_access;
    int struct_type = index_declaration;

    if (base_address == NULL_VALUE) {
        // The node is the structure variable itself: its fields start at its child.
        struct_type = get_declaration_description(struct_field_access->index_declaration);
        base_address = get_variable_address(struct_field_access->index_declaration);
        field = struct_field_access->child;
    }

    return base_address + struct_field_offset(field, struct_type, NULL);
}
