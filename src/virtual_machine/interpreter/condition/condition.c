#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../core/vm_cell.h"
#include "../../core/execution.h"

#include "../expression/expression.h"
#include "../interpreter.h"
#include "condition.h"


void execute_condition(AST ast) {
    Node *current_node = ast;

    while (current_node != NULL) {
        vm_cell condition = resolve_boolean_expression(current_node->child);

        if (current_node->type == A_IF_ELSE_IF || current_node->type == A_IF_ELSE) {
            if (condition.value.boolean) {
                resolve_statement_list(current_node->child->sibling->child);
                break;
            }

            // Move to the next branch (else if or else)
            Node *next_branch = current_node->child->sibling->sibling;
            if (current_node->type == A_IF_ELSE && next_branch && next_branch->child) {
                resolve_statement_list(next_branch->child);
                break;
            }
            current_node = next_branch;
        } else {
            printf("Unexpected node type: %d\n", current_node->type);
            break;
        }
    }
}


void execute_loop(AST ast) {
    int is_while = (ast->type == A_WHILE);
    vm_cell condition = resolve_boolean_expression(ast->child->child);

    while (is_while && condition.value.boolean) {
        resolve_statement_list(ast->child->sibling); 
        condition = resolve_boolean_expression(ast->child->child); 
    }
}