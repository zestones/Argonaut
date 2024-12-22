#include "../../../data/region_table.h"

#include "../../stack_management/stack_management.h"
#include "../../core/execution.h"

#include "../expression/expression.h"


void resolve_assignement(Node *assignement_start) {
    if (assignement_start == NULL) return;

    Node *assignement = assignement_start->child;
    if (assignement->type == A_VARIABLE_ASSIGNMENT) {
        vm_cell cell = resolve_expression(assignement->child);
        handle_variable_affectation(assignement->index_declaration, cell);
    }
}