#include "../../symbol_table/declaration/declaration_table.h"
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../data/region_table.h"
#include "type_inference.h"

int resolve_expression_type(Node *expression);

/**
 * @brief Type of a comparison: both operands must have the same base type; booleans only
 *        support == and !=. The result is bool.
 */
static int resolve_comparison_type(Node *comparison) {
    int left_type = resolve_expression_type(comparison->child);
    int right_type = resolve_expression_type(comparison->child->sibling);
    int operator= comparison->type;
    if (left_type == NULL_VALUE || right_type == NULL_VALUE) return NULL_VALUE;

    if (left_type != right_type) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error,
                          "Type mismatch in condition at %s.\n"
                          "  Left operand type: '%s', right operand type: '%s'.\n"
                          "  Ensure both operands are of compatible types for the condition.\n",
                          get_formatted_location(), type_name(left_type), type_name(right_type));
        yerror(error);
        return NULL_VALUE;
    }

    if (left_type != A_INTEGER_LITERAL && left_type != A_FLOAT_LITERAL && left_type != A_BOOLEAN_LITERAL &&
        left_type != A_CHARACTER_LITERAL) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error,
                          "Invalid operand type in condition at %s: '%s'.\n"
                          "  Only int, float, bool and char values can be compared.\n",
                          get_formatted_location(), type_name(left_type));
        yerror(error);
        return NULL_VALUE;
    }

    if (left_type == A_BOOLEAN_LITERAL && (operator== A_GREATER_EQUAL_OP || operator== A_LESS_EQUAL_OP || operator==
                                           A_GREATER_THAN_OP || operator== A_LESS_THAN_OP)) {
        set_error_type(&error, TYPE_ERROR);
        set_error_message(&error,
                          "Invalid comparison at %s.\n"
                          "  Boolean cannot be compared with '%s' operator.\n"
                          "  Ensure the operator is used with compatible types.\n",
                          get_formatted_location(), node_type_to_comparison_operator(operator));
        yerror(error);
        return NULL_VALUE;
    }

    return A_BOOLEAN_LITERAL;
}

const char *type_name(int type) {
    if (type == STRING_LITERAL_TYPE) return "string";
    if (type == NULL_VALUE) return "UNKNOWN";
    return get_lexeme(type);
}

int resolve_expression_type(Node *expression) {
    switch (expression->type) {
        case A_IDENTIFIER: return get_declaration_description(expression->index_declaration);

        case A_INTEGER_LITERAL: return A_INTEGER_LITERAL;

        case A_FLOAT_LITERAL: return A_FLOAT_LITERAL;

        case A_BOOLEAN_LITERAL: return A_BOOLEAN_LITERAL;

        case A_CHARACTER_LITERAL: return A_CHARACTER_LITERAL;

        case A_STRING_LITERAL: return STRING_LITERAL_TYPE;

        case A_ADD_OP:
        case A_SUB_OP:
        case A_MUL_OP:
        case A_MODULO_OP:
        case A_DIV_OP: {
            int left_type = resolve_expression_type(expression->child);
            int right_type = resolve_expression_type(expression->child->sibling);

            // Ensure both sides of the operation have the same type
            if (left_type != right_type) {
                set_error_type(&error, TYPE_ERROR);
                set_error_message(&error,
                                  "Type mismatch during operation '%c' at %s.\n"
                                  "  Left operand type: '%s', right operand type: '%s'.\n"
                                  "  Ensure both operands are of compatible types for the operation.\n",
                                  node_type_to_operator_char(expression->type), get_formatted_location(),
                                  type_name(left_type), type_name(right_type));
                yerror(error);
                return NULL_VALUE;
            }

            // Only numeric types are allowed for arithmetic operations
            if (left_type != A_INTEGER_LITERAL && left_type != A_FLOAT_LITERAL) {
                set_error_type(&error, TYPE_ERROR);
                // TODO: impossible to trigger ? Handled by previous checks ?
                set_error_message(&error, "Invalid type for arithmetic operation '%s': operand type '%s'.",
                                  NodeTypeStrings[expression->type], type_name(left_type));
                yerror(error);
                return NULL_VALUE;
            }

            return left_type;
        }

        case A_NEG_OP: {
            int operand_type = resolve_expression_type(expression->child);
            if (operand_type == NULL_VALUE) return NULL_VALUE;
            if (operand_type != A_INTEGER_LITERAL && operand_type != A_FLOAT_LITERAL) {
                set_error_type(&error, TYPE_ERROR);
                set_error_message(&error,
                                  "Invalid operand for unary minus at %s.\n"
                                  "  The operand has type '%s', but only int and float can be negated.\n",
                                  get_formatted_location(), type_name(operand_type));
                yerror(error);
                return NULL_VALUE;
            }
            return operand_type;
        }

        case A_EQUAL_OP:
        case A_NOT_EQUAL_OP:
        case A_LESS_THAN_OP:
        case A_GREATER_THAN_OP:
        case A_LESS_EQUAL_OP:
        case A_GREATER_EQUAL_OP: return resolve_comparison_type(expression);

        case A_AND_CONDITION:
        case A_OR_CONDITION: {
            int left_type = resolve_expression_type(expression->child);
            int right_type = resolve_expression_type(expression->child->sibling);
            if (left_type == NULL_VALUE || right_type == NULL_VALUE) return NULL_VALUE;

            if (left_type != A_BOOLEAN_LITERAL || right_type != A_BOOLEAN_LITERAL) {
                set_error_type(&error, TYPE_ERROR);
                set_error_message(
                    &error,
                    "Logical operators ('AND', 'OR') require boolean operands at %s, but received '%s' and '%s'.",
                    get_formatted_location(), type_name(left_type), type_name(right_type));
                yerror(error);
                return NULL_VALUE;
            }
            return A_BOOLEAN_LITERAL;
        }

        case A_NOT_CONDITION: {
            int operand_type = resolve_expression_type(expression->child);
            if (operand_type == NULL_VALUE) return NULL_VALUE;

            if (operand_type != A_BOOLEAN_LITERAL) {
                set_error_type(&error, TYPE_ERROR);
                set_error_message(&error, "NOT operator requires a boolean operand at %s, but received '%s'.",
                                  get_formatted_location(), type_name(operand_type));
                yerror(error);
                return NULL_VALUE;
            }
            return A_BOOLEAN_LITERAL;
        }

        case A_ARRAY_ACCESS:
        case A_STRUCT_FIELD_ACCESS: return resolve_access_type(expression);

        case A_FUNC_PROC_CALL_STATEMENT: return resolve_func_proc_return_type(expression);

        default: {
            // ! should never reach this point
            set_error_type(&error, SEMANTIC_ERROR);
            set_error_message(&error,
                              "Unexpected expression type '%s' at %s.\n"
                              "  The encountered expression type is not handled or supported.\n"
                              "  Please verify the expression type or ensure proper handling.\n",
                              NodeTypeStrings[expression->type], get_formatted_location());
            yerror(error);
            return NULL_VALUE;
        }
    }
}