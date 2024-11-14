#include "../symbol_table/declaration_table.h" 
#include "../lexer/lexeme_table.h"
#include "validation.h"


void check_type_definition(int index_type_lexicographic) {
    if (find_declaration_index(index_type_lexicographic) == NULL_VALUE) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Type '%s' is not defined.", get_lexeme(index_type_lexicographic));

        yerror(error);
    }
}

void check_variable_definition(int index_lexeme_lexicographic) {
    if (find_declaration_index(index_lexeme_lexicographic) == NULL_VALUE) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Variable '%s' is not defined.", get_lexeme(index_lexeme_lexicographic));

        yerror(error);
    }
}


void check_variable_redefinition(int index_lexeme_lexicographic) {
    // FIXME: Should check in all stack regions or only in the current region?
    if (find_declaration_index(index_lexeme_lexicographic) != NULL_VALUE) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Redefinition of variable '%s'.", get_lexeme(index_lexeme_lexicographic));

        yywarn(error);
    }
}

void check_type_redefinition(int index_lexeme_lexicographic) {
    // FIXME: Should check in all stack regions or only in the current region?
    if (find_declaration_index(index_lexeme_lexicographic) != NULL_VALUE) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Redefinition of type '%s'.", get_lexeme(index_lexeme_lexicographic));

        yywarn(error);
    }
}