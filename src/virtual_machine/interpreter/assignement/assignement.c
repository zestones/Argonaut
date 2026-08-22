#include "../../../data/region_table.h"
#include "../../../symbol_table/utility.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../stack_management/stack_management.h"
#include "../../core/address/address_calculation.h"
#include "../../core/execution.h"

#include "../expression/expression.h"


/**
 * @brief Copies a structure or an array into another one of the same type.
 *
 * @param target_type The declaration index of the target's (aggregate) type.
 * @param target_frame The frame of the target, and the address of its first cell.
 * @param value The lvalue node of the source aggregate.
 */
static void assign_aggregate(int target_type, stack_frame *target_frame, int target_address, Node *value) {
    int size = get_declaration_execution(target_type);
    copy_cells(target_frame, target_address, get_lvalue_frame(value), get_lvalue_address(value), size);
}

static int is_aggregate(int type_declaration) {
    int nature = get_declaration_nature(type_declaration);
    return nature == TYPE_STRUCT || nature == TYPE_ARRAY;
}

static int is_lvalue(Node *node) {
    return node != NULL && (node->type == A_IDENTIFIER || node->type == A_ARRAY_ACCESS || node->type == A_STRUCT_FIELD_ACCESS);
}

void resolve_assignement(Node *assignement_start) {
    if (assignement_start == NULL) return;

    Node *assignement = assignement_start->child;
    if (assignement->type == A_VARIABLE_ASSIGNMENT) {
        int target_type = get_declaration_description(assignement->index_declaration);
        if (is_aggregate(target_type) && is_lvalue(assignement->child)) {
            stack_frame *frame = find_stack_frame_by_region_index(get_declaration_region(assignement->index_declaration));
            assign_aggregate(target_type, frame, get_variable_address(assignement->index_declaration), assignement->child);
            return;
        }

        vm_cell cell = resolve_expression(assignement->child);
        handle_variable_affectation(assignement->index_declaration, cell);
    } else if (assignement->type == A_ARRAY_ASSIGNMENT || assignement->type == A_STRUCT_ASSIGNMENT) {
        Node *target = assignement->child;
        Node *value = target->sibling;

        int target_type = get_lvalue_type(target);
        if (is_aggregate(target_type) && is_lvalue(value)) {
            assign_aggregate(target_type, get_lvalue_frame(target), get_lvalue_address(target), value);
            return;
        }

        vm_cell cell = resolve_expression(value);
        if (assignement->type == A_ARRAY_ASSIGNMENT) handle_array_affectation(target, cell);
        else handle_structure_affectation(target, cell);
    }
}