#include "../../symbol_table/representation/representation_table.h" 
#include "../../symbol_table/declaration/declaration_table.h" 
#include "../../symbol_table/lexeme/lexeme_table.h"
#include "../../utils/utils.h"
#include "formatting.h"

/**
 * @brief Formats a list of array indices into a string representation.
 * 
 * This function takes a linked list of array indices and formats it into a string, 
 * which includes each index enclosed in square brackets. Each index is separated 
 * by a comma and a space. The string is dynamically allocated and must be freed by the caller.
 * 
 * @param array_index_list A pointer to the root node of the array index list.
 * @return A dynamically allocated string representing the formatted array index list.
 * 
 * @note The caller is responsible for freeing the returned string.
 * 
 * @warning If memory allocation fails during concatenation, the function may result in undefined behavior.
 */
static char *format_array_index_list(Node *array_index_list) {
    char *lexeme = (char *)malloc(256 * sizeof(char));
    lexeme[0] = '\0';

    Node *current_node = array_index_list->child;
    while (current_node != NULL) {
        if (current_node->type == A_ARRAY_INDEX) {
            // Allocate or reallocate memory for each new lexeme part
            char *index_lexeme = format_expression(current_node->child);
            size_t new_size = strlen(lexeme) + strlen(index_lexeme) + 5; // +5 for brackets, comma, space and index
            lexeme = (char *)realloc(lexeme, new_size);

            if (strlen(lexeme) == 0) {
                strcat(lexeme, "[");
            }

            strcat(lexeme, index_lexeme);
            
            if (current_node->sibling != NULL) strcat(lexeme, ", ");
            else strcat(lexeme, "]");
        }
        current_node = current_node->sibling;
    }

    return lexeme;
}

char *format_array_access(Node *array_access) {
    char *lexeme = (char *)malloc(256 * sizeof(char));
    strcpy(lexeme, get_lexeme(array_access->index_lexicographic));

    Node *current_node = array_access->child;
    while (current_node != NULL) {
        if (current_node->type == A_ARRAY_INDEX_LIST) {
            // Concatenate the resolved array index list lexeme
            char *index_list_lexeme = format_array_index_list(current_node);
            size_t new_size = strlen(lexeme) + strlen(index_list_lexeme) + 1;
            lexeme = (char *)realloc(lexeme, new_size);

            strcat(lexeme, index_list_lexeme);
            free(index_list_lexeme);
        }
        else if (current_node->type == A_STRUCT_FIELD_ACCESS) {
            char *field_lexeme = format_struct_access(current_node);
            size_t new_size = strlen(lexeme) + strlen(field_lexeme) + 2;
            lexeme = (char *)realloc(lexeme, new_size);

            strcat(lexeme, ".");
            strcat(lexeme, field_lexeme);
            free(field_lexeme);
        }

        current_node = current_node->sibling;
    }

    return lexeme;
}