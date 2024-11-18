#include "../../lib/table_printer.h"
#include "hash_table.h"

static int hash_table[HASH_TABLE_SIZE];

static void new_hash_entry(int index, int value);


void insert_hash(const char* lexeme, int next) {
    int hash_index = hash_function(lexeme, strlen(lexeme));
    new_hash_entry(hash_index, next);
}

void init_hash_table() {
    memset(hash_table, -1, sizeof(hash_table));
}

int hash_function(const char* lexeme, int length) {
    int hash = 0;
    for (int i = 0; i < length; i++) {
        hash += lexeme[i];
    }

    return hash % HASH_TABLE_SIZE;
}

static void new_hash_entry(int index, int value) {
    if (index < 0 || index >= HASH_TABLE_SIZE) {
        fprintf(stderr, COLOR_RED "<Error>: Hash Table Index %d out of bounds. Valid range is 0 to %d.\n" COLOR_RESET, index, HASH_TABLE_SIZE);
        exit(EXIT_FAILURE);
    }

    hash_table[index] = value;
}

int get_hash_value(int index) {
    if (index < 0 || index >= HASH_TABLE_SIZE) {
        fprintf(stderr, COLOR_RED "<Error>: Hash Table Index %d out of bounds. Valid range is 0 to %d.\n" COLOR_RESET, index, HASH_TABLE_SIZE);
        exit(EXIT_FAILURE);
    }

    return hash_table[index];
}

void print_hash_table() {
    const int cols_width[] = {10, 20};

    print_table_title("Hash Table");
    print_table_separator(2, cols_width[0], cols_width[1]);
    print_table_header(2, cols_width[0], "Hash", cols_width[1], "Lexicographic Index");
    print_table_separator(2, cols_width[0], cols_width[1]);

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hash_table[i] == -1) continue;

        char index_str[10], value_str[10];
        sprintf(index_str, "%d", i);
        sprintf(value_str, "%d", hash_table[i]);

        print_table_row(2,
                        cols_width[0], index_str, 
                        cols_width[1], value_str
                    );
    }

    print_table_separator(2, cols_width[0], cols_width[1]);
}