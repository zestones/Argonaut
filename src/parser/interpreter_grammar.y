%{
    #include "../symbol_table/representation/representation_table.h"
    #include "../symbol_table/declaration/declaration_table.h"
    #include "../symbol_table/lexeme/lexeme_table.h"
    #include "../data/region_table.h"

    #include "parser.h"
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
    AST ast;

    int integer;
    char* string;
    float real;
}


%token BEGIN_LEXEME_TABLE END_LEXEME_TABLE
%token BEGIN_DECLARATION_TABLE END_DECLARATION_TABLE
%token BEGIN_REPRESENTATION_TABLE END_REPRESENTATION_TABLE
%token BEGIN_REGION_TABLE END_REGION_TABLE
%token NODE_LPAREN NODE_RPAREN COMMA LBRACKET RBRACKET CHILD SIBLING

%token PIPE

%token <integer> INTEGER
%token <string> LEXEME
%token <real> FLOAT
%start program

%type <ast> ast_node child_node sibling_node node

%%

program: lexeme_table declaration_table representation_table region_table
;



lexeme_table: BEGIN_LEXEME_TABLE lexeme_table_body END_LEXEME_TABLE
;

lexeme_table_body: lexeme_table_row
                 | lexeme_table_body lexeme_table_row
;

lexeme_table_row: PIPE INTEGER PIPE LEXEME PIPE INTEGER PIPE INTEGER PIPE {
                    insert_lexeme_row($2, $4, $6, $8);
                }
                | PIPE INTEGER PIPE INTEGER PIPE INTEGER PIPE INTEGER PIPE {
                    int len = snprintf(NULL, 0, "%d", $4);  
                    char* lexeme = malloc(len + 1);
                    sprintf(lexeme, "%d", $4);

                    insert_lexeme_row($2, lexeme, $6, $8);
                    free(lexeme);
                }
                | PIPE INTEGER PIPE FLOAT PIPE INTEGER PIPE INTEGER PIPE {
                   int len = snprintf(NULL, 0, "%f", $4);  
                    char* lexeme = malloc(len + 1);
                    sprintf(lexeme, "%f", $4);
                    insert_lexeme_row($2, lexeme, $6, $8);
                }
;



declaration_table: BEGIN_DECLARATION_TABLE declaration_table_body END_DECLARATION_TABLE
;

declaration_table_body: declaration_table_row 
                      | declaration_table_body declaration_table_row
;

declaration_table_row: PIPE INTEGER PIPE INTEGER PIPE INTEGER PIPE INTEGER PIPE INTEGER PIPE INTEGER PIPE {
                        insert_declaration_row($2, $4, $6, $8, $10, $12);
                    }
;



representation_table: BEGIN_REPRESENTATION_TABLE representation_table_body END_REPRESENTATION_TABLE
;

representation_table_body:
                         | representation_table_body representation_table_row
;

representation_table_row: PIPE INTEGER PIPE INTEGER PIPE {
                            insert_representation_row($2, $4);
                        }
;



region_table: BEGIN_REGION_TABLE region_table_body END_REGION_TABLE
;

region_table_body: region_table_row
                | region_table_body region_table_row
;


region_table_row: PIPE INTEGER PIPE INTEGER PIPE INTEGER PIPE ast_node PIPE
    { 
        fprintf_ast(stdout, $8); 
    }
;

ast_node: node child_node sibling_node
    { 
        append_child($1, $2);
        add_sibling($1, $3);
    }
    | node child_node
    { 
        append_child($1, $2);
    }
    | node sibling_node
    { 
        add_sibling($1, $2);  
    }
    | node { $$ = $1; }
;

node: NODE_LPAREN INTEGER COMMA INTEGER COMMA INTEGER NODE_RPAREN
    { 
        $$ = construct_node($2, $4, $6);
    }
;

child_node: LBRACKET CHILD ast_node RBRACKET { $$ = $3; }
;

sibling_node: LBRACKET SIBLING ast_node RBRACKET { $$ = $3; }
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

    yyrun(INTERPRETATION);
    /* ydebug(1); */

    fclose(file);
    return 0;
}
