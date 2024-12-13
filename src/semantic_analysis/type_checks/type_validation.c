#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"
#include "../semantic_checks.h"

int get_arr_struct_declaration_index(int index_type_lexicographic) {
    int arr_index = find_declaration_index_by_nature(index_type_lexicographic, TYPE_ARRAY);
    int struct_index = find_declaration_index_by_nature(index_type_lexicographic, TYPE_STRUCT);

    int index = (arr_index > struct_index) ? arr_index : struct_index;

    return (index != NULL_VALUE) ? index : NULL_VALUE;
}

void check_base_type(int index_lexeme_lexicographic) {
       if(!is_declaration_base_type(index_lexeme_lexicographic)) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error, "Type '%s' should be a base type (int, float, bool, char).", get_lexeme(index_lexeme_lexicographic));

        yerror(error);
    }   
}

void check_type_definition(int index_type_lexicographic) {
    if (get_arr_struct_declaration_index(index_type_lexicographic) == NULL_VALUE) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error, 
            "Undefined type at %s.\n"
            "  The type '%s' is not defined in the current scope.\n"
            "  Ensure the type is properly declared before use.\n",
            get_formatted_location(),
            get_lexeme(index_type_lexicographic)
        );
        yerror(error);
    }
}

void check_type_redefinition(int index_lexeme_lexicographic, Nature nature) {
    int index_lexeme_declaration = find_declaration_index_by_nature(index_lexeme_lexicographic, nature);
    
    if (index_lexeme_declaration != NULL_VALUE && peek_region() == get_declaration_region(index_lexeme_declaration)) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error, "Redefinition of the %s : '%s'.", 
            nature_to_string(nature), 
            get_lexeme(index_lexeme_lexicographic)
        );

        int declaration_region = get_declaration_region(index_lexeme_declaration);
        int current_region = get_current_region_id();

        declaration_region == current_region ? yerror(error) : yywarn(error);
    }

}