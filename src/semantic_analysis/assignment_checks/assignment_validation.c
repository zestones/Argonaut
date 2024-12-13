#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../type_system/type_inference/type_inference.h"
#include "../../type_system/format/formatting.h"
#include "../../lexer/lexeme_table.h"
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
            "Type mismatch at during assignement %s.\n"
            "  Cannot assign expression of type '%s' to variable '%s' of type '%s'.\n"
            "  Ensure that the expression type matches the expected type for the variable.\n",
            get_formatted_location(),
            (expression_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(expression_type),
            (variable_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(index_lexeme_lexicographic),
            (variable_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(variable_type_lexeme_index)
        );
        yerror(error);
        return;
    }
}

void check_array_assignment(Node *array, Node *expression) {
    // Step 1: Resolve the type of the array
    int array_type = resolve_array_access_type(array);

    // Step 2: Resolve the type of the expression
    int expression_type = resolve_expression_type(expression);

    // Step 3: Check type compatibility
    if (array_type != expression_type) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error, 
            "Type mismatch at during assignement %s.\n"
            "  Cannot assign expression of type '%s' to array '%s' of type '%s'.\n"
            "  Please ensure the expression type is compatible with the array type.\n",
            get_formatted_location(),
            (expression_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(expression_type),
            (array_type == NULL_VALUE) ? "UNKNOWN" : format_array_access(array),
            (array_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(array_type)
        );
        yerror(error);
        return;
    }
}

void check_struct_assignment(Node *structure, Node *expression) {
    // Step 1: Resolve the type of the structure field access
    int struct_field_type = resolve_struct_field_access_type(structure);

    // Step 2: Resolve the type of the expression
    int expression_type = resolve_expression_type(expression);

    // Step 3: Check type compatibility
    if (struct_field_type != expression_type) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error, 
            "Type mismatch at during assignement %s.\n"
            "  Cannot assign expression of type '%s' to struct field '%s' of type '%s'.\n"
            "  Ensure the expression type matches the expected type for the struct field.\n",
            get_formatted_location(),
            (expression_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(expression_type),
            (struct_field_type == NULL_VALUE) ? "UNKNOWN" : format_struct_access(structure),
            (struct_field_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(struct_field_type)
        );
        yerror(error);
        return;
    }
}
