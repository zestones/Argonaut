#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../utils/text_buffer.h"
#include "../../utils/utils.h"
#include "formatting.h"

static void append_index_list(TextBuffer *buffer, Node *index_list) {
    text_buffer_append(buffer, "[");
    for (Node *index = index_list->child; index != NULL; index = index->sibling) {
        char *expression = format_expression(index->child);
        text_buffer_append(buffer, expression ? expression : "?");
        if (index->sibling != NULL) text_buffer_append(buffer, ", ");
    }
    text_buffer_append(buffer, "]");
}

char *format_access(Node *access) {
    TextBuffer buffer = construct_text_buffer();
    text_buffer_append(&buffer, get_lexeme(access->index_lexicographic));

    Node *node = access;
    while (node != NULL) {
        if (node->type == A_ARRAY_ACCESS) {
            if (node != access && node->index_lexicographic != NULL_VALUE) {
                text_buffer_append(&buffer, ".");
                text_buffer_append(&buffer, get_lexeme(node->index_lexicographic));
            }
            append_index_list(&buffer, node->child);
            node = node->child->sibling;
        } else {
            if (node != access) {
                text_buffer_append(&buffer, ".");
                text_buffer_append(&buffer, get_lexeme(node->index_lexicographic));
            }
            node = node->child;
        }
    }

    return release_text_buffer(&buffer);
}
