#include "parser.h"
#include "../bin/y.tab.h"


void ydebug(int debug) {
    if (!debug) return;

    print_lexeme_table();
    print_hash_table();
    
    print_declaration_table();
    print_representation_table();

    print_region_table();
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
