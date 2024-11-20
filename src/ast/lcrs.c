#include "../utils/utils.h"

#include "lcrs.h"

Node* construct_node(NodeType type, int index_lexicographic, int index_declaration) {
    Node* new_node = (Node*) malloc(sizeof(Node));
    if (!new_node) {
        perror("Failed to allocate memory for AST node");
        exit(EXIT_FAILURE);
    }

    new_node->type = type;

    new_node->index_lexicographic = index_lexicographic;
    new_node->index_declaration = index_declaration;
    
    new_node->child = NULL;
    new_node->sibling = NULL;

    return new_node;
}

Node* construct_node_default(NodeType type) {
    return construct_node(type, NULL_VALUE, NULL_VALUE);
}

void add_child(Node* parent, Node* child) {
    if (!parent || !child) return;
    child->sibling = parent->child;
    parent->child = child;
}

void add_sibling(Node* node, Node* sibling) {
    if (!node || !sibling) return;
    while (node->sibling) {
        node = node->sibling;
    }

    node->sibling = sibling;
}

int is_node_null(Node* node) {
    return node == NULL;
}