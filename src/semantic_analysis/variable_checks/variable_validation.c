#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"
#include "../semantic_checks.h"
#include "../../type_system/type_inference/type_inference.h"


int get_var_param_declaration_index(int index_lexeme_lexicographic) {
    int var_index = find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_VAR);
    int param_index = find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_PARAM);

    return (var_index > param_index) ? var_index : param_index;
}


void check_variable_definition(int index_lexeme_lexicographic) {
    if (find_declaration_index(index_lexeme_lexicographic) == NULL_VALUE) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, 
            "Undeclared variable at %s.\n"
            "  The variable '%s' is used before being defined or declared.\n"
            "  Ensure the variable is declared in the correct scope before use.\n",
            get_formatted_location(),
            get_lexeme(index_lexeme_lexicographic)
        );
        yerror(error);
    }
}

void check_variable_redefinition(int index_lexeme_lexicographic) {
    int index_lexeme_declaration = find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_VAR);
    if (index_lexeme_declaration != NULL_VALUE) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, 
            "Redefinition of variable '%s' at %s.\n"
            "  This variable has already been defined in the current scope.\n"
            "  Consider renaming or modifying the existing definition.\n",
            get_lexeme(index_lexeme_lexicographic),
            get_formatted_location()
        );
        
        int declaration_region = get_declaration_region(index_lexeme_declaration);
        int current_region = get_current_region_id();

        declaration_region == current_region ? yerror(error) : yywarn(error);
    }
}