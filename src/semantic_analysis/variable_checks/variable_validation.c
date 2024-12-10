#include "../../symbol_table/declaration_table.h" 
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


int resolve_expression_type(Node *expression) {
    switch (expression->type) {
        case A_IDENTIFIER:
            return get_declaration_description(expression->index_declaration);

        case A_INTEGER_LITERAL:
            return A_INTEGER_LITERAL;

        case A_FLOAT_LITERAL:
            return A_FLOAT_LITERAL;

        case A_BOOLEAN_LITERAL:
            return A_BOOLEAN_LITERAL;

        case A_STRING_LITERAL:
            return A_STRING_LITERAL;

        case A_ADD_OP:
        case A_SUB_OP:
        case A_MUL_OP:
        case A_DIV_OP: {
            int left_type = resolve_expression_type(expression->child);
            int right_type = resolve_expression_type(expression->child->sibling);

            // Ensure both sides of the operation have the same type
            if (left_type != right_type) {
                set_error_type(&error, TYPE_ERROR);
                set_error_message(&error, "Type mismatch: Cannot apply operator to '%s' and '%s'.",
                                  (left_type == NULL_VALUE) ? "NO TYPE" : get_lexeme(left_type),
                                  (right_type == NULL_VALUE) ? "NO TYPE" : get_lexeme(right_type));
                yerror(error);
                return NULL_VALUE;
            }

            // Only numeric types are allowed for arithmetic operations
            if (left_type != A_INTEGER_LITERAL && left_type != A_FLOAT_LITERAL) {
                set_error_type(&error, TYPE_ERROR);
                set_error_message(&error, "Invalid type for arithmetic operation: '%s'.", get_lexeme(left_type));
                yerror(error);
                return NULL_VALUE;
            }

            return left_type;
        }

        case A_ARRAY_ACCESS:
            return resolve_array_access_type(expression);

        case A_STRUCT_FIELD_ACCESS:
            return resolve_struct_field_access_type(expression);

        case A_FUNC_PROC_CALL_STATEMENT:
            return resolve_func_proc_return_type(expression);

        default:
            return NULL_VALUE;
    }
}


void check_variable_assignment(int index_lexeme_lexicographic, Node *expression) {
    // Step 1: Retrieve the declaration index and type of the variable
    int index_lexeme_declaration = find_declaration_index(index_lexeme_lexicographic);
    int variable_type = get_declaration_description(index_lexeme_declaration);

    // Step 2: Resolve the type of the expression
    int expression_type = resolve_expression_type(expression);
    if (expression_type == NULL_VALUE) {
        set_error_type(&error, SEMANTIC_ERROR);
        set_error_message(&error, "Unable to resolve the type of the expression.");
        yerror(error);
        return;
    }

    // Step 3: Check type compatibility
    if (variable_type != expression_type) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error, "Type mismatch: Cannot assign expression of type '%s' to variable of type '%s'.",
                          get_lexeme(expression_type), get_lexeme(variable_type));
        yerror(error);
        return;
    }
}