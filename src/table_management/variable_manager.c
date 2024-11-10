#include "../symbol_table/declaration_table.h"
#include "../data/region_table.h"
#include "variable_manager.h"
#include "../utils/utils.h"

void declaration_variable_start(int index_lexeme_lexicographic, int index_type_lexicographic) {
    // search for the index of the type inside the declaration table
    int current_nis = get_current_nis();

    insert_declaration_var(index_lexeme_lexicographic, current_nis, index_type_lexicographic);

    update_region_size(current_nis, get_region_size(current_nis));
}