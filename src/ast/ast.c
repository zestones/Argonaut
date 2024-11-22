#include "../../lib/colors.h"

#include "../lexer/lexeme_table.h"
#include "../utils/utils.h"

#include "ast.h"

AST construct_ast() {
    return NULL;
}

static void print_ast_helper(Node* node, int depth, int is_sibling) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf(COLOR_CYAN "│   " COLOR_RESET);

    if (is_sibling) printf(COLOR_YELLOW "└── " COLOR_RESET);
    else printf(COLOR_YELLOW "├── " COLOR_RESET);

    printf(COLOR_BOLD "%s" COLOR_RESET, NodeTypeStrings[node->type]);

    printf(", " COLOR_GREEN "Lexico Idx: %d" COLOR_RESET, node->index_lexicographic);
    printf(", " COLOR_BLUE "Decl Idx: %d" COLOR_RESET, node->index_declaration);
    if (node->index_lexicographic != NULL_VALUE) {
        printf(" -- " COLOR_MAGENTA "Lexeme: '%s'" COLOR_RESET, get_lexeme(node->index_lexicographic));
    }

    printf("\n");

    // Recursively print the child (if exists) with indentation
    if (node->child) print_ast_helper(node->child, depth + 1, 0);
    if (node->sibling) print_ast_helper(node->sibling, depth, 1); 
}

void print_ast(AST ast) {
    printf(COLOR_BOLD "Root of the AST:\n" COLOR_RESET);
    print_ast_helper(ast, 0, 0);
}

static void free_ast_helper(Node* node) {
    if (!node) return;
    
    free_ast_helper(node->child);
    free_ast_helper(node->sibling);
    
    free(node);
}

void free_ast(AST ast) {
    free_ast_helper(ast);
}
