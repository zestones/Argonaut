%{
    #include "../table_management/func_proc_manager.h"
    #include "../table_management/structure_manager.h"
    #include "../table_management/variable_manager.h"
    #include "../table_management/array_manager.h"

    #include "../symbol_table/declaration_table.h"
    #include "../symbol_table/hash_table.h"

    #include "../lexer/lexeme_table.h"
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
    int current_lexeme_code;

    void yyerror(const char *s) {
        // TODO: Uncomment this line to prevent the program from exiting on syntax errors
        // TODO: Uncomment only if syntax errors are all handled properly in grammar rules
        // if (error.type == NO_ERROR) return;

        if (!strcmp(s, "syntax error")) {
            set_error_message(&error,
                "Unexpected token found: '%s'.\n"
                "\t> This error is critical and will cause the program to terminate.\n"
                "\t> Exiting due to a syntax error.",
                yytext);
        } 
        else {
            set_error_message(&error,
                "Unexpected token found: '%s'.\n"
                "\t> %s\n"
                "\t> This error is critical and will cause the program to terminate.\n"
                "\t> Exiting due to a syntax error.",
                yytext, s);
        }

        
        set_error_type(&error, SYNTAX_ERROR);
        yerror(error);
        
        exit(EXIT_FAILURE);
    }
%}

%union {
	int lexicographic_index;
    int ival;
    double fval;
}


%token PROG
%token SEMICOLON TWO_POINTS COMMA OPEN_PARENTHESIS CLOSE_PARENTHESIS DOT
%token START END
%token ARRAY OF OPEN_BRACKET CLOSE_BRACKET
%token VARIABLE OPAFF
%token STRUCT FSTRUCT
%token <lexicographic_index> IDENTIFIER TYPE INTEGER_TYPE FLOAT_TYPE BOOLEAN_TYPE CHARACTER_TYPE STRING_TYPE
%token PROCEDURE FUNCTION RETURN_TYPE RETURN_VALUE
%token IF ELSE WHILE
%token EQUAL NOT_EQUAL LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL
%token PRINT INPUT

%token <ival> INTEGER_VALUE CHARACTER_VALUE
%token <fval> FLOAT_VALUE
%token <lexicographic_index> BOOLEAN_VALUE STRING_VALUE

%left AND OR 
%right NOT

%right RETURN_TYPE RETURN_VALUE
%left SEMICOLON COMMA

%left PLUS MINUS
%left MULTIPLY DIVIDE

%type <lexicographic_index> type

%start program
%debug
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

comparison_operator: EQUAL
                   | NOT_EQUAL
                   | LESS_THAN
                   | GREATER_THAN
                   | LESS_EQUAL
                   | GREATER_EQUAL
                   ;

// Declarations
declaration_list: declaration declaration_list 
                | 
                ;

declaration: variable_declaration 
           | function_declaration 
           | type_declaration
           | procedure_declaration 
           ;

variable_declaration: VARIABLE IDENTIFIER TWO_POINTS type SEMICOLON { declaration_variable_start($2, $4); }
                    ;

function_declaration: FUNCTION IDENTIFIER { construct_func_proc_manager_context($2); declaration_func_start(); } OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS RETURN_TYPE type { update_declaration_func_return_type($8); } START declaration_list statement_list return_statement END { declaration_func_proc_end(); } 
                    ;

procedure_declaration: PROCEDURE IDENTIFIER { construct_func_proc_manager_context($2); declaration_proc_start(); } OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS START declaration_list statement_list END { declaration_func_proc_end(); } 
                     ;

type_declaration: TYPE IDENTIFIER TWO_POINTS STRUCT { construct_structure_manager_context($2); } START { declaration_structure_start(); } complex_type_fields END FSTRUCT SEMICOLON { declaration_structure_end(); }
                | TYPE IDENTIFIER TWO_POINTS ARRAY { construct_array_manager_context($2); declaration_array_start(); } dimension OF type SEMICOLON { declaration_array_end($8); }
                ;

argument_list: argument_list COMMA expression
    | expression
    | 
    ;

parameter_list: parameter_list COMMA parameter
    | parameter
    | 
    ;

parameter: IDENTIFIER TWO_POINTS type { func_proc_add_parameter($1, $3); }
         ;

dimension: OPEN_BRACKET list_dimensions CLOSE_BRACKET 
         ;

list_dimensions: one_dimension
               | list_dimensions COMMA one_dimension 
               ;

one_dimension: INTEGER_VALUE TWO_POINTS INTEGER_VALUE { array_add_dimension($1, $3); }
              ;

// Arithmetic expressions
expression: expression PLUS expression
          | expression MINUS expression
          | expression MULTIPLY expression
          | expression DIVIDE expression
          | expression_atom 
          ;

expression_atom: function_call_expression  
               | array_access_statement
               | struct_access_statement
               | IDENTIFIER  { check_variable_definition($1); }
               | INTEGER_VALUE
               | FLOAT_VALUE
               | BOOLEAN_VALUE
               | CHARACTER_VALUE
               | STRING_VALUE
               | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS 
               ;

// TODO : is there a better way to assign the lexicographic_index to the base type ?
type: INTEGER_TYPE { $$ = 0; }
    | FLOAT_TYPE   { $$ = 1; }
    | BOOLEAN_TYPE { $$ = 2; }
    | CHARACTER_TYPE { $$ = 3; }
    | STRING_TYPE OPEN_BRACKET INTEGER_VALUE CLOSE_BRACKET // FIXME: HELP I DONT KNOW HOW TO HANDLE THIS
    | IDENTIFIER 
    ;

complex_type_fields: type_field
                   | complex_type_fields type_field 
                   ;

type_field: IDENTIFIER TWO_POINTS type SEMICOLON { structure_add_field($1, $3); }
          ;

function_call_expression: IDENTIFIER { check_func_proc_definition($1); } OPEN_PARENTHESIS argument_list CLOSE_PARENTHESIS 
                        ;


// Statements
statement_block: START statement_list END ;


statement_list: statement statement_list
              | statement declaration { 
                set_error_type(&error, SYNTAX_ERROR);
                yyerror("Do not mix declarations and statements! All declarations must be at the beginning of the block.");
                exit(EXIT_FAILURE);
              }
              | 
              ;


statement: assignment_statement
    | if_statement
    | standalone_function_call_statement
    | loop_statement 
    | print_statement
    | input_statement
    ;

assignment_statement: IDENTIFIER { check_variable_definition($1); } OPAFF expression SEMICOLON 
                    | array_access_statement OPAFF expression SEMICOLON
                    | struct_access_statement OPAFF expression SEMICOLON
                    ;

return_statement: RETURN_VALUE expression SEMICOLON
                ;

if_statement: IF condition statement_block
            | IF condition statement_block ELSE statement_block;

loop_statement: WHILE condition statement_block ;

standalone_function_call_statement: function_call_expression SEMICOLON ;

array_access_statement: IDENTIFIER OPEN_BRACKET array_indices CLOSE_BRACKET;

array_indices: expression
             | expression COMMA array_indices;

struct_access_statement: IDENTIFIER DOT IDENTIFIER
                       | struct_access_statement DOT IDENTIFIER
                       | array_access_statement DOT IDENTIFIER
                       ;

print_statement: PRINT OPEN_PARENTHESIS argument_list CLOSE_PARENTHESIS SEMICOLON
               ;

input_statement: INPUT OPEN_PARENTHESIS assignable_entity CLOSE_PARENTHESIS SEMICOLON
               ;

assignable_entity: IDENTIFIER
  | array_access_statement
  | struct_access_statement
  | assignable_entity COMMA assignable_entity
  ;

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
    
    error.line = 1;
    error.column = 1;
    error.type = NO_ERROR;
    
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

    ydebug(verbose);
    return 0;
}
