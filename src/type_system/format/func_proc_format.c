#include "../../symbol_table/representation/representation_table.h"
#include "../../symbol_table/declaration/declaration_table.h"
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../utils/text_buffer.h"
#include "../../utils/utils.h"
#include "formatting.h"

/**
 * @brief Formats an argument list into a string representation.
 *
 * @param argument_list A pointer to the node representing the argument list to be formatted.
 * @return A dynamically allocated string representing the formatted argument list.
 *
 * @note The caller is responsible for freeing the returned string.
 */
static char *format_argument_list(Node *argument_list) {
    TextBuffer buffer = construct_text_buffer();

    Node *current_node = argument_list->child;
    while (current_node != NULL) {
        if (current_node->type == A_ARGUMENT) {
            char *expression_lexeme = format_expression(current_node->child);
            text_buffer_append(&buffer, expression_lexeme == NULL ? "?" : expression_lexeme);
            if (current_node->sibling != NULL) text_buffer_append(&buffer, ", ");
        }

        current_node = current_node->sibling;
    }

    return release_text_buffer(&buffer);
}

char *format_func_proc_call(Node *func_proc_call) {
    TextBuffer buffer = construct_text_buffer();
    text_buffer_append(&buffer, get_lexeme(func_proc_call->index_lexicographic));
    text_buffer_append(&buffer, "(");

    Node *current_node = func_proc_call->child;
    while (current_node != NULL) {
        if (current_node->type == A_ARGUMENT_LIST) {
            char *argument_list_lexeme = format_argument_list(current_node);
            text_buffer_append(&buffer, argument_list_lexeme);
            free(argument_list_lexeme);
        }

        current_node = current_node->sibling;
    }

    text_buffer_append(&buffer, ")");
    return release_text_buffer(&buffer);
}
