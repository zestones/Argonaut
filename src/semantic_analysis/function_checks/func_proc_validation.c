#include "../../symbol_table/representation/representation_table.h" 
#include "../../symbol_table/declaration/declaration_table.h" 
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

void check_func_prototype(int index_lexeme_lexicographic, Node *return_statement) {
    // functions return type can only be base types (int, float, char, bool)
    int base_type = index_lexeme_lexicographic;
    int expression_type = resolve_expression_type(return_statement->child);

    if (base_type != expression_type) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error, "Type mismatch: Function returned value has type '%s' but should be of type '%s'.",
                    (expression_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(expression_type), get_lexeme(base_type));
        yerror(error);
        return;
    }
}

void check_func_proc_argument_list(int index_lexeme_lexicographic, Node *argument_list) {
    int index_declaration = find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_FUNC);
    if (index_declaration == NULL_VALUE) index_declaration = find_declaration_index_by_nature(index_lexeme_lexicographic, TYPE_PROC);
    Nature nature = get_declaration_nature(index_declaration);

    int parameter_count = resolve_func_proc_parameter_count(index_lexeme_lexicographic, nature);

    // Check argument count validity first
    validate_argument_count(argument_list, parameter_count);

    // Validate argument types
    validate_each_argument(argument_list, parameter_count, index_lexeme_lexicographic, nature);
}
