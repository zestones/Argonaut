#include "../bin/y.tab.h"
#include "parser.h"


void yydebug(int debug) {
    if (!debug) return;

    print_lexeme_table();
    print_hash_table();
    
    print_declaration_table();
    print_representation_table();
}

void yywarn(const char *s) {
    fprintf(stderr, COLOR_BOLD_YELLOW "<Warning> At line %d, column %d: %s near token '%s'\n" COLOR_RESET, error_line, error_column, s, yytext);
}

void yyerror(const char *s) {
    const char *type = "Syntax Error"; // Default type

    if (strstr(s, "unexpected input") != NULL) {
        type = "Syntax Error";
    } else if (strstr(s, "undeclared variable") != NULL) {
        type = "Semantic Error";
    } else if (strstr(s, "type mismatch") != NULL) {
        type = "Type Error";
    } else if (strstr(s, "invalid") != NULL) {
        type = "Invalid Input Error";
    }

    if (yychar) {
        fprintf(stderr, COLOR_RED "<%s> At line %d, column %d: %s near token '%s'\n" COLOR_RESET, type, error_line, error_column, s, yytext);
    } 
}

static void initialize_tables() {
    int lexicographic_index = 0;

    init_hash_table();
    init_lexeme_table();
    init_declaration_table();
    init_representation_table();

    lexicographic_index = insert_lexeme("int");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    lexicographic_index = insert_lexeme("float");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    lexicographic_index = insert_lexeme("bool");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);

    lexicographic_index = insert_lexeme("char");
    insert_declaration(lexicographic_index, TYPE_BASE, 0, lexicographic_index, 1);
}

static void initialize_parser() {
    initialize_tables();
}


int yyrun() {
    initialize_parser();
    int parse_result = yyparse(); 

    return parse_result;
}
