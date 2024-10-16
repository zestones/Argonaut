#include "../bin/y.tab.h"
#include "parser.h"


void yywarn(const char *s) {
    fprintf(stderr, "\033[1;33m<Warning> At line %d, column %d: %s near token '%s'\033[0m\n", error_line, error_column, s, yytext);
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
        fprintf(stderr, "\033[1;31m<%s> At line %d, column %d: %s near token '%s'\033[0m\n", type, error_line, error_column, s, yytext);
    } 
}


int run_parser() {
    // initialize_parser();
    int parse_result = yyparse(); 
    // cleanup_parser();

    return parse_result;
}
