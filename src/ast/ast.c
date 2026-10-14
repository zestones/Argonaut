#include "../../lib/colors.h"

#include "../symbol_table/lexeme/lexeme_table.h"
#include "../utils/text_buffer.h"
#include "../utils/utils.h"

#include "ast.h"

AST construct_ast(void) {
    return NULL;
}

static void append_formatted_ast(TextBuffer *buffer, Node *node) {
    char node_data[96];
    int node_length = snprintf(node_data, sizeof(node_data), "Node(%d, %d, %d)", node->type, node->index_lexicographic,
                               node->index_declaration);
    if (node_length < 0 || (size_t) node_length >= sizeof(node_data)) {
        fprintf(stderr, COLOR_RED "<Error> Failed to format AST node\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }
    text_buffer_append_bytes(buffer, node_data, (size_t) node_length);

    if (node->child != NULL) {
        text_buffer_append(buffer, " [Child: ");
        append_formatted_ast(buffer, node->child);
        text_buffer_append_character(buffer, ']');
    }
    if (node->sibling != NULL) {
        text_buffer_append(buffer, " [Sibling: ");
        append_formatted_ast(buffer, node->sibling);
        text_buffer_append_character(buffer, ']');
    }
}

char *format_ast(Node *node) {
    if (node == NULL) return NULL;

    TextBuffer buffer = construct_text_buffer();
    append_formatted_ast(&buffer, node);
    return release_text_buffer(&buffer);
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
