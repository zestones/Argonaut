
#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../data/region_table.h"
#include "formatting.h"


char *format_arithmetic_op(Node *expression) {
    char *lexeme = (char *)malloc(MAX_LEXEME_LENGTH * sizeof(char));
    strcpy(lexeme, "(");
    strcat(lexeme, format_expression(expression->child));
    strcat(lexeme, " ");

    // Convert the operator char to a string
    char op[2] = {node_type_to_operator_char(expression->type), '\0'};
    strcat(lexeme, op);

    strcat(lexeme, " ");
    strcat(lexeme, format_expression(expression->child->sibling));
    strcat(lexeme, ")");

    return lexeme;
}

char *format_expression(Node *expression) {
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
            return format_arithmetic_op(expression);
        }

        case A_ARRAY_ACCESS:
            return format_array_access(expression);

        case A_STRUCT_FIELD_ACCESS: 
            return format_struct_access(expression);

        case A_FUNC_PROC_CALL_STATEMENT: 
            return format_func_proc_call(expression); 

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