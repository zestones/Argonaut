#include "../../symbol_table/representation_table.h" 
#include "../../symbol_table/declaration_table.h" 
#include "../../type_system/type_system.h"
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"

void check_func_proc_definition(int index_lexeme_lexicographic) {
    // TODO: the procedure call should not be assigned to a variable
    if (find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_FUNC) == NULL_VALUE && 
        find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_PROC) == NULL_VALUE) {

        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Function or procedure '%s' is not defined.", get_lexeme(index_lexeme_lexicographic));

        yerror(error);
    }
}

void check_func_proc_redefinition(int index_lexeme_lexicographic, char *type) {
    // TODO: Change condition to 'OR' if we don't want to allow function and procedure with the same name 
    if (find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_FUNC) != NULL_VALUE &&
        find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_PROC) != NULL_VALUE) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Redefinition of %s '%s'.", type, get_lexeme(index_lexeme_lexicographic));

        yerror(error);
    }
}

void check_func_argument_list(int index_lexeme_lexicographic, Node *argument_list) {
    int parameter_count = resolve_function_parameter_count(index_lexeme_lexicographic);

    // Check argument count validity first
    validate_argument_count(argument_list, parameter_count);

    // Validate argument types
    int index_representation_start = get_declaration_description(find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_FUNC));
    validate_each_argument(argument_list, index_representation_start, parameter_count, index_lexeme_lexicographic);
}
