#include "../stack_management/stack_management.h"

#include "assignement/assignement.h"
#include "condition/condition.h"

#include "../core/execution.h"
#include "interpreter.h"

static void resolve_declaration_list(Node *declaration_list) {
    if (declaration_list == NULL) return;

    Node *declaration = declaration_list->child;
    while (declaration != NULL) {
        handle_variable_declaration(declaration->type, declaration->index_lexicographic, declaration->index_declaration);
        declaration = declaration->sibling;
    }
}

void resolve_statement_list(AST statement_list) {
    if (statement_list == NULL) return;

    switch (statement_list->type) {
        case A_ASSIGNMENT_STATEMENT:
            resolve_assignement(statement_list);
            break;

        case A_IF:
        case A_IF_ELSE:
            execute_condition(statement_list->child);
            break;
        
        case A_WHILE:
            execute_loop(statement_list);
            break;

        default:
            resolve_statement_list(statement_list->child);
            resolve_statement_list(statement_list->sibling);
            break;
    }

    resolve_statement_list(statement_list->sibling);
}


void interpret_ast(AST ast) {
    if (ast == NULL) return;

    // Process the current node based on its type
    switch (ast->type) {
        case A_DECLARATION_LIST:
            resolve_declaration_list(ast);
            break;

        case A_STATEMENT_LIST:
            resolve_statement_list(ast);
            break;

        default: {
            interpret_ast(ast->child);
            interpret_ast(ast->sibling);
            break;
        }
    }

    interpret_ast(ast->sibling);
}


static int execute(int region_index) {

    push_region(region_index);
    interpret_ast(get_region_ast(region_index));
    pop_region();

    fprintf_vm_stack(stdout); 
}

void interpret() {
    init_execution_stack(); 
    init_stack_region();

    for (int i = 0; i <= get_current_region_id(); i++) {
        if (get_region_size(i) > 0) {
            execute(i);
        }
    }
}
