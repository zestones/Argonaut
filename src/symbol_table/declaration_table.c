#include "../../lib/table_printer.h"
#include "../../lib/colors.h" 

#include "declaration_table.h"
#include "../utils/utils.h"

static Declaration declaration_table[MAX_DECLARATION_COUNT];
static int declaration_table_size = 0;

Declaration construct_declaration(Nature nature, int next, int region, int description, int execution) {
    Declaration new_declaration;

    new_declaration.nature = nature;
    new_declaration.next = next;
    new_declaration.region = region;
    new_declaration.description = description;
    new_declaration.execution = execution;

    return new_declaration;
}

void init_declaration_table() {
    memset(declaration_table, NULL_VALUE, sizeof(declaration_table));
}

void insert_declaration(int index, Nature nature, int region, int description, int execution) {
    if (declaration_table_size >= MAX_DECLARATION_COUNT) {
        fprintf(stderr, COLOR_RED "<Error> Declaration table is full\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    // TODO: ZONE DE DEBORDEMENT NON GÉRÉE
    declaration_table[index] = construct_declaration(nature, NULL_VALUE, region, description, execution);
    declaration_table_size++;
}

void insert_declaration_var(int lexeme_lexicographic_index, int type_lexicographic_index) {
    insert_declaration(lexeme_lexicographic_index, TYPE_VAR, NULL_VALUE, type_lexicographic_index, NULL_VALUE);  
}

void insert_declaration_struct(int lexeme_lexicographic_index, int type_lexicographic_index) {
    insert_declaration(lexeme_lexicographic_index, TYPE_STRUCT, NULL_VALUE, type_lexicographic_index, NULL_VALUE);
}

void insert_declaration_array(int lexeme_lexicographic_index, int description) {
    insert_declaration(lexeme_lexicographic_index, TYPE_ARRAY, NULL_VALUE, description, NULL_VALUE);
}

void insert_declaration_fonc_or_proc(int next, int region, int description, int execution) {
}

void print_declaration_table() {
    const int col_width_index = 10;
    const int col_width_nature = 15;
    const int col_width_next = 10;
    const int col_width_region = 15;
    const int col_width_description = 15;
    const int col_width_execution = 15;

    print_table_title("Declaration Table");
    print_table_separator(6, col_width_index, col_width_nature, col_width_next, col_width_region, col_width_description, col_width_execution);
    print_table_header(6, col_width_index, "Index", col_width_nature, "Nature", col_width_next, "Suivant", col_width_region, "Région", col_width_description, "Description", col_width_execution, "Exécution");
    print_table_separator(6, col_width_index, col_width_nature, col_width_next, col_width_region, col_width_description, col_width_execution);

    for (int i = 0; i < MAX_DECLARATION_COUNT; i++) {
        if (declaration_table[i].nature == NULL_VALUE) continue;
        
        int char_length = 20;
        char region_str[char_length], next_str[char_length], index_str[char_length];
        char description_str[char_length], execution_str[char_length], nature_str[char_length];

        sprintf(region_str, "%d", declaration_table[i].region);
        sprintf(next_str, "%d", declaration_table[i].next);
        sprintf(description_str, "%d", declaration_table[i].description);
        sprintf(execution_str, "%d", declaration_table[i].execution);
        sprintf(index_str, "%d", i);
        sprintf(nature_str, "%s", nature_to_string(declaration_table[i].nature));

        print_table_row(6, 
                        col_width_index, index_str,
                        col_width_nature, nature_str,
                        col_width_next, next_str,
                        col_width_region, region_str,
                        col_width_description, description_str,
                        col_width_execution, execution_str
                    );
    }

    print_table_separator(6, col_width_index, col_width_nature, col_width_next, col_width_region, col_width_description, col_width_execution);
}
