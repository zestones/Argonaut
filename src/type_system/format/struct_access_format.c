#include "../../symbol_table/representation/representation_table.h" 
#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../lexer/lexeme_table.h"
#include "../../utils/utils.h"
#include "formatting.h"

char *format_struct_access(Node *struct_access) {
    char *lexeme = (char *)malloc(256 * sizeof(char));
    strcpy(lexeme, get_lexeme(struct_access->index_lexicographic));

    Node *current_node = struct_access->child;
    while (current_node != NULL) {
        if (current_node->type == A_STRUCT_FIELD_ACCESS) {
            // Concatenate the struct field access lexeme
            size_t new_size = strlen(lexeme) + 2 + strlen(get_lexeme(current_node->index_lexicographic)) + 1;
            lexeme = (char *)realloc(lexeme, new_size);
 
            strcat(lexeme, ".");
            strcat(lexeme, get_lexeme(current_node->index_lexicographic));
        }
        else if (current_node->type == A_ARRAY_ACCESS) {
            char *array_access = format_array_access(current_node);
            size_t new_size = strlen(lexeme) + strlen(array_access) + 2;
            lexeme = (char *)realloc(lexeme, new_size);

            strcat(lexeme, ".");
            strcat(lexeme, array_access);
            free(array_access);
        }
        
        current_node = current_node->child;
    }

    return lexeme;
}