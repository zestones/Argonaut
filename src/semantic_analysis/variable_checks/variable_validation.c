#include "../../symbol_table/declaration_table.h" 
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"
#include "../semantic_checks.h"

void check_variable_definition(int index_lexeme_lexicographic) {
    if (find_declaration_index(index_lexeme_lexicographic) == NULL_VALUE) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Variable '%s' is not defined.", get_lexeme(index_lexeme_lexicographic));

        yerror(error);
    }
}

void check_variable_redefinition(int index_lexeme_lexicographic) {
    int index_lexeme_declaration = find_declaration_index(index_lexeme_lexicographic);
    if (index_lexeme_declaration != NULL_VALUE) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Redefinition of variable '%s'.", get_lexeme(index_lexeme_lexicographic));
        
        int declaration_region = get_declaration_region(index_lexeme_declaration);
        int current_region = get_current_region_id();

        declaration_region == current_region ? yerror(error) : yywarn(error);
    }
}