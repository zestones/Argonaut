#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../core/vm_cell.h"
#include "../../core/execution.h"

#include "../expression/expression.h"
#include "../interpreter.h"
#include "condition.h"

void execute_condition(AST ast) {
    vm_cell condition = resolve_boolean_expression(ast);

    if (condition.value.boolean) {
        resolve_statement_list(ast->sibling->child);
    }
    else if (ast->sibling->sibling && ast->sibling->sibling->child) {
        resolve_statement_list(ast->sibling->sibling->child);
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