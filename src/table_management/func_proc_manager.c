#include "../../lib/table_printer.h"
#include "../../lib/colors.h" 

#include "../symbol_table/representation_table.h"
#include "../symbol_table/declaration_table.h"
#include "../data/region_table.h"
#include "func_proc_manager.h"
#include "../utils/utils.h"

static func_proc_manager_context context;
static int current_nis = 0;

void construct_func_proc_manager_context(int index_func_proc_name_lexicographic) {
    context.index_return_type_representation = NULL_VALUE;
    context.index_number_of_parameters_representation = NULL_VALUE;

    context.number_of_parameters = 0;

    context.index_return_type_lexicographic = NULL_VALUE;
    context.index_func_proc_name_lexicographic = index_func_proc_name_lexicographic;
}

void func_proc_add_parameter(int index_lexeme_lexicographic, int index_type_lexicographic) {
    // The Parameter is a base type (int, float, char, etc) thus tdec index = tlex index //
    int index_type_declaration = index_lexeme_lexicographic;
    
    insert_representation(index_type_lexicographic);
    insert_representation(index_type_declaration); 
    
    context.number_of_parameters++;

    insert_declaration_param(index_lexeme_lexicographic, index_type_lexicographic);
}

void declaration_func_start() {
    current_nis++;
    insert_region(current_nis);

    context.index_number_of_parameters_representation = insert_representation(context.number_of_parameters);
    context.index_return_type_representation = insert_representation(context.index_return_type_lexicographic);

    insert_declaration_func(context.index_func_proc_name_lexicographic, context.index_number_of_parameters_representation);
}

void declaration_func_end(int index_return_type_lexicographic) {
    context.index_return_type_lexicographic = index_return_type_lexicographic;
    
    update_representation(context.index_return_type_representation, context.index_return_type_lexicographic);
    update_representation(context.index_number_of_parameters_representation, context.number_of_parameters);

    current_nis--;
}

void declaration_proc_start() {
    current_nis++;
    insert_region(current_nis);

    context.index_number_of_parameters_representation = insert_representation(context.number_of_parameters);
    insert_declaration_proc(context.index_func_proc_name_lexicographic, context.index_number_of_parameters_representation);
}

void declaration_proc_end() {
    update_representation(context.index_number_of_parameters_representation, context.number_of_parameters);
    current_nis--;
}