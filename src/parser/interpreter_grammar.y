%{
    #include "../symbol_table/representation/representation_table.h"
    #include "../symbol_table/declaration/declaration_table.h"
    #include "../symbol_table/lexeme/lexeme_table.h"
    #include "../data/region_table.h"

    #include "../utils/errors.h"

    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>

    int yylex();
    extern FILE *yyin;
    extern FILE *yyout;
    extern char *yytext;

    Error error;

    void yyerror(const char *s) {
        if (!strcmp(s, "syntax error")) {
            set_error_message(&error,
                "Unexpected token found: '%s' at %s.\n"
                "\t> This error is critical and will cause the program to terminate.\n"
                "\t> Exiting due to a syntax error.",
                yytext,
                get_formatted_location()
            );
        } 
        else {
            set_error_message(&error,
                "Unexpected token found: '%s' at %s.\n"
                "\t> %s\n"
                "\t> This error is critical and will cause the program to terminate.\n"
                "\t> Exiting due to a syntax error.",
                yytext,
                get_formatted_location(),
                s
            );
        }

        set_error_type(&error, SYNTAX_ERROR);
        yerror(error);

        exit(EXIT_FAILURE);
    }
%}

%union {
    int integer;
    char* string;
    float real;
}


%token BEGIN_LEXEME_TABLE END_LEXEME_TABLE
%token CPYRR PIPE

%token <integer> INTEGER
%token <string> LEXEME
%token <real> FLOAT
%start program

%%

program: CPYRR lexeme_table
        | error { yyerrok; }
;

lexeme_table: BEGIN_LEXEME_TABLE lexeme_table_body END_LEXEME_TABLE
;

lexeme_table_body: lexeme_table_body lexeme_table_row
                | lexeme_table_row
;

lexeme_table_row: PIPE INTEGER PIPE LEXEME PIPE INTEGER PIPE INTEGER PIPE {
                    // add_lexeme($2, $4, $6, $8);
                    printf("lexeme: %d %s %d %d\n", $2, $4, $6, $8);
                }
                | PIPE INTEGER PIPE INTEGER PIPE INTEGER PIPE INTEGER PIPE {
                    // add_lexeme($2, NULL, $4, $6);
                    printf("lexeme: %d %d %d %d\n", $2, $4, $6, $8);
                }
                | PIPE INTEGER PIPE FLOAT PIPE INTEGER PIPE INTEGER PIPE {
                    // add_lexeme($2, NULL, $4, $6);
                    printf("lexeme: %d %f %d %d\n", $2, $4, $6, $8);
                }
;


%%

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    char *input_file = argv[1];

    error.line = 1;
    error.column = 1;

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    error.filename = input_file;

    extern FILE *yyin;
    yyin = file;

    yyparse();

    fclose(file);
    return 0;
}
