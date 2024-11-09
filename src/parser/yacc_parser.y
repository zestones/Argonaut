%{
    #include "../symbol_table/declaration_table.h"
    #include "../table_management/array_manager.h"
    #include "../lexer/lexeme_table.h"
    #include "../utils/hash.h"

    #include "parser.h"

    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>

    int yylex();

    extern FILE *yyin;
    extern FILE *yyout;

    extern int error_line;
    extern char *yytext;

    int current_lexeme_code;
%}

%union {
	int lexicographic_index;
    int ival;
}


%token PROG
%token SEMICOLON TWO_POINTS COMMA DOT_DOT OPEN_PARENTHESIS CLOSE_PARENTHESIS
%token START END
%token ARRAY OF OPEN_BRACKET CLOSE_BRACKET
%token VARIABLE OPAFF
%token STRUCT FSTRUCT
%token <lexicographic_index> IDENTIFIER INTEGER TYPE FLOAT BOOLEAN CHARACTER STRING
%token PROCEDURE FUNCTION RETURN_TYPE RETURN_VALUE
%token IF ELSE WHILE
%token EQUAL NOT_EQUAL LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL

%left AND OR 
%right NOT

%left PLUS MINUS
%left MULTIPLY DIVIDE

%type <lexicographic_index> type
%%

program: PROG declaration_list statement_list
       | 
       ;

// Conditions and boolean expressions
condition: OPEN_PARENTHESIS expression comparison_operator expression CLOSE_PARENTHESIS
         | OPEN_PARENTHESIS condition CLOSE_PARENTHESIS
         | condition AND condition
         | condition OR condition
         | NOT condition
         | NOT expression
         ;

loop: WHILE condition statement_block ;

comparison_operator: EQUAL
                   | NOT_EQUAL
                   | LESS_THAN
                   | GREATER_THAN
                   | LESS_EQUAL
                   | GREATER_EQUAL
                   ;

// Declarations
declaration_list: declaration_list declaration
                | ;

declaration: variable_declaration 
           | function_declaration 
           | type_declaration
           | procedure_declaration 
           ;

variable_declaration: VARIABLE IDENTIFIER TWO_POINTS type SEMICOLON 
                      { insert_declaration_var($2, $4); }
                    ;

function_declaration: FUNCTION IDENTIFIER OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS RETURN_TYPE type START declaration_list statement_list return_statement END ;

procedure_declaration: PROCEDURE IDENTIFIER OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS START declaration_list statement_list END ;

// TODO: fix the insertion of the struct and array types as the description is the lex index type !!
type_declaration: TYPE IDENTIFIER TWO_POINTS STRUCT START complex_type_fields END FSTRUCT SEMICOLON
                  { insert_declaration_struct($2, $1); }
                | TYPE IDENTIFIER TWO_POINTS ARRAY { construct_array_manager_context($2); declaration_array_start(); } dimension OF type SEMICOLON { declaration_array_end(); }
                  
                ;

argument_list: argument_list COMMA expression
    | expression
    | ;

parameter_list: parameter_list COMMA parameter
    | parameter
    | ;

parameter: IDENTIFIER TWO_POINTS type ;

dimension: OPEN_BRACKET list_dimensions CLOSE_BRACKET 
         ;

list_dimensions: one_dimension
               | list_dimensions COMMA one_dimension ;

one_dimension: INTEGER DOT_DOT INTEGER 
              { array_add_dimension($1, $3); }
              ;

// Arithmetic expressions
expression: expression PLUS expression
          | expression MINUS expression
          | expression MULTIPLY expression
          | expression DIVIDE expression
          | expression_atom 
          ;

expression_atom: function_call_expression  
               | IDENTIFIER
               | INTEGER
               | FLOAT
               | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS 
               ;

// TODO : is there a better way to assign the lexicographic_index to the base type?
type: INTEGER { $$ = 0;}
    | FLOAT   { $$ = 1;}
    | BOOLEAN { $$ = 2;}
    | CHARACTER { $$ = 3;}
    | STRING OPEN_BRACKET INTEGER CLOSE_BRACKET // HELP I DONT KNOW HOW TO HANDLE THIS
    | IDENTIFIER 
    ;

complex_type_fields: type_field
                   | complex_type_fields type_field 
                   ;

type_field: IDENTIFIER TWO_POINTS type SEMICOLON 
          ;

function_call_expression: IDENTIFIER OPEN_PARENTHESIS argument_list CLOSE_PARENTHESIS 
                        ;


// Statements
statement_block: START statement_list END ;

statement_list: statement_list statement
              | ;

statement: assignment_statement
    | if_statement
    | standalone_function_call_statement
    | loop_statement ;

assignment_statement: IDENTIFIER OPAFF expression SEMICOLON 
                    ;

return_statement: RETURN_VALUE IDENTIFIER SEMICOLON;

if_statement: IF condition statement_block
            | IF condition statement_block ELSE statement_block;

loop_statement: loop;

standalone_function_call_statement: function_call_expression SEMICOLON ;



%%

static void print_usage(const char *program_name) {
    fprintf(stdout, COLOR_BOLD COLOR_UNDERLINE "\nUsage:" COLOR_RESET);
    fprintf(stdout, COLOR_GREEN " %s -f <file> [-v] [-h]\n" COLOR_RESET, program_name);

    fprintf(stdout, "\n");

    fprintf(stdout, COLOR_BOLD "Options:\n" COLOR_RESET);
    fprintf(stdout, "  " COLOR_YELLOW "-f <file>      " COLOR_RESET "The input file to be processed.\n");
    fprintf(stdout, "  " COLOR_YELLOW "-v             " COLOR_RESET "Enable verbose mode (display lexeme table).\n");
    fprintf(stdout, "  " COLOR_YELLOW "-h             " COLOR_RESET "Show help (this usage information).\n");

    fprintf(stdout, "\n");

    fprintf(stdout, COLOR_BOLD "Description:\n" COLOR_RESET);
    fprintf(stdout, "  This program compiles the specified input file.\n\n");

    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    int opt, verbose = 0;
    
    if (argc == 1) {
        // No arguments provided
        print_usage(argv[0]);
    }

    while ((opt = getopt(argc, argv, "f:vh")) != -1) {
        switch (opt) {
            case 'f':
                FILE *file = fopen(optarg, "r");
                if (!file) {
                    fprintf(stderr, COLOR_RED "Error: Could not open file %s\n" COLOR_RESET, optarg);
                    return 1;
                }
                
                yyin = file;
                yyrun();

                fclose(file);
                break;
            case 'v':
                verbose = 1;
                break;
            case 'h':
                print_usage(argv[0]);
                break;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    yydebug(verbose);
    return 0;
}
