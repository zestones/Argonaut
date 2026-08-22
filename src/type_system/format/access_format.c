#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../utils/utils.h"
#include "formatting.h"

/**
 * @brief A growable text buffer used to print an access chain.
 */
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} text;

static void append(text *t, const char *s) {
    size_t n = strlen(s);
    if (t->length + n + 1 > t->capacity) {
        size_t capacity = t->capacity ? t->capacity : 64;
        while (t->length + n + 1 > capacity) capacity *= 2;
        char *data = realloc(t->data, capacity);
        if (data == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        t->data = data;
        t->capacity = capacity;
    }
    memcpy(t->data + t->length, s, n + 1);
    t->length += n;
}

static void append_index_list(text *t, Node *index_list) {
    append(t, "[");
    for (Node *index = index_list->child; index != NULL; index = index->sibling) {
        char *expression = format_expression(index->child);
        append(t, expression ? expression : "?");
        if (index->sibling != NULL) append(t, ", ");
    }
    append(t, "]");
}

char *format_access(Node *access) {
    text t = {NULL, 0, 0};
    append(&t, "");
    append(&t, get_lexeme(access->index_lexicographic));

    Node *node = access;
    while (node != NULL) {
        if (node->type == A_ARRAY_ACCESS) {
            if (node != access && node->index_lexicographic != NULL_VALUE) {
                append(&t, ".");
                append(&t, get_lexeme(node->index_lexicographic));
            }
            append_index_list(&t, node->child);
            node = node->child->sibling;
        } else {
            if (node != access) {
                append(&t, ".");
                append(&t, get_lexeme(node->index_lexicographic));
            }
            node = node->child;
        }
    }

    return t.data;
}
