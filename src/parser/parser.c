#include "parser.h"
#include "../bin/y.tab.h"

static void write_tables_to(FILE* out) {
    fprintf_lexeme_table(out);
    fprintf_hash_table(out);
    
    fprintf_declaration_table(out);
    fprintf_representation_table(out);

    fprintf_region_table(out);
}

void ylog(int log) {
    if (!log) return;
    const char *filename = "log.txt";
    FILE *output = fopen(filename, "w");
    if (!output) {
        fprintf(stderr, "Error opening log file %s\n", filename);
        return;
    }

    fprintf(stdout, COLOR_GREEN "Logging enabled. Exporting tables to %s\n" COLOR_RESET, filename);
    write_tables_to(output);
    fclose(output);
}

void ydebug(int debug) {
    if (!debug) return;
    fprintf(stdout, COLOR_GREEN "Verbose mode enabled. Printing tables and ast...\n" COLOR_RESET);
    write_tables_to(stdout);
}

static void initialize_tables() {
    int lexicographic_index = 0;

    init_hash_table();
    init_lexeme_table();
    
    init_declaration_table();
    init_representation_table();

    init_region_table();

    lexicographic_index = insert_lexeme("int");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    lexicographic_index = insert_lexeme("float");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    lexicographic_index = insert_lexeme("bool");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    lexicographic_index = insert_lexeme("char");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    start_region();
}

static void initialize_parser() {
    initialize_tables();
}


int yyrun() {
    initialize_parser();
    int parse_result = yyparse(); 

    return parse_result;
}
