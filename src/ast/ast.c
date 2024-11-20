#include "../../lib/colors.h"
#include "../utils/utils.h"
#include "ast.h"


AST construct_ast() {
    return NULL;
}

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

static const char* node_type_to_string(NodeType type) {
    switch (type) {
        case A_PROGRAM: return "Program";
        case A_CONDITION: return "Condition";
        case A_AND_CONDITION: return "And Condition";
        case A_OR_CONDITION: return "Or Condition";
        case A_NOT_CONDITION: return "Not Condition";
        case A_NOT_EXPRESSION: return "Not Expression";
        case A_ADDITION: return "Addition";
        case A_SUBTRACTION: return "Subtraction";
        case A_MULTIPLICATION: return "Multiplication";
        case A_DIVISION: return "Division";
        case A_EQUAL_OP: return "Equal Operator";
        case A_NOT_EQUAL_OP: return "Not Equal Operator";
        case A_LESS_THAN_OP: return "Less Than Operator";
        case A_GREATER_THAN_OP: return "Greater Than Operator";
        case A_LESS_EQUAL_OP: return "Less Equal Operator";
        case A_GREATER_EQUAL_OP: return "Greater Equal Operator";
        case A_ADD_OP: return "Add Operator";
        case A_SUB_OP: return "Subtract Operator";
        case A_MUL_OP: return "Multiply Operator";
        case A_DIV_OP: return "Divide Operator";
        case A_DECLARATION_LIST: return "Declaration List";
        case A_VARIABLE_DECLARATION: return "Variable Declaration";
        case A_FUNCTION_DECLARATION: return "Function Declaration";
        case A_PROCEDURE_DECLARATION: return "Procedure Declaration";
        case A_TYPE_DECLARATION: return "Type Declaration";
        case A_INTEGER_LITERAL: return "Integer Literal";
        case A_FLOAT_LITERAL: return "Float Literal";
        case A_BOOLEAN_LITERAL: return "Boolean Literal";
        case A_CHARACTER_LITERAL: return "Character Literal";
        case A_STRING_LITERAL: return "String Literal";
        case A_IDENTIFIER: return "Identifier";
        case A_STATEMENT_LIST: return "Statement List";
        case A_ASSIGNMENT_STATEMENT: return "Assignment Statement";
        case A_FUNCTION_CALL_STATEMENT: return "Function Call Statement";
        case A_LOOP_STATEMENT: return "Loop Statement";
        case A_VARIABLE_ASSIGNMENT: return "Variable Assignment";
        case A_ARRAY_ASSIGNMENT: return "Array Assignment";
        case A_STRUCT_ASSIGNMENT: return "Struct Assignment";
        case A_IF: return "If";
        case A_IF_ELSE: return "If Else";
        case A_WHILE: return "While";
        case A_ARRAY_ACCESS: return "Array Access";
        case A_ARRAY_INDEX_LIST: return "Array Index List";
        case A_ARRAY_INDEX: return "Array Index";
        case A_ARRAY_FIELD_ACCESS: return "Array Field Access";
        case A_STRUCT_ARRAY_ACCESS: return "Struct Array Access";
        case A_STRUCT_FIELD_ACCESS: return "Struct Field Access";
        case A_PRINT_STATEMENT: return "Print Statement";
        case A_INPUT_STATEMENT: return "Input Statement";
        case A_ASSIGNABLE_ENTITY: return "Assignable Entity";
        case A_ARRAY_ASSIGNABLE_ENTITY: return "Array Assignable Entity";
        case A_STRUCT_ASSIGNABLE_ENTITY: return "Struct Assignable Entity";
        case A_ASSIGNABLE_ENTITY_LIST: return "Assignable Entity List";
        case A_ARGUMENT_LIST: return "Argument List";
        case A_PARAMETER_LIST: return "Parameter List";
        case A_PARAMETER: return "Parameter";
        default: return "Unknown";
    }
}

static void print_ast_helper(Node* node, int depth, int is_sibling) {
    if (!node) return;

    // Print indentation with appropriate color
    for (int i = 0; i < depth; i++) printf(COLOR_CYAN "│   " COLOR_RESET);

    // Mark the current node and print its type and index info
    if (is_sibling) {
        printf(COLOR_YELLOW "└── " COLOR_RESET);  // Sibling node
    } else {
        printf(COLOR_YELLOW "├── " COLOR_RESET);  // Non-sibling node
    }

    // Print the type of the node in bold and colored for clarity
    printf(COLOR_BOLD "%s" COLOR_RESET, node_type_to_string(node->type));

    // Print the Lexico and Decl Index in different colors for easy distinction
    printf(", " COLOR_GREEN "Lexico Idx: %d" COLOR_RESET, node->index_lexicographic);
    printf(", " COLOR_BLUE "Decl Idx: %d" COLOR_RESET, node->index_declaration);

    // Move to the next line after printing the current node information
    printf("\n");

    // Recursively print the child (if exists) with indentation
    if (node->child) {
        print_ast_helper(node->child, depth + 1, 0);  // First child, marked with ├──
    }

    // Recursively print the sibling (if exists) without increasing indentation
    if (node->sibling) {
        print_ast_helper(node->sibling, depth, 1);  // Sibling node, marked with └──
    }
}

void print_ast(AST ast) {
    printf(COLOR_BOLD "Root of the AST:\n" COLOR_RESET);
    print_ast_helper(ast, 0, 0);
}


void free_ast_helper(Node* node) {
    if (!node) return;
    free_ast_helper(node->child);
    free_ast_helper(node->sibling);
    free(node);
}

void free_ast(AST ast) {
    free_ast_helper(ast);
}
