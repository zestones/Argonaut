#ifndef __AST_H__
#define __AST_H__

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    A_PROGRAM,

    A_CONDITION,
    A_AND_CONDITION,
    A_OR_CONDITION,
    A_NOT_CONDITION,
    A_NOT_EXPRESSION,

    A_ADDITION,
    A_SUBTRACTION,
    A_MULTIPLICATION,
    A_DIVISION,

    A_EQUAL_OP,
    A_NOT_EQUAL_OP,
    A_LESS_THAN_OP,
    A_GREATER_THAN_OP,
    A_LESS_EQUAL_OP,
    A_GREATER_EQUAL_OP,

    A_ADD_OP,
    A_SUB_OP,
    A_MUL_OP,
    A_DIV_OP,

    A_DECLARATION_LIST,
    A_VARIABLE_DECLARATION,
    A_FUNCTION_DECLARATION,
    A_PROCEDURE_DECLARATION,
    A_TYPE_DECLARATION,

    A_INTEGER_LITERAL,
    A_FLOAT_LITERAL,
    A_BOOLEAN_LITERAL,
    A_CHARACTER_LITERAL,
    A_STRING_LITERAL,
    A_IDENTIFIER,

    A_STATEMENT_LIST,
    A_ASSIGNMENT_STATEMENT,
    A_FUNCTION_CALL_STATEMENT,
    A_LOOP_STATEMENT,
    A_VARIABLE_ASSIGNMENT,
    A_ARRAY_ASSIGNMENT,
    A_STRUCT_ASSIGNMENT,

    A_IF,
    A_IF_ELSE,
    A_WHILE,

    A_FUNCTION_CALL,
    A_ARRAY_ACCESS,
    A_ARRAY_INDEX_LIST,
    A_ARRAY_INDEX,
    A_ARRAY_FIELD_ACCESS,
    A_STRUCT_ARRAY_ACCESS,
    A_STRUCT_FIELD_ACCESS,

    A_PRINT_STATEMENT,
    A_INPUT_STATEMENT,

    A_ASSIGNABLE_ENTITY,
    A_ARRAY_ASSIGNABLE_ENTITY,
    A_STRUCT_ASSIGNABLE_ENTITY,
    A_ASSIGNABLE_ENTITY_LIST,

    A_ARGUMENT_LIST,
    A_PARAMETER_LIST,
    A_PARAMETER,
} NodeType;

typedef struct Node {
    struct Node* child;            // Pointer to the leftmost child
    struct Node* sibling;          // Pointer to the right sibling
    
    NodeType type;                      // Node type (operator, variable, literal, etc.)
    int index_lexicographic;       // Index in the lexicographic table
    int index_declaration;         // Index in the declaration table
} Node;

// AST Root Type
typedef struct Node* AST;

/**
 * @brief Creates an empty AST.
 * @return An empty AST (root node is NULL).
 */
AST construct_ast();

/**
 * @brief Creates a new AST node.
 * 
 * @param type Type of the node.
 * @param index_lexicographic Index in the lexicographic table (if applicable).
 * @param index_declaration Index in the declaration table (if applicable).
 * @return A pointer to the newly created node.
 */
Node* construct_node(NodeType type, int index_lexicographic, int index_declaration);

/**
 * @brief Adds a child node to the given parent node.
 * 
 * @param parent The parent node to which the child will be added.
 * @param child The child node to add.
 */
void add_child(Node* parent, Node* child);

/**
 * @brief Adds a sibling node to the given node.
 * 
 * @param node The node to which the sibling will be added.
 * @param sibling The sibling node to add.
 */
void add_sibling(Node* node, Node* sibling);

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
