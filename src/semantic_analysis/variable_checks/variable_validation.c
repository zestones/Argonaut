#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"
#include "../semantic_checks.h"
#include "../../type_system/type_system.h"

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

void check_variable_assignment(int index_lexeme_lexicographic, Node *expression) {
    // Step 1: Retrieve the declaration index and type of the variable
    int index_lexeme_declaration = find_declaration_index(index_lexeme_lexicographic);
    int variable_type = get_declaration_description(index_lexeme_declaration);

    // Step 2: Resolve the type of the expression
    int expression_type = resolve_expression_type(expression);

    // Step 3: Check type compatibility
    if (variable_type != expression_type) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error, "Type mismatch: Cannot assign expression of type '%s' to variable of type '%s'.",
                    (expression_type == NULL_VALUE) ? "NO TYPE" : get_lexeme(expression_type), get_lexeme(variable_type));
        yerror(error);
        return;
    }
}