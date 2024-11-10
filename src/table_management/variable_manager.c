#include "../symbol_table/declaration_table.h"
#include "variable_manager.h"
#include "../utils/utils.h"


void declaration_variable_start(int index_lexeme_lexicographic, int index_type_lexicographic) {
    insert_declaration_var(index_lexeme_lexicographic, current_nis, index_type_lexicographic);
}