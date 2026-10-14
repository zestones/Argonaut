#include "../../symbol_table/declaration/declaration_table.h"
#include "../../type_system/type_inference/type_inference.h"
#include "../../type_system/format/formatting.h"
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../data/region_table.h"
#include "../semantic_checks.h"


void check_variable_assignment(int index_lexeme_lexicographic, Node *expression) {
    // Step 1: Retrieve the declaration index and type of the variable
    int index_lexeme_declaration = get_var_param_declaration_index(index_lexeme_lexicographic);
    int variable_type = get_declaration_description(index_lexeme_declaration);
    int variable_type_lexeme_index = get_declaration_lexicographic_index(variable_type);

    // Step 2: Resolve the type of the expression
    int expression_type = resolve_expression_type(expression);

    // Step 3: Check type compatibility
    if (variable_type != expression_type) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error,
                          "Type mismatch during assignement at %s.\n"
                          "  Cannot assign expression of type '%s' to variable '%s' of type '%s'.\n"
                          "  Ensure that the expression type matches the expected type for the variable.\n",
                          get_formatted_location(), type_name(expression_type),
                          (variable_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(index_lexeme_lexicographic),
                          (variable_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(variable_type_lexeme_index));
        yerror(error);
        return;
    }
}

void check_access_assignment(Node *access, Node *expression) {
    // Step 1: Resolve the type of the target (errors inside the chain are already reported)
    int target_type = resolve_access_type(access);
    if (target_type == NULL_VALUE) return;

    // Step 2: Resolve the type of the expression
    int expression_type = resolve_expression_type(expression);
    if (expression_type == NULL_VALUE) return;

    // Step 3: Check type compatibility
    if (target_type != expression_type) {
        char *target = format_access(access);
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error,
                          "Type mismatch during assignement at %s.\n"
                          "  Cannot assign expression of type '%s' to '%s' of type '%s'.\n"
                          "  Ensure the expression type matches the expected type of the target.\n",
                          get_formatted_location(), type_name(expression_type), target, type_name(target_type));
        yerror(error);
        free(target);
    }
}
