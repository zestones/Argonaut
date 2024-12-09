#include "../symbol_table/representation_table.h" 
#include "../symbol_table/declaration_table.h" 
#include "../lexer/lexeme_table.h"
#include "../data/region_table.h"

int resolve_function_call_type(Node *function_call) {
    int index_declaration = function_call->child->index_declaration;
    int index_representation = get_declaration_description(index_declaration);
    
    return get_representation_value(index_representation);
}

int resolve_function_parameter_count(int index_lexeme_lexicographic) {
    int index_declaration = find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_FUNC);
    int index_representation_start = get_declaration_description(index_declaration);
    return get_representation_value(index_representation_start + 1);
}

int resolve_function_parameter_type_at(int num_param, int index_representation_start) {
    int param_type_index = index_representation_start + 2 + (num_param * 2) + 1;
    return get_representation_value(param_type_index);
}