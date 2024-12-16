#include "../../symbol_table/representation/representation_table.h"
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../symbol_table/utility.h"
#include "../../data/region_table.h"

#include "../stack_management/stack_management.h"
#include "../core/execution.h"
#include "interpreter.h"

static void resolve_declaration_list(Node *declaration_list) {
    if (declaration_list == NULL) return;

    Node *declaration = declaration_list->child;
    while (declaration != NULL) {
        handle_declaration(declaration);
        declaration = declaration->sibling;
    }
}

static void interpret_ast(AST ast) {
    if (ast == NULL) return;

    switch (ast->type) {
        case A_DECLARATION_LIST:
            resolve_declaration_list(ast);
            break;
        default:
            interpret_ast(ast->child);
            interpret_ast(ast->sibling);
    }
}

static int execute(int region_index) {
    AST ast = get_region_ast(region_index);
    interpret_ast(ast);
    fprintf_vm_stack(stdout); 
}

void interpret() {
    init_execution_stack(); 

    for (int i = 0; i <= get_current_region_id(); i++) {
        if (get_region_size(i) > 0) {
            execute(i);
        }
    }
}
