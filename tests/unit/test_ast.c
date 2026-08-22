#include "test.h"

#include "ast/ast.h"

TEST(construct_node_initialises_every_field) {
    Node *n = construct_node(A_INTEGER_LITERAL, 4, 7);
    ASSERT_NOT_NULL(n);
    ASSERT_EQ_INT(A_INTEGER_LITERAL, n->type);
    ASSERT_EQ_INT(4, n->index_lexicographic);
    ASSERT_EQ_INT(7, n->index_declaration);
    ASSERT_NULL(n->child);
    ASSERT_NULL(n->sibling);
    free_ast(n);

    Node *d = construct_node_default(A_PROGRAM);
    ASSERT_EQ_INT(-1, d->index_lexicographic);
    ASSERT_EQ_INT(-1, d->index_declaration);
    free_ast(d);
}

TEST(add_child_prepends_and_append_child_appends) {
    Node *parent = construct_node_default(A_STATEMENT_LIST);
    Node *a = construct_node_default(A_PRINT_STATEMENT);
    Node *b = construct_node_default(A_INPUT_STATEMENT);
    Node *c = construct_node_default(A_BREAK);

    add_child(parent, a);
    add_child(parent, b); /* prepends: b, a */
    ASSERT_TRUE(parent->child == b);
    ASSERT_TRUE(b->sibling == a);

    append_child(parent, c); /* appends: b, a, c */
    ASSERT_TRUE(a->sibling == c);
    ASSERT_NULL(c->sibling);
    free_ast(parent);
}

TEST(add_sibling_walks_to_the_end_of_the_chain) {
    Node *a = construct_node_default(A_IF);
    Node *b = construct_node_default(A_WHILE);
    Node *c = construct_node_default(A_FOR_LOOP);
    add_sibling(a, b);
    add_sibling(a, c);
    ASSERT_TRUE(a->sibling == b);
    ASSERT_TRUE(b->sibling == c);
    ASSERT_NULL(c->sibling);
    free_ast(a);
}

TEST(add_chain_skips_null_nodes) {
    Node *parent = construct_node_default(A_FUNCTION_DECLARATION);
    Node *params = construct_node_default(A_PARAMETER_LIST);
    Node *stmts = construct_node_default(A_STATEMENT_LIST);
    Node *nodes[] = {params, NULL, stmts};
    add_chain(parent, nodes, 3);
    ASSERT_TRUE(parent->child == params);
    ASSERT_TRUE(params->sibling == stmts);
    ASSERT_NULL(stmts->sibling);
    free_ast(parent);
}

TEST(null_helpers_are_safe) {
    ASSERT_TRUE(is_node_null(NULL));
    add_child(NULL, NULL);
    add_sibling(NULL, NULL);
    append_child(NULL, NULL);
    ASSERT_NULL(format_ast(NULL));
    free_ast(NULL);
}

TEST(format_ast_serialises_the_lcrs_shape) {
    Node *root = construct_node(A_PROGRAM, -1, -1);
    Node *child = construct_node(A_DECLARATION_LIST, -1, -1);
    Node *sib = construct_node(A_STATEMENT_LIST, -1, -1);
    add_child(root, child);
    add_sibling(child, sib);
    char *s = format_ast(root);
    ASSERT_NOT_NULL(s);
    ASSERT_EQ_STR("Node(6, -1, -1) [Child: Node(23, -1, -1) [Sibling: Node(33, -1, -1)]]", s);
    free(s);
    free_ast(root);
}
