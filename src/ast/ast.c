#include "../../lib/colors.h"

#include "../symbol_table/lexeme/lexeme_table.h"
#include "../utils/utils.h"

#include "ast.h"

#include <stdint.h>

AST construct_ast(void) {
    return NULL;
}

static size_t checked_add(size_t left, size_t right) {
    if (right > SIZE_MAX - left) {
        fprintf(stderr, COLOR_RED "<Error> Formatted AST is too large\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }
    return left + right;
}

char *format_ast(Node *node) {
    if (!node) return NULL;

    char node_data[96];
    int node_length = snprintf(node_data, sizeof(node_data), "Node(%d, %d, %d)", node->type, node->index_lexicographic,
                               node->index_declaration);
    if (node_length < 0 || (size_t) node_length >= sizeof(node_data)) {
        fprintf(stderr, COLOR_RED "<Error> Failed to format AST node\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    char *child_data = format_ast(node->child);
    char *sibling_data = format_ast(node->sibling);
    size_t room = (size_t) node_length;
    if (child_data != NULL) {
        room = checked_add(room, sizeof(" [Child: ") - 1);
        room = checked_add(room, strlen(child_data));
        room = checked_add(room, 1);
    }
    if (sibling_data != NULL) {
        room = checked_add(room, sizeof(" [Sibling: ") - 1);
        room = checked_add(room, strlen(sibling_data));
        room = checked_add(room, 1);
    }
    room = checked_add(room, 1);

    char *formatted_row = malloc(room);
    if (formatted_row == NULL) {
        free(child_data);
        free(sibling_data);
        fprintf(stderr, COLOR_RED "<Error> Memory allocation failed\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    snprintf(formatted_row, room, "%s%s%s%s%s%s%s", node_data,
             child_data == NULL ? "" : " [Child: ", child_data == NULL ? "" : child_data, child_data == NULL ? "" : "]",
             sibling_data == NULL ? "" : " [Sibling: ", sibling_data == NULL ? "" : sibling_data,
             sibling_data == NULL ? "" : "]");
    free(child_data);
    free(sibling_data);

    return formatted_row;
}

static void print_ast_helper(FILE *out, Node *node, int depth, int is_sibling) {
    if (!node) return;

    int use_colors = (out == stdout);
    for (int i = 0; i < depth; i++) {
        if (use_colors) fprintf(out, COLOR_CYAN "│   " COLOR_RESET);
        else fprintf(out, "│   ");
    }

    if (is_sibling) {
        if (use_colors) fprintf(out, COLOR_YELLOW "└── " COLOR_RESET);
        else fprintf(out, "└── ");
    } else {
        if (use_colors) fprintf(out, COLOR_YELLOW "├── " COLOR_RESET);
        else fprintf(out, "├── ");
    }

    if (use_colors) fprintf(out, COLOR_BOLD "%s" COLOR_RESET, NodeTypeStrings[node->type]);
    else fprintf(out, "%s", NodeTypeStrings[node->type]);

    if (use_colors) {
        fprintf(out, ", " COLOR_GREEN "Lexico Idx: %d" COLOR_RESET, node->index_lexicographic);
        fprintf(out, ", " COLOR_BLUE "Decl Idx: %d" COLOR_RESET, node->index_declaration);
    } else {
        fprintf(out, ", Lexico Idx: %d", node->index_lexicographic);
        fprintf(out, ", Decl Idx: %d", node->index_declaration);
    }

    if (node->index_lexicographic != NULL_VALUE) {
        if (use_colors)
            fprintf(out, " -- " COLOR_MAGENTA "Lexeme: '%s'" COLOR_RESET, get_lexeme(node->index_lexicographic));
        else fprintf(out, " -- Lexeme: '%s'", get_lexeme(node->index_lexicographic));
    }

    fprintf(out, "\n");

    // Recursively print the child (if exists) with indentation
    if (node->child) print_ast_helper(out, node->child, depth + 1, 0);
    if (node->sibling) print_ast_helper(out, node->sibling, depth, 1);
}

void fprintf_ast(FILE *out, AST ast) {
    fprintf(out, "Root of the AST:\n");
    print_ast_helper(out, ast, 0, 0);
}

static void free_ast_helper(Node *node) {
    if (!node) return;

    free_ast_helper(node->child);
    free_ast_helper(node->sibling);

    free(node);
}

void free_ast(AST ast) {
    free_ast_helper(ast);
}
