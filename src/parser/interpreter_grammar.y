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
        set_error_message(&error,
            "Unexpected token found: '%s' at %s.\n"
            "\t> %s\n"
            "\t> This error is critical and will cause the program to terminate.\n"
            "\t> Exiting due to a syntax error.",
            yytext,
            get_formatted_location(),
            s
        );

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

%token <integer> TOKEN_INTEGER
%token <string> TOKEN_LEXEME
%token <real> TOKEN_FLOAT
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

lexeme_table_row: PIPE TOKEN_INTEGER PIPE TOKEN_LEXEME PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE { insert_lexeme_row($2, $4, $6, $8); }
                | PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE {
                    int len = snprintf(NULL, 0, "%d", $4);  
                    char* lexeme = malloc(len + 1);
                    sprintf(lexeme, "%d", $4);

                    insert_lexeme_row($2, lexeme, $6, $8);
                    free(lexeme);
                }
                | PIPE TOKEN_INTEGER PIPE TOKEN_FLOAT PIPE TOKEN_INTEGER PIPE TOKEN_INTEGER PIPE {
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

static void print_usage(const char *program_name) {
    // Header
    fprintf(stdout, COLOR_BOLD COLOR_UNDERLINE "\nUsage:" COLOR_RESET "\n");
    fprintf(stdout, COLOR_GREEN "  %s -f <file> [-v] [-h]\n" COLOR_RESET, program_name);

    fprintf(stdout, "\n");

    // Options section
    fprintf(stdout, COLOR_BOLD "Options:\n" COLOR_RESET);
    fprintf(stdout, "  " COLOR_YELLOW "-f <file>      " COLOR_RESET "The input file to be interpreted.\n");
    fprintf(stdout, "  " COLOR_YELLOW "-v             " COLOR_RESET "Enable verbose mode (display symbol tables and AST).\n");
    fprintf(stdout, "  " COLOR_YELLOW "-h             " COLOR_RESET "Show this help message.\n");

    fprintf(stdout, "\n");

    // Description section
    fprintf(stdout, COLOR_BOLD "Description:\n" COLOR_RESET);
    fprintf(stdout, "  This program interprets the specified input file and optionally displays\n");
    fprintf(stdout, "  additional information such as the symbol tables and abstract syntax tree (AST).\n");

    fprintf(stdout, "\n");

    // Example section
    fprintf(stdout, COLOR_BOLD COLOR_BLUE "Example:\n" COLOR_RESET);
    fprintf(stdout, COLOR_GREEN "  %s -f my_program.txt -v\n" COLOR_RESET, program_name);

    fprintf(stdout, "\n");

    exit(EXIT_SUCCESS);
}


int main(int argc, char **argv) {
    int verbose_mode = 0;  // Flag to enable verbose mode
    char *input_file = NULL;  // File to interpret
    int opt;

    // Parse command-line options
    while ((opt = getopt(argc, argv, "f:vh")) != -1) {
        switch (opt) {
            case 'f':
                input_file = optarg;  // Capture the file argument
                break;
            case 'v':
                verbose_mode = 1;  // Enable verbose mode
                break;
            case 'h':
                print_usage(argv[0]);  // Display help message
                return 0;
            default:
                fprintf(stderr, "Error: Unknown option '-%c'\n", optopt);
                print_usage(argv[0]);
                return 1;
        }
    }

    // Check if the file argument was provided
    if (!input_file) {
        fprintf(stderr, COLOR_RED "Error: No input file specified.\n" COLOR_RESET);
        print_usage(argv[0]);
        return 1;
    }

    // Open the file
    FILE *file = fopen(input_file, "r");
    if (!file) {
        fprintf(stderr, COLOR_RED "Error: Could not open file '%s'.\n" COLOR_RESET, input_file);
        perror("\t> Reason");
        return 1;
    }

    // Initialize and run the interpreter
    error.filename = input_file;
    yyin = file;
    
    yyrun(INTERPRETATION);
    fclose(file);

    ydebug(verbose_mode);

    return 0;
}
