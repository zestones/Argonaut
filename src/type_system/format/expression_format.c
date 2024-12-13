
#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../lexer/lexeme_table.h"
#include "../../data/region_table.h"
#include "formatting.h"


char *resolve_expression_lexeme(Node *expression) {
    switch (expression->type) {
        case A_IDENTIFIER:
            return get_lexeme(expression->index_lexicographic);

        case A_INTEGER_LITERAL:
        case A_FLOAT_LITERAL:
        case A_BOOLEAN_LITERAL:
        case A_CHARACTER_LITERAL:
        case A_STRING_LITERAL:
            return get_lexeme(expression->index_lexicographic);

        case A_ADD_OP:
        case A_SUB_OP:
        case A_MUL_OP:
        case A_DIV_OP: {
            char *lexeme = (char *)malloc(MAX_LEXEME_LENGTH * sizeof(char));
            strcpy(lexeme, "(");
            strcat(lexeme, resolve_expression_lexeme(expression->child));
            strcat(lexeme, " ");
            strcat(lexeme, get_lexeme(expression->index_lexicographic));
            strcat(lexeme, " ");
            strcat(lexeme, resolve_expression_lexeme(expression->child->sibling));
            strcat(lexeme, ")");
            return lexeme;
        }

        case A_ARRAY_ACCESS:
            return format_array_access(expression);

        case A_STRUCT_FIELD_ACCESS: 
            return get_lexeme(expression->index_lexicographic);

        case A_FUNC_PROC_CALL_STATEMENT: 
            return get_lexeme(expression->index_lexicographic);

        default: {
            set_error_type(&error, SEMANTIC_ERROR);
            set_error_message(
                &error,
                "Unexpected or unhandled expression type encountered: '%s'.",
                get_lexeme(expression->type)
            );
            yerror(error);
            return NULL;
        }
    }
}