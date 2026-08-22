#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../core/vm_cell.h"
#include "../../core/execution.h"

#include "../assignement/assignement.h"
#include "../expression/expression.h"
#include "../interpreter.h"
#include "condition.h"


ControlFlow execute_condition(AST ast) {
    // Shape of an if node: child = condition, condition->sibling = then block (A_STATEMENT_LIST),
    // then_block->sibling = NULL (A_IF), an else block (A_IF_ELSE) or the next if node (A_IF_ELSE_IF).
    Node *node = ast;

    while (node != NULL && (node->type == A_IF || node->type == A_IF_ELSE || node->type == A_IF_ELSE_IF)) {
        Node *condition = node->child;
        Node *then_block = condition->sibling;
        Node *else_part = then_block->sibling;

        vm_cell result = resolve_boolean_expression(condition);
        if (result.value.boolean) {
            return resolve_statement_list(then_block->child);
        }

        if (node->type == A_IF_ELSE) {
            return resolve_statement_list(else_part ? else_part->child : NULL);
        }

        node = else_part; // A_IF_ELSE_IF: evaluate the next if; A_IF: NULL, nothing to do
    }

    return CONTROL_NONE;
}

