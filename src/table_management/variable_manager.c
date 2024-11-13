#include "../../lib/colors.h"

#include "../symbol_table/declaration_table.h"
#include "../lexer/lexeme_table.h"
#include "../data/region_table.h"
#include "variable_manager.h"
#include "../utils/utils.h"

void declaration_variable_start(int index_lexeme_lexicographic, int index_type_lexicographic) {
    int index_type_declaration = find_declaration_index(index_type_lexicographic);
    int execution = get_declaration_execution(index_type_declaration); 
    
    insert_declaration_var(index_lexeme_lexicographic, get_current_region_id(), index_type_declaration, get_region_size(get_current_region_id()));
    update_region_size(get_current_region_id(), get_region_size(get_current_region_id()) + execution);
}

void declaration_param_start(int index_lexeme_lexicographic, int index_type_lexicographic) {
    int index_type_declaration = find_declaration_index(index_type_lexicographic);
    int execution = get_declaration_execution(index_type_declaration); 
    
    insert_declaration_param(index_lexeme_lexicographic, get_current_region_id(), index_type_declaration, get_region_size(get_current_region_id()));
    update_region_size(get_current_region_id(), get_region_size(get_current_region_id()) + execution);
}

void validate_variable_definition(int index_lexeme_lexicographic) {
    if (find_declaration_index(index_lexeme_lexicographic) == NULL_VALUE) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Variable '%s' is not defined.", get_lexeme(index_lexeme_lexicographic));

        yerror(error);
    }
}