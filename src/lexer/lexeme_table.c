#include "../../lib/colors.h"
#include "lexeme_table.h"


static Lexeme *lexeme_table[HASH_TABLE_SIZE];
static int lexicographic_code_counter = 1;

void init_lexeme_table() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        lexeme_table[i] = NULL;
    }
}

int insert_lexeme(const char* lexeme) {
    int length = strlen(lexeme);
    unsigned int hash_index = hash_function(lexeme, length);

    // Check if the lexeme already exists
    Lexeme *current = lexeme_table[hash_index];
    while (current != NULL) {
        if (strcmp(current->lexeme, lexeme) == 0) {
            return current->lexicographic_code;
        }

        current = current->next;
    }

    // Lexeme doesn't exist, insert new one
    Lexeme *new_lexeme = (Lexeme*)malloc(sizeof(Lexeme));
    if (new_lexeme == NULL) {
        fprintf(stderr, "Memory allocation failed for new lexeme\n");
        exit(EXIT_FAILURE);
    }

    strcpy(new_lexeme->lexeme, lexeme);
    new_lexeme->length = length;
    new_lexeme->lexicographic_code = lexicographic_code_counter++;
    new_lexeme->next = lexeme_table[hash_index];
    lexeme_table[hash_index] = new_lexeme;

    return new_lexeme->lexicographic_code;
}

Lexeme *find_lexeme(const char* lexeme) {
    int length = strlen(lexeme);
    unsigned int hash_index = hash_function(lexeme, length);

    Lexeme* current = lexeme_table[hash_index];
    while (current != NULL) {
        if (strcmp(current->lexeme, lexeme) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;  // Lexeme not found
}

void print_lexeme_table() {
    printf(COLOR_BOLD COLOR_BLUE "Lexeme Table:\n" COLOR_RESET);
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Lexeme* current = lexeme_table[i];
        while (current != NULL) {
            printf(COLOR_GREEN "Lexeme: " COLOR_RESET COLOR_YELLOW "%s" COLOR_RESET ", " 
                   COLOR_GREEN "Length: " COLOR_RESET COLOR_YELLOW "%d" COLOR_RESET ", " 
                   COLOR_GREEN "Lexicographic Code: " COLOR_RESET COLOR_YELLOW "%d" COLOR_RESET "\n", 
                   current->lexeme, current->length, current->lexicographic_code);
            current = current->next;
        }
    }
}

void free_lexeme_table() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Lexeme* current = lexeme_table[i];
        while (current != NULL) {
            Lexeme* to_free = current;
            current = current->next;
            free(to_free);
        }
        lexeme_table[i] = NULL;
    }
}
