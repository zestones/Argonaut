#include "../bin/y.tab.h"
#include "parser.h"


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


int run_parser() {
    // initialize_parser();
    int parse_result = yyparse(); 
    // cleanup_parser();

    return parse_result;
}
