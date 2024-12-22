#include "../../../data/region_table.h"
#include "../../../symbol_table/utility.h"
#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../stack_management/stack_management.h"
#include "../../core/address/address_calculation.h"
#include "../../core/execution.h"

#include "../expression/expression.h"


void resolve_assignement(Node *assignement_start) {
    if (assignement_start == NULL) return;

    Node *assignement = assignement_start->child;
    if (assignement->type == A_VARIABLE_ASSIGNMENT) {
        vm_cell cell = resolve_expression(assignement->child);
        handle_variable_affectation(assignement->index_declaration, cell);
    } else if (assignement->type == A_ARRAY_ASSIGNMENT) {
        vm_cell cell = resolve_expression(assignement->child->sibling);
        handle_array_affectation(assignement->child, cell);
    } else if (assignement->type == A_STRUCT_ASSIGNMENT) {
        vm_cell cell = resolve_expression(assignement->child->sibling);
        handle_structure_affectation(assignement->child, cell);
    }
}