#include "../../lib/colors.h"

#include "../symbol_table/declaration_table.h"
#include "../data/region_table.h"
#include "variable_manager.h"
#include "../utils/utils.h"

void declaration_variable_start(int index_lexeme_lexicographic, int index_type_lexicographic) {
    int current_nis = get_current_region_nis();

    int index_type_declaration = find_declaration_index(index_type_lexicographic, current_nis);
    int execution = get_declaration_execution(index_type_declaration); 
    
    insert_declaration_var(index_lexeme_lexicographic, get_current_region_id(), index_type_declaration, get_region_size(get_current_region_id()));
    update_region_size(get_current_region_id(), get_region_size(get_current_region_id()) + execution);
}

void declaration_param_start(int index_lexeme_lexicographic, int index_type_lexicographic) {
    int current_nis = get_current_region_nis();

    int index_type_declaration = find_declaration_index(index_type_lexicographic, get_current_region_id());
    int execution = get_declaration_execution(index_type_declaration); 
    
    insert_declaration_param(index_lexeme_lexicographic, get_current_region_id(), index_type_declaration, get_region_size(get_current_region_id()));
    update_region_size(get_current_region_id(), get_region_size(get_current_region_id()) + execution);
}