#include "../stack_management/frame/stack_frame.h"
#include "../stack_management/stack_management.h"

#include "assignement/assignement.h"
#include "expression/expression.h"
#include "condition/condition.h"
#include "func_proc/func_proc.h"

#include "../core/execution.h"
#include "interpreter.h"

/**
 * @brief Resolves a list of variable declarations.
 * 
 * This function processes a list of declarations (typically found in a program's 
 * declaration section) and handles the declaration of each variable. It iterates through 
 * the list of declarations, calling `handle_variable_declaration` for each variable 
 * declaration encountered.
 * 
 * @param declaration_list A pointer to the AST node representing the list of declarations.
 */
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
    if (statement_list == NULL || get_return_cell().is_initialized) return;   

    switch (statement_list->type) {
        case A_ASSIGNMENT_STATEMENT:
            resolve_assignement(statement_list);
            break;

        case A_IF:
        case A_IF_ELSE:
        case A_IF_ELSE_IF:
            execute_condition(statement_list);
            break;

        case A_INPUT_STATEMENT:
            execute_input(statement_list);
            break;

        case A_PRINT_STATEMENT:
            execute_print(statement_list);
            break;
            
        case A_FOR_LOOP:
        case A_WHILE:
            execute_loop(statement_list);
            break;

        case A_FUNC_PROC_CALL_STATEMENT: {
            execute_func_proc_call(statement_list);
            break;
        }

        case A_RETURN_STATEMENT: {
            handle_function_return_value(resolve_expression(statement_list->child));
            break;
        }

        default:
            resolve_statement_list(statement_list->child);
            resolve_statement_list(statement_list->sibling);
            break;
    }

    resolve_statement_list(statement_list->sibling);
}

/**
 * @brief Interprets an Abstract Syntax Tree (AST) of the program.
 * 
 * This function is the entry point for interpreting an AST representing a program or 
 * a block of code. It recursively processes the AST nodes and resolves declarations, 
 * executes statements, and handles function returns. The function dispatches the execution 
 * based on the type of AST node, calling relevant functions for declarations, statements, 
 * and returns.
 * 
 * @param ast A pointer to the root AST node to be interpreted.
 */
static void interpret_ast(AST ast) {
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

/**
 * @brief Executes the global program (entry point).
 * 
 * This function starts the execution of the global program by pushing the global region 
 * to the execution stack and interpreting the corresponding AST. After the execution, 
 * it pops the region and the frame from the stack.
 */
static void execute_global_program() {
    int program_region = 0;

    push_region(program_region);
    stack_frame frame = construct_stack_frame(program_region, program_region);
    push_frame_to_execution_stack(frame);

    interpret_ast(get_region_ast(program_region));
}

void interpret() {
    init_execution_stack(); 
    init_stack_region();

    execute_global_program();
}
