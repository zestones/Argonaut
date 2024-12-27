#include "../../stack_management/frame/stack_frame.h"
#include "../../stack_management/stack_management.h"

#include "../../../data/region_table.h"

#include "../../core/address/address_calculation.h"
#include "../../core/execution.h"

#include "../expression/expression.h"
#include "../interpreter.h"
#include "func_proc.h"

/**
 * @brief Helper function to handle parameters in function and procedure calls.
 * We prepare the stack frame for the function or procedure call by handling the parameters.
 * By allocating memory for the parameters and updating the values in the stack frame, we 
 * ensure that the function or procedure can access the arguments passed to it.
 * 
 * @param parameter_list 
 * @param argument_list 
 */
static void handle_parameters(AST parameter_list, AST argument_list) {
    if (parameter_list->type != A_PARAMETER_LIST || argument_list->type != A_ARGUMENT_LIST) return;

    Node *param_node = parameter_list->child;
    Node *arg_node = argument_list->child;
    stack_frame *current_frame = peek_execution_stack_as_mutable();

    while (param_node != NULL && arg_node != NULL) {
        if (param_node->type == A_PARAMETER && arg_node->type == A_ARGUMENT) {
            handle_variable_declaration(param_node->index_declaration);

            vm_cell cell = resolve_expression(arg_node->child);
            int address = get_variable_address(param_node->index_declaration);

            update_cell_in_stack_frame(current_frame, address, cell);
        }

        param_node = param_node->sibling;
        arg_node = arg_node->sibling;
    }
}

void execute_func_proc_call(AST ast) {
    if (ast == NULL) return;

    int static_link;
    int dynamic_link = get_execution_stack_current_frame_id();
    int target_region_index = get_declaration_region(ast->index_declaration);

    stack_frame current_frame = peek_execution_stack();
    stack_frame frame = construct_stack_frame(target_region_index, dynamic_link, get_declaration_execution(ast->index_declaration));

    push_frame_to_execution_stack(frame);
    
    int index_region = get_declaration_execution(ast->index_declaration);
    AST parameter_list = get_region_ast(index_region)->child;

    handle_parameters(parameter_list, ast->child);
    execute(index_region);
}