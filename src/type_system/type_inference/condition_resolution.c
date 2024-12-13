#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"
#include "type_inference.h"


int resolve_condition_type(Node *condition) {
    switch (condition->type) {
        case A_CONDITION: {
            int left_type = resolve_expression_type(condition->child->child);
            int right_type = resolve_expression_type(condition->child->child->sibling);
            int operator = condition->child->type;

            // Ensure the operands are of compatible types
            if (left_type != right_type) {
                set_error_type(&error, TYPE_ERROR);
                set_error_message(
                    &error,
                    "Type mismatch in condition: left operand type '%s', right operand type '%s'.",
                    get_lexeme(left_type),
                    get_lexeme(right_type)
                );
                yerror(error);
                return NULL_VALUE;
            }

            // Comparisons must be between valid types (e.g., integers, floats)
            if (left_type != A_INTEGER_LITERAL && left_type != A_FLOAT_LITERAL &&
                left_type != A_BOOLEAN_LITERAL && left_type != A_CHARACTER_LITERAL) {
                set_error_type(&error, TYPE_ERROR);
                set_error_message(
                    &error,
                    "Invalid operand type in condition: '%s'.",
                    get_lexeme(left_type)
                );
                yerror(error);
                return NULL_VALUE;
            }

            if (left_type == A_BOOLEAN_LITERAL && 
                (operator == A_GREATER_EQUAL_OP || operator == A_LESS_EQUAL_OP
                || operator == A_GREATER_THAN_OP || operator == A_LESS_THAN_OP)) {
                set_error_type(&error, TYPE_ERROR);
                set_error_message(
                    &error,
                    "Invalid comparison at %s.\n"
                    "  Boolean cannot be compared with '%s' operator.\n"
                    "  Ensure the operator is used with compatible types.\n",
                    get_formatted_location(),
                    node_type_to_comparison_operator(operator)
                );
                yerror(error);
                return NULL_VALUE;
            }


            return A_BOOLEAN_LITERAL;
        }

        case A_AND_CONDITION:
        case A_OR_CONDITION: {
            int left_type = resolve_condition_type(condition->child);
            int right_type = resolve_condition_type(condition->child->sibling);

            if (left_type != A_BOOLEAN_LITERAL || right_type != A_BOOLEAN_LITERAL) {
                set_error_type(&error, TYPE_ERROR);
                set_error_message(
                    &error,
                    "Logical operators ('AND', 'OR') require boolean operands, but received '%s' and '%s'.",
                    (left_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(left_type),
                    (right_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(right_type)
                );
                yerror(error);
                return NULL_VALUE;
            }

            return A_BOOLEAN_LITERAL;
        }

        case A_NOT_CONDITION: {
            int operand_type = resolve_condition_type(condition->child);

            if (operand_type != A_BOOLEAN_LITERAL) {
                set_error_type(&error, TYPE_ERROR);
                set_error_message(
                    &error,
                    "NOT operator requires a boolean operand, but received '%s'.",
                    (operand_type == NULL_VALUE) ? "UNKNOWN" : get_lexeme(operand_type)
                );
                yerror(error);
                return NULL_VALUE;
            }

            return A_BOOLEAN_LITERAL;
        }

        default: {
            set_error_type(&error, SEMANTIC_ERROR);
            set_error_message(
                &error,
                "Unexpected or unhandled condition type encountered: '%s'.",
                get_lexeme(condition->type)
            );
            yerror(error);
            return NULL_VALUE;
        }
    }
}
