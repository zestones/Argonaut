#include "../../lib/table_printer.h"
#include "../../lib/colors.h"
#include "../utils/utils.h"
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
    memset(lexeme_table, NULL_VALUE, sizeof(lexeme_table));
}

int insert_lexeme(const char* lexeme) {
    int length = strlen(lexeme);
    int hash_code = get_hash_value(hash_function(lexeme, length));
    int prev_index = NULL_VALUE;

    while (hash_code != NULL_VALUE) {
        if (!strcmp(lexeme_table[hash_code].lexeme, lexeme)) break;

        prev_index = hash_code;
        hash_code = lexeme_table[hash_code].next;
    }


    if (hash_code != NULL_VALUE) return hash_code;
    else hash_code = lexeme_table_size;

    Lexeme new_lexeme = construct_lexeme(lexeme, length, NULL_VALUE);
    insert_hash(lexeme, hash_code);

    if (prev_index != NULL_VALUE) lexeme_table[prev_index].next = hash_code;
    lexeme_table[lexeme_table_size++] = new_lexeme;

    return hash_code;
}


void print_lexeme_table() {
    const int cols_width[] = {10, 20, 10, 10};

    print_table_title("Lexeme Table");
    print_table_separator(4, cols_width[0], cols_width[1], cols_width[2], cols_width[3]);
    print_table_header(4, cols_width[0], "Index", cols_width[1], "Lexeme", cols_width[2], "Length", cols_width[3], "Next");
    print_table_separator(4, cols_width[0], cols_width[1], cols_width[2], cols_width[3]);

    for (int i = 0; i < lexeme_table_size; i++) {
        char length_str[10], next_str[10], index_str[10];
        sprintf(length_str, "%d", lexeme_table[i].length);
        sprintf(next_str, "%d", lexeme_table[i].next);
        sprintf(index_str, "%d", i);

        print_table_row(4, 
                        cols_width[0], index_str,
                        cols_width[1], lexeme_table[i].lexeme,
                        cols_width[2], length_str,
                        cols_width[3], next_str
                    );
    }

    print_table_separator(4, cols_width[0], cols_width[1], cols_width[2], cols_width[3]);
}