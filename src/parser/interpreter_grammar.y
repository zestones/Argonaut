%{
    #include "../symbol_table/representation/representation_table.h"
    #include "../symbol_table/declaration/declaration_table.h"
    #include "../symbol_table/lexeme/lexeme_table.h"
    #include "../data/region_table.h"

    #include "../utils/errors.h"
    #include "../utils/ir_version.h"
    #include "../lexer/escapes.h"

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    int yylex(void);
    void yyerror(const char *s);
    extern FILE *yyin;
    extern FILE *yyout;
    extern char *yytext;

    void yyerror(const char *s) {
        fatal_syntax_error(yytext, s);
    }

    /**
     * @brief Loads one row of the lexeme table, "|index|lexeme|length|next|".
     *
     * The lexeme is stored verbatim (it may contain '|'), so the two integer fields are read
     * from the end of the row. The lexer guarantees the row has this shape.
     */
    static void insert_lexeme_row_from_text(char *row) {
        char *end = row + strlen(row) - 1;            // the closing '|'
        char *next_field = end;
        while (next_field > row && next_field[-1] != '|') next_field--;
        char *length_field = next_field - 1;          // the '|' before 'next'
        while (length_field > row && length_field[-1] != '|') length_field--;
        char *lexeme_end = length_field - 1;          // the '|' after the lexeme
        char *lexeme = strchr(row + 1, '|');          // the '|' after the index

        if (lexeme == NULL || lexeme >= lexeme_end || next_field <= row || length_field <= row) {
            yyerror("Malformed lexeme table row");
        }

        int index = (int) strtol(row + 1, NULL, 10);
        int length = (int) strtol(length_field, NULL, 10);
        int next = (int) strtol(next_field, NULL, 10);
        *lexeme_end = '\0';

        char *decoded = unescape_text(lexeme + 1);
        insert_lexeme_row(index, decoded, length, next);
        free(decoded);
    }
%}

%code requires {
    #include "../ast/ast.h"
}

%union {
    AST ast;

    int integer;
    char* string;
}


%token IR_HEADER
%token BEGIN_LEXEME_TABLE END_LEXEME_TABLE
%token BEGIN_DECLARATION_TABLE END_DECLARATION_TABLE
%token BEGIN_REPRESENTATION_TABLE END_REPRESENTATION_TABLE
%token BEGIN_REGION_TABLE END_REGION_TABLE
%token NODE_LPAREN NODE_RPAREN COMMA LBRACKET RBRACKET CHILD SIBLING

%token PIPE

%token <integer> TOKEN_INTEGER
%token <string> TOKEN_LEXEME_ROW
%start program

%type <ast> ast_node child_node sibling_node node

%%

program: header lexeme_table declaration_table representation_table region_table
;

header: IR_HEADER TOKEN_INTEGER {
            if ($2 != ARGONAUT_IR_VERSION) {
                fprintf(stderr, COLOR_RED "Unsupported IR version %d: this argov reads version %d.\n"
                                "  Recompile the program with the matching argoc." COLOR_RESET "\n",
                        $2, ARGONAUT_IR_VERSION);
                exit(EXIT_FAILURE);
            }
        }
;



lexeme_table: BEGIN_LEXEME_TABLE lexeme_table_body END_LEXEME_TABLE
;

lexeme_table_body: lexeme_table_row
                 | lexeme_table_body lexeme_table_row
;

lexeme_table_row: TOKEN_LEXEME_ROW { insert_lexeme_row_from_text($1); free($1); }
;



declaration_table: BEGIN_DECLARATION_TABLE declaration_table_body END_DECLARATION_TABLE
;

declaration_table_body: declaration_table_row 
                      | declaration_table_body declaration_table_row
;

declaration_table_row: PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE { insert_declaration_row($2, $4, $6, $8, $10, $12); }
;



representation_table: BEGIN_REPRESENTATION_TABLE representation_table_body END_REPRESENTATION_TABLE
;

representation_table_body:
                         | representation_table_body representation_table_row
;

representation_table_row: PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE { insert_representation_row($2, $4); }
;



region_table: BEGIN_REGION_TABLE region_table_body END_REGION_TABLE
;

region_table_body: region_table_row
                | region_table_body region_table_row
;


region_table_row: PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE ast_node PIPE { insert_region_row($2, $4, $6, $8); }
;

ast_node: node child_node sibling_node { append_child($1, $2); add_sibling($1, $3); }
        | node child_node { append_child($1, $2); }
        | node sibling_node { add_sibling($1, $2); }
        | node { $$ = $1; }
;

node: NODE_LPAREN TOKEN_INTEGER COMMA TOKEN_INTEGER COMMA TOKEN_INTEGER NODE_RPAREN { $$ = construct_node($2, $4, $6); }
;

child_node: LBRACKET CHILD ast_node RBRACKET { $$ = $3; }
;

sibling_node: LBRACKET SIBLING ast_node RBRACKET { $$ = $3; }
;

%%
