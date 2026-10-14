
#include "../../symbol_table/declaration/declaration_table.h"
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../data/region_table.h"
#include "../../utils/memory.h"
#include "formatting.h"

/**
 * @brief Formats an arithmetic operation into a string representation.
 * 
 * This function takes an arithmetic operation represented by a node and formats it 
 * into a string expression. The resulting string is wrapped in parentheses with the 
 * left operand, the operator, and the right operand, properly formatted. The operator 
 * is derived from the node type.
 * 
 * @param expression A pointer to the node representing the arithmetic operation.
 * @return A dynamically allocated string representing the formatted arithmetic operation.
 * 
 * @note The caller is responsible for freeing the returned string.
 * 
 * @warning The function assumes that the expression is valid and that both operands 
 *          are properly formatted. Memory allocation failures during string concatenation 
 *          may lead to undefined behavior.
 */
static char *format_arithmetic_op(Node *expression) {
    const char *left = format_expression(expression->child);
    const char *right = format_expression(expression->child->sibling);
    if (left == NULL) left = "?";
    if (right == NULL) right = "?";

    size_t room = strlen(left) + strlen(right) + 8; // "(left op right)"
    char *lexeme = xmalloc(room);

    snprintf(lexeme, room, "(%s %c %s)", left, node_type_to_operator_char(expression->type), right);
    return lexeme;
}

static char *format_negation(Node *expression) {
    const char *operand = format_expression(expression->child);
    if (operand == NULL) operand = "?";

    size_t room = strlen(operand) + 4; // "(-x)"
    char *lexeme = xmalloc(room);

    snprintf(lexeme, room, "(-%s)", operand);
    return lexeme;
}

char *format_expression(Node *expression) {
    switch (expression->type) {
        case A_IDENTIFIER: return get_lexeme(expression->index_lexicographic);

        case A_INTEGER_LITERAL:
        case A_FLOAT_LITERAL:
        case A_BOOLEAN_LITERAL:
        case A_CHARACTER_LITERAL:
        case A_STRING_LITERAL: return get_lexeme(expression->index_lexicographic);

        case A_ADD_OP:
        case A_SUB_OP:
        case A_MUL_OP:
        case A_MODULO_OP:
        case A_DIV_OP: {
            return format_arithmetic_op(expression);
        }

        case A_NEG_OP: return format_negation(expression);

        case A_ARRAY_ACCESS:
        case A_STRUCT_FIELD_ACCESS: return format_access(expression);

        case A_FUNC_PROC_CALL_STATEMENT: return format_func_proc_call(expression);

        default: {
            set_error_type(&error, SEMANTIC_ERROR);
            set_error_message(&error, "Unexpected or unhandled expression type encountered: '%s'.",
                              NodeTypeStrings[expression->type]);
            yerror(error);
            return NULL;
        }
    }
}
