#include "../stack_management/frame/stack_frame.h"
#include "../stack_management/stack_management.h"

#include "assignement/assignement.h"
#include "expression/expression.h"
#include "condition/condition.h"
#include "func_proc/func_proc.h"

#include "../core/execution.h"
#include "interpreter.h"

static void resolve_declaration_list(Node *declaration_list) {
    if (declaration_list == NULL) return;

    Node *declaration = declaration_list->child;
    while (declaration != NULL) {
        if (declaration->type == A_VARIABLE_DECLARATION) {
            handle_variable_declaration(declaration->index_declaration);
        }
        
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

        case A_INPUT_STATEMENT:
            execute_input(statement_list);
            break;

        case A_PRINT_STATEMENT:
            execute_print(statement_list);
            break;
        
        case A_WHILE:
            execute_loop(statement_list);
            break;

        case A_FUNC_PROC_CALL_STATEMENT: {
            execute_func_proc_call(statement_list);
            break;
        }

        default:
            resolve_statement_list(statement_list->child);
            resolve_statement_list(statement_list->sibling);
            break;
    }

    resolve_statement_list(statement_list->sibling);
}


void interpret_ast(AST ast) {
    if (ast == NULL) return;

    switch (ast->type) {
        case A_DECLARATION_LIST:
            resolve_declaration_list(ast);
            break;

        case A_STATEMENT_LIST:
            resolve_statement_list(ast);
            break;

        case A_PROCEDURE_DECLARATION:
        case A_FUNCTION_DECLARATION:
            // Skip function and procedure declarations
            // Go directly to the next statement 
            if (ast->child->type == A_PARAMETER_LIST) interpret_ast(ast->child->sibling);
            else interpret_ast(ast->child);
            break;

        case A_RETURN_STATEMENT: {
            handle_function_return_value(resolve_expression(ast->child));
            break;
        }

        default: {
            interpret_ast(ast->child);
            interpret_ast(ast->sibling);
            break;
        }
    }

    interpret_ast(ast->sibling);
}

void execute(int region_index) {
    push_region(region_index);

    interpret_ast(get_region_ast(region_index));
    
    pop_region();
    pop_frame_from_execution_stack();
}

static void execute_global_program() {
    int program_region = 0;

    push_region(program_region);
    stack_frame frame = construct_stack_frame(program_region, program_region, program_region);
    push_frame_to_execution_stack(frame);

    interpret_ast(get_region_ast(program_region));
}

void interpret() {
    init_execution_stack(); 
    init_stack_region();

    execute_global_program();

    // fprintf_vm_stack(stdout); 
}
