#include "../../symbol_table/representation/representation_table.h" 
#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../lexer/lexeme_table.h"
#include "../../utils/utils.h"
#include "formatting.h"

static char *format_argument_list(Node *argument_list) {
    char *lexeme = (char *)malloc(256 * sizeof(char));
    lexeme[0] = '\0';

    Node *current_node = argument_list->child;
    while (current_node != NULL) {
        if (current_node->type == A_ARGUMENT) {
            // Allocate or reallocate memory for each new lexeme part
            char *expression_lexeme = resolve_expression_lexeme(current_node->child);
            size_t new_size = strlen(lexeme) + strlen(expression_lexeme) + 3; // +3 for comma, space and expression
            lexeme = (char *)realloc(lexeme, new_size);

            strcat(lexeme, expression_lexeme);
            if (current_node->sibling != NULL) strcat(lexeme, ", ");
        }

        current_node = current_node->sibling;
    }

    return lexeme;
}

char *format_func_proc_call(Node *func_proc_call) {
    char *lexeme = (char *)malloc(256 * sizeof(char));
    strcpy(lexeme, get_lexeme(func_proc_call->index_lexicographic));
    strcat(lexeme, "(");

    Node *current_node = func_proc_call->child;

    while (current_node != NULL) {
        if (current_node->type == A_ARGUMENT_LIST) {
            // Concatenate the resolved argument list lexeme
            char *argument_list_lexeme = format_argument_list(current_node);
            size_t new_size = strlen(lexeme) + strlen(argument_list_lexeme) + 1;
            lexeme = (char *)realloc(lexeme, new_size);

            strcat(lexeme, argument_list_lexeme);
            free(argument_list_lexeme);
        }

        current_node = current_node->sibling;
    }

    strcat(lexeme, ")");
    return lexeme;
}