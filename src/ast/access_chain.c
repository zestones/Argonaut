#include "../utils/utils.h"
#include "access_chain.h"

static Node *next_in_chain(Node *node) {
    if (node->type == A_ARRAY_ACCESS) return node->child ? node->child->sibling : NULL;
    return node->child;
}

Node *access_tail(Node *root) {
    Node *node = root;
    for (Node *next = next_in_chain(node); next != NULL; next = next_in_chain(node)) {
        node = next;
    }
    return node;
}

void access_chain_field(Node *root, int field_lexeme) {
    Node *tail = access_tail(root);
    Node *field = construct_node(A_STRUCT_FIELD_ACCESS, field_lexeme, NULL_VALUE);

    if (tail->type == A_ARRAY_ACCESS) add_sibling(tail->child, field); // after the index list
    else add_child(tail, field);
}

void access_chain_index(Node *root, Node *index_list) {
    Node *tail = access_tail(root);

    if (tail->type == A_STRUCT_FIELD_ACCESS) {
        // The field itself is indexed (s.t[i]): the field node becomes an array access node.
        tail->type = A_ARRAY_ACCESS;
        add_child(tail, index_list);
        return;
    }

    // An element of an array is indexed again (g[i][j]).
    Node *level = construct_node(A_ARRAY_ACCESS, NULL_VALUE, NULL_VALUE);
    add_child(level, index_list);
    add_sibling(tail->child, level);
}
