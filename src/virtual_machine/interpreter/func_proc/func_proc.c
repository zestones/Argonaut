#include "../../stack_management/frame/stack_frame.h"
#include "../../stack_management/stack_management.h"

#include "../../../symbol_table/utility.h"
#include "../../../data/region_table.h"

#include "../../core/address/address_calculation.h"
#include "../../core/execution.h"

#include "../expression/expression.h"
#include "../interpreter.h"
#include "func_proc.h"

/**
 * @brief Constructs and initializes a stack frame for function or procedure calls with parameters.
 * 
 * This function creates a new stack frame for a function or procedure call and processes 
 * the provided parameter and argument lists. It ensures that each parameter in the function 
 * or procedure has a corresponding allocated memory slot in the stack frame, with its value 
 * updated based on the resolved argument expression.
 * 
 * @param parameter_list A pointer to the AST node representing the parameter list of the function or procedure.
 * @param argument_list A pointer to the AST node representing the argument list provided during the function or procedure call.
 * @param region_index An integer representing the region index associated with the stack frame.
 * 
 * @return A stack frame populated with parameters and their corresponding argument values.
 */

static stack_frame construct_frame_with_parameters(AST parameter_list, AST argument_list, int region_index) {
    int dynamic_link = get_execution_stack_current_frame_id();
    stack_frame current_frame = construct_stack_frame(dynamic_link, region_index);
    
    if (parameter_list->type != A_PARAMETER_LIST || argument_list->type != A_ARGUMENT_LIST) {
        return current_frame;
    }

    Node *param_node = parameter_list->child;
    Node *arg_node = argument_list->child;

    while (param_node != NULL && arg_node != NULL) {
        if (param_node->type == A_PARAMETER && arg_node->type == A_ARGUMENT) {
            int index_type_declaration = get_declaration_description(param_node->index_declaration);
            declare_variable(&current_frame, index_type_declaration);
            if (is_declaration_base_type(index_type_declaration)) {
                vm_cell cell = resolve_expression(arg_node->child);
                int address = get_variable_address(param_node->index_declaration);
                update_cell_in_stack_frame(&current_frame, address, cell);
            }
            else if (get_declaration_nature(index_type_declaration) == TYPE_STRUCT) {
                int base_address         = get_variable_address(arg_node->child->index_declaration);
                int size                 = get_declaration_execution(index_type_declaration);
                int address              = get_variable_address(param_node->index_declaration);

                for (int i = 0; i < size; i++) {
                    vm_cell cell = get_cell_from_stack_frame(peek_execution_stack(), base_address + i);
                    update_cell_in_stack_frame(&current_frame, address + i, cell);
                    // TODO: handle nested array in struct
                }
            }
            else if (get_declaration_nature(index_type_declaration) == TYPE_ARRAY) {
                // TODO: handle arrays as param
                printf("idnex : %d\n", index_type_declaration);
            }
        }

        param_node = param_node->sibling;
        arg_node = arg_node->sibling;
    }

    return current_frame;
}

void execute_func_proc_call(AST ast) {
    if (ast == NULL) return;

    int region_index = get_declaration_execution(ast->index_declaration);
    AST parameter_list = get_region_ast(region_index)->child;

    stack_frame frame = construct_frame_with_parameters(parameter_list, ast->child, region_index);
    push_frame_to_execution_stack(frame);

    execute(region_index);
}