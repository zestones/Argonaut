
#include "../symbol_table/declaration/declaration_table.h" 
#include "../lexer/lexeme_table.h"
#include "../data/region_table.h"
#include "type_system.h"

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
                set_error_message(
                    &error,
                    "Type mismatch during operation '%s': left operand type '%s', right operand type '%s'.",
                    NodeTypeStrings[expression->type],
                    (left_type == NULL_VALUE) ? "NO TYPE" : get_lexeme(left_type),
                    (right_type == NULL_VALUE) ? "NO TYPE" : get_lexeme(right_type)
                );
                yerror(error);
                return NULL_VALUE;
            }

            // Only numeric types are allowed for arithmetic operations
            if (left_type != A_INTEGER_LITERAL && left_type != A_FLOAT_LITERAL) {
                set_error_type(&error, TYPE_ERROR);
                set_error_message(
                    &error,
                    "Invalid type for arithmetic operation '%s': operand type '%s'.",
                    NodeTypeStrings[expression->type],
                    get_lexeme(left_type)
                );
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

        default: {
            set_error_type(&error, SEMANTIC_ERROR);
            set_error_message(
                &error,
                "Unexpected or unhandled expression type encountered: '%s'.",
                get_lexeme(expression->type)
            );
            yerror(error);
            return NULL_VALUE;
        }
    }
}