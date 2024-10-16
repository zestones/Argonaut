#include "../bin/y.tab.h"
#include "parser.h"


void yyerror(const char *s) {
    if (yychar == 0) {
        fprintf(stderr, "\033[1;31m<error> Syntax error at line %d: %s\033[0m\n", error_line, s);
    } 
    else {
        fprintf(stderr, "\033[1;31m<error> Syntax error at line %d: %s near token '%s'\033[0m\n", error_line, s, yytext);
    }
}

int run_parser() {
    // initialize_parser();
    int parse_result = yyparse(); 
    // cleanup_parser();

    return parse_result;
}
