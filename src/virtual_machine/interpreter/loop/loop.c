#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../core/vm_cell.h"
#include "../../core/execution.h"

#include "../assignement/assignement.h"
#include "../expression/expression.h"
#include "../interpreter.h"
#include "loop.h"

static void execute_while_loop(AST ast) {
    vm_cell condition = resolve_boolean_expression(ast->child->child);

    while (condition.value.boolean) {
        if (get_return_cell().is_initialized) return;
        ControlFlow flow = resolve_statement_list(ast->child->sibling);

        if (flow == CONTROL_BREAK) break;
        else if (flow == CONTROL_CONTINUE) {
            condition = resolve_boolean_expression(ast->child->child);
            continue;
        }

        condition = resolve_boolean_expression(ast->child->child);
    }
}

static void execute_for_loop(AST ast) {
    AST initialization = ast->child;
    AST condition = initialization->sibling;
    AST update = condition->sibling;
    AST statement_block = update->sibling;

    resolve_assignement(initialization);

    vm_cell cond_result = resolve_boolean_expression(condition);
    while (cond_result.value.boolean) {
        if (get_return_cell().is_initialized) return;
        ControlFlow flow = resolve_statement_list(statement_block);

        if (flow == CONTROL_BREAK) break;
        else if (flow == CONTROL_CONTINUE) {
            resolve_assignement(update);
            cond_result = resolve_boolean_expression(condition);
            continue;
        }

        resolve_assignement(update);
        cond_result = resolve_boolean_expression(condition);
    }
}

void execute_loop(AST ast) {
    if (ast->type == A_WHILE) execute_while_loop(ast);
    else if (ast->type == A_FOR_LOOP) execute_for_loop(ast);
}