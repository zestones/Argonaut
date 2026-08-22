#ifndef __ACCESS_CHAIN_H__
#define __ACCESS_CHAIN_H__

#include "lcrs.h"

/**
 * @file access_chain.h
 * @brief Builds the AST of an access chain such as `t[i].f.g[j][k]`.
 *
 * The root node names the variable: an A_ARRAY_ACCESS when the first postfix is an index list,
 * an A_STRUCT_FIELD_ACCESS when it is a field. Then, along the chain:
 * - a field is an A_STRUCT_FIELD_ACCESS node (lexeme = field name), or an A_ARRAY_ACCESS node
 *   when the field is indexed (`s.t[i]`);
 * - an A_ARRAY_ACCESS node keeps its index list (A_ARRAY_INDEX_LIST) as first child and the next
 *   postfix as the sibling of that list;
 * - an extra `[...]` applied to an element that is itself an array (`g[i][j]`) is an
 *   A_ARRAY_ACCESS node without lexeme (NULL_VALUE);
 * - a field node continues the chain through its child.
 */

/**
 * @brief The last node of an access chain.
 */
Node *access_tail(Node *root);

/**
 * @brief Appends a `.field` postfix to an access chain.
 */
void access_chain_field(Node *root, int field_lexeme);

/**
 * @brief Appends a `[indices]` postfix to an access chain.
 */
void access_chain_index(Node *root, Node *index_list);

#endif // __ACCESS_CHAIN_H__
