#include "../../../lib/table_printer.h"
#include "../../../lib/colors.h"

#include "../../utils/utils.h"

#include "../hash/hash_table.h"
#include "../utility.h"

#include "lexeme_table.h"
#include "../../lexer/escapes.h"

static Lexeme lexeme_table[MAX_LEXEME_COUNT];
static int lexeme_table_size = 0;

Lexeme construct_lexeme(const char *lexeme, int length, int next) {
    Lexeme new_lexeme;

    new_lexeme.lexeme = strdup(lexeme);
    if (new_lexeme.lexeme == NULL) {
        fprintf(stderr, COLOR_RED "<Error> Memory allocation failed\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }
    new_lexeme.length = length;
    new_lexeme.next = next;

    return new_lexeme;
}

void init_lexeme_table() {
    memset(lexeme_table, NULL_VALUE, sizeof(lexeme_table));
}

static int get_lexeme_index(const char *lexeme) {
    for (int i = 0; i < lexeme_table_size; i++) {
        if (strcmp(lexeme_table[i].lexeme, lexeme) == 0) {
            return i;
        }
    }

    return NULL_VALUE;
}

int insert_lexeme(const char *lexeme) {
    int length = strlen(lexeme);
    int index = get_hash_value(hash_function(lexeme, length));

    // We first check if the lexeme is already in the table
    int index_duplicate_lexeme = get_lexeme_index(lexeme);
    if (index_duplicate_lexeme != NULL_VALUE) return index_duplicate_lexeme;

    if (lexeme_table_size >= MAX_LEXEME_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Too many distinct lexemes: the table holds at most %d.\n" COLOR_RESET,
                MAX_LEXEME_COUNT);
        exit(EXIT_FAILURE);
    }

    // If the lexeme is not in the table, we insert it
    Lexeme new_lexeme = construct_lexeme(lexeme, length, NULL_VALUE);
    insert_hash(lexeme, lexeme_table_size);

    // If there is a collision, we link the new lexeme to the previous one
    if (index != NULL_VALUE) lexeme_table[index].next = lexeme_table_size;
    lexeme_table[lexeme_table_size] = new_lexeme;

    return lexeme_table_size++;
}

void insert_lexeme_row(int index, const char *lexeme, int length, int next) {
    if (index < 0 || index >= MAX_LEXEME_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Lexeme table row %d out of bounds (0 to %d).\n" COLOR_RESET, index,
                MAX_LEXEME_COUNT - 1);
        exit(EXIT_FAILURE);
    }

    Lexeme new_lexeme = construct_lexeme(lexeme, length, next);
    lexeme_table[index] = new_lexeme;

    lexeme_table_size++;
}

int is_lexeme_loaded(int index) {
    return index >= 0 && index < MAX_LEXEME_COUNT && lexeme_table[index].length >= 0;
}

char *get_lexeme(int index) {
    static char unknown[] = "<unknown>";
    if (!is_lexeme_loaded(index)) return unknown;
    return lexeme_table[index].lexeme;
}

static char *format_lexeme_row(void *data) {
    Lexeme *row = (Lexeme *) data;
    // A row must stay on one line: the decoded text is escaped (\n, \\ ...) and argov decodes it back.
    char *escaped = escape_text(row->lexeme);
    size_t room = strlen(escaped) + 32;
    char *formatted_row = (char *) malloc(room);
    if (!formatted_row) {
        fprintf(stderr, COLOR_RED "<Error> Memory allocation failed\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    snprintf(formatted_row, room, "%s|%d|%d", escaped, row->length, row->next);
    free(escaped);
    return formatted_row;
}

void export_lexeme_table(const char *filename) {
    export_table(filename, lexeme_table, lexeme_table_size, sizeof(Lexeme), format_lexeme_row, "BEGIN_LEXEME_TABLE",
                 "END_LEXEME_TABLE");
}

void fprintf_lexeme_table(FILE *out) {
    const int cols_width[] = {10, 20, 10, 10};
    char title[50];
    sprintf(title, "Lexeme Table: %d entries", lexeme_table_size);

    print_table_title(out, title);
    print_table_separator(out, 4, cols_width[0], cols_width[1], cols_width[2], cols_width[3]);
    print_table_header(out, 4, cols_width[0], "Index", cols_width[1], "Lexeme", cols_width[2], "Length", cols_width[3],
                       "Next");
    print_table_separator(out, 4, cols_width[0], cols_width[1], cols_width[2], cols_width[3]);

    for (int i = 0; i < lexeme_table_size; i++) {
        char length_str[10], next_str[10], index_str[10];
        sprintf(length_str, "%d", lexeme_table[i].length);
        sprintf(next_str, "%d", lexeme_table[i].next);
        sprintf(index_str, "%d", i);
        char *escaped = escape_text(lexeme_table[i].lexeme);

        print_table_row(out, 4, cols_width[0], index_str, cols_width[1], escaped, cols_width[2], length_str,
                        cols_width[3], next_str);
        free(escaped);

        // Print separator after the base types
        if (i == 3) {
            print_table_separator(out, 4, cols_width[0], cols_width[1], cols_width[2], cols_width[3]);
        }
    }

    print_table_separator(out, 4, cols_width[0], cols_width[1], cols_width[2], cols_width[3]);
}