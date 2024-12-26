#include "../../stack_management/frame/stack_frame.h"
#include "../../stack_management/stack_management.h"

#include "../../../data/region_table.h"

#include "../../core/address/address_calculation.h"
#include "../../core/execution.h"

#include "../expression/expression.h"
#include "../interpreter.h"
#include "func_proc.h"


static void handle_parameters(AST parameter_list, AST argument_list) {
    if (parameter_list->type != A_PARAMETER_LIST || argument_list->type != A_ARGUMENT_LIST) return;

    Node *param_node = parameter_list->child;
    Node *arg_node = argument_list->child;
    stack_frame *current_frame = peek_execution_stack_as_mutable();

    // Traverse both trees simultaneously
    while (param_node != NULL && arg_node != NULL) {
        if (param_node->type == A_PARAMETER && arg_node->type == A_ARGUMENT) {
            // Handle the parameter declaration
            handle_variable_declaration(param_node->index_lexicographic, param_node->index_declaration);

            // Resolve the argument's expression
            vm_cell cell = resolve_expression(arg_node->child);

            // Get the variable address for the declared parameter
            int address = get_parameter_address(param_node->index_declaration);

            // Update the stack frame with the resolved value
            update_cell_in_stack_frame(current_frame, address, cell);
        }

        // Move to the next parameter and argument
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