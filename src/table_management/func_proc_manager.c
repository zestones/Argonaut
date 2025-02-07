#include "../symbol_table/representation/representation_table.h"
#include "../symbol_table/declaration/declaration_table.h"

#include "../data/region_stack.h"
#include "../data/region_table.h"

#include "func_proc_manager.h"
#include "variable_manager.h"

#include "../utils/scope_tracker.h"
#include "../utils/utils.h"

static func_proc_manager_context context;

void construct_func_proc_manager_context(int index_func_proc_name_lexicographic) {
    context.index_return_type_representation = NULL_VALUE;
    context.index_number_of_parameters_representation = NULL_VALUE;

    context.number_of_parameters = 0;

    context.index_return_type_lexicographic = NULL_VALUE;
    context.index_func_proc_name_lexicographic = index_func_proc_name_lexicographic;
    
    initialize_scope_identifier_tracker();
}

void func_proc_add_parameter(int index_lexeme_lexicographic, int index_type_lexicographic) {
    // The Parameter is a base type (int, float, char, etc) thus tdec index = tlex index //
    int index_type_declaration = index_lexeme_lexicographic;

    check_scope_redefinition(index_lexeme_lexicographic, "parameter");
    add_identifier_to_scope_tracker(index_lexeme_lexicographic);
    
    insert_representation(index_type_declaration); 
    insert_representation(index_type_lexicographic);
    
    context.number_of_parameters++;
    update_representation(context.index_number_of_parameters_representation, context.number_of_parameters);
    
    declaration_param_start(index_type_declaration, index_type_lexicographic);
}

void declaration_func_start() {
    check_func_proc_redefinition(context.index_func_proc_name_lexicographic, "function");
    context.index_return_type_representation = insert_representation(context.index_return_type_lexicographic);
    context.index_number_of_parameters_representation = insert_representation(context.number_of_parameters);

    int parent_index = peek_region();
    start_region();

    insert_declaration_func(context.index_func_proc_name_lexicographic, parent_index, context.index_return_type_representation);
}

void update_declaration_func_return_type(int index_return_type_lexicographic) {
    context.index_return_type_lexicographic = index_return_type_lexicographic;

    // check_base_type(context.index_return_type_lexicographic);
    update_representation(context.index_return_type_representation, context.index_return_type_lexicographic);
}

void declaration_proc_start() {
    check_func_proc_redefinition(context.index_func_proc_name_lexicographic, "procedure");
    context.index_number_of_parameters_representation = insert_representation(context.number_of_parameters);

    int parent_index = peek_region();
    start_region();

    insert_declaration_proc(context.index_func_proc_name_lexicographic, parent_index, context.index_number_of_parameters_representation);
}

void declaration_func_proc_end() {
    end_region();
    clear_scope_identifier_tracker();
}