#include "../../../symbol_table/lexeme/lexeme_table.h"

#include "../../core/vm_cell.h"
#include "../../core/execution.h"

#include "../assignement/assignement.h"
#include "../expression/expression.h"
#include "../interpreter.h"
#include "loop.h"

static ControlFlow execute_while_loop(AST ast) {
    vm_cell condition = resolve_boolean_expression(ast->child);

    while (condition.value.boolean) {
        ControlFlow flow = resolve_statement_list(ast->child->sibling);

        if (flow == CONTROL_BREAK) break;
        if (flow == CONTROL_RETURN) return CONTROL_RETURN;
        // CONTROL_CONTINUE and CONTROL_NONE both move on to the next iteration

        condition = resolve_boolean_expression(ast->child);
    }

    return CONTROL_NONE;
}

static ControlFlow execute_for_loop(AST ast) {
    AST initialization = ast->child;
    AST condition = initialization->sibling;
    AST update = condition->sibling;
    AST statement_block = update->sibling;

    resolve_assignement(initialization);

    vm_cell cond_result = resolve_boolean_expression(condition);
    while (cond_result.value.boolean) {
        ControlFlow flow = resolve_statement_list(statement_block);

        if (flow == CONTROL_BREAK) break;
        if (flow == CONTROL_RETURN) return CONTROL_RETURN;

        resolve_assignement(update);
        cond_result = resolve_boolean_expression(condition);
    }

    return CONTROL_NONE;
}

ControlFlow execute_loop(AST ast) {
    if (ast->type == A_WHILE) return execute_while_loop(ast);
    if (ast->type == A_FOR_LOOP) return execute_for_loop(ast);
    return CONTROL_NONE;
}