#include "../../symbol_table/representation/representation_table.h" 
#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../data/region_table.h"

int resolve_func_proc_return_type(Node *function_call) {
    int nature = get_declaration_nature(function_call->index_declaration);

    if (nature == TYPE_FUNC) {
        int index_declaration = function_call->index_declaration;
        int index_representation = get_declaration_description(index_declaration);
        return get_representation_value(index_representation);
    }

    return NULL_VALUE;
}

// TODO: refactor move to symbol_table/utility.c
static int resolve_procedure_parameter_count(int index_lexeme_lexicographic) {
    int index_declaration = find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_PROC);
    int index_representation_start = get_declaration_description(index_declaration);
    return get_representation_value(index_representation_start); 
}

// TODO: refactor move to symbol_table/utility.c
static int resolve_function_parameter_count(int index_lexeme_lexicographic) {
    int index_declaration = find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_FUNC);
    int index_representation_start = get_declaration_description(index_declaration);
    return get_representation_value(index_representation_start + 1);
}

int resolve_func_proc_parameter_count(int index_lexeme_lexicographic, Nature nature) {
    if (nature == TYPE_FUNC) {
        return resolve_function_parameter_count(index_lexeme_lexicographic);
    } 

    return resolve_procedure_parameter_count(index_lexeme_lexicographic);
}

int resolve_func_proc_parameter_type_at(int num_param, int index_declaration, Nature nature) {
    int shift = (nature == TYPE_FUNC) ? 1 : 0;

    int index_representation_start = get_declaration_description(index_declaration);
    int param_type_index = index_representation_start + 2 + (num_param * 2) + shift;
    
    return get_representation_value(param_type_index);
}