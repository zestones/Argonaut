#ifndef __AST_H__
#define __AST_H__

#include "node_type.h"
#include "lcrs.h"

// AST Root Type
typedef struct Node* AST;

/**
 * @brief Creates an empty AST.
 * @return An empty AST (root node is NULL).
 */
AST construct_ast();

/**
 * @brief Prints the AST in a human-readable format.
 * 
 * @param ast The root of the AST to print.
 */
void print_ast(AST ast);

/**
 * @brief Frees all memory associated with the AST.
 * 
 * @param ast The root of the AST to free.
 */
void free_ast(AST ast);

#endif // __AST_H__
