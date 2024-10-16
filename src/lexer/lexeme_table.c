#include "../../lib/table_printer.h"
#include "../../lib/colors.h"
#include "../utils/hash.h"
#include "lexeme_table.h"

static Lexeme lexeme_table[MAX_LEXEME_COUNT];
static int lexeme_table_size = 0;

Lexeme construct_lexeme(const char* lexeme, int length, int next) {
    Lexeme new_lexeme;

    strcpy(new_lexeme.lexeme, lexeme);
    new_lexeme.length = length;
    new_lexeme.next = next;

    return new_lexeme;
}

void init_lexeme_table() {
    memset(lexeme_table, -1, sizeof(lexeme_table));
    
    insert_lexeme("int");
    insert_lexeme("float");
    insert_lexeme("bool");
    insert_lexeme("char");
}

int insert_lexeme(const char* lexeme) {
    int length = strlen(lexeme);
    int hash_index = find_hash(hash_function(lexeme, length));
    int prev_index = -1;

    while (hash_index != -1) {
        if (!strcmp(lexeme_table[hash_index].lexeme, lexeme)) break;

        prev_index = hash_index;
        hash_index = lexeme_table[hash_index].next;
    }

    if (hash_index != -1) return hash_index;
    else hash_index = lexeme_table_size;

    Lexeme new_lexeme = construct_lexeme(lexeme, length, -1);
    insert_hash(lexeme, hash_index);

    if (prev_index != -1) lexeme_table[prev_index].next = hash_index;
    lexeme_table[lexeme_table_size++] = new_lexeme;

    return hash_index;
}


void print_lexeme_table() {
    const int col_width_index = 10;
    const int col_width_lexeme = 20;
    const int col_width_length = 10;
    const int col_width_next = 10;

    fprintf(stdout, COLOR_BOLD "\nLexeme Table:\n" COLOR_RESET);
    print_table_separator(4, col_width_index, col_width_lexeme, col_width_length, col_width_next);
    print_table_header(4, col_width_index, "Index", col_width_lexeme, "Lexeme", col_width_length, "Length", col_width_next, "Next");
    print_table_separator(4, col_width_index, col_width_lexeme, col_width_length, col_width_next);

    for (int i = 0; i < lexeme_table_size; i++) {
        char length_str[10], next_str[10], index_str[10];
        sprintf(length_str, "%d", lexeme_table[i].length);
        sprintf(next_str, "%d", lexeme_table[i].next);
        sprintf(index_str, "%d", i);

        print_table_row(4, 
                        col_width_index, index_str,
                        col_width_lexeme, lexeme_table[i].lexeme,
                        col_width_length, length_str,
                        col_width_next, next_str
                    );
    }

    print_table_separator(4, col_width_index, col_width_lexeme, col_width_length, col_width_next);
}