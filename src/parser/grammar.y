%{
    #include "../table_management/func_proc_manager.h"
    #include "../table_management/structure_manager.h"
    #include "../table_management/variable_manager.h"
    #include "../table_management/array_manager.h"

    #include "../symbol_table/declaration/declaration_table.h"
    #include "../symbol_table/hash/hash_table.h"
    #include "../data/region_table.h"

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
    AST ast;
	int lexicographic_index;
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

%token <lexicographic_index> BOOLEAN_VALUE STRING_VALUE INTEGER_VALUE CHARACTER_VALUE FLOAT_VALUE

%right NOT
%left AND OR 

%right RETURN_TYPE RETURN_VALUE
%left SEMICOLON COMMA

%left PLUS MINUS
%left MULTIPLY DIVIDE

%type <lexicographic_index> type 

%type <ast> assignment_statement if_statement loop_statement standalone_func_proc_call_statement func_proc_call_expression
%type <ast> array_access_statement array_indices struct_access_statement print_statement input_statement

%type <ast> variable_declaration type_declaration complex_type_fields type_field
%type <ast> function_declaration return_statement procedure_declaration parameter_list parameter argument_list

%type <ast> program declaration_list statement_list statement_block declaration statement expression expression_atom
%type <ast> condition comparison_operator

%type <ast> input_argument_list format_string 

%start program
%debug
%%

program: PROG declaration_list statement_list { 
            $$ = construct_node_default(A_PROGRAM); 

            add_child($$, $2); 
            add_sibling($2, $3);

            update_region_ast(peek_region(), $$);
        }
        | { $$ = NULL; }
       ;
     
// Declarations
declaration_list: declaration declaration_list {    
                    if (!is_node_null($2)) {
                        $$ = $2;
                        add_sibling($1, $2);
                        add_child($$, $1);
                    } else {
                        $$ = construct_node_default(A_DECLARATION_LIST);
                        add_child($$, $1);
                    }
                }
                | { $$ = NULL; }
;

declaration: variable_declaration { $$ = $1; }
           | function_declaration  { $$ = $1; }
           | type_declaration      { $$ = $1; }
           | procedure_declaration { $$ = $1; }
;

variable_declaration: VARIABLE IDENTIFIER TWO_POINTS type SEMICOLON { 
                        declaration_variable_start($2, $4);
                        $$ = construct_node(A_VARIABLE_DECLARATION, $2, get_arr_struct_declaration_index($4));
                    }
;

function_declaration: FUNCTION IDENTIFIER {
                        construct_func_proc_manager_context($2);
                        declaration_func_start();
                    } OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS RETURN_TYPE type {
                        update_declaration_func_return_type($8);
                    } START declaration_list statement_list return_statement END {
                        $$ = construct_node(A_FUNCTION_DECLARATION, $2, find_declaration_index_by_nature($2, TYPE_FUNC));

                        Node *nodes[] = { $5, $11, $12, $13 };
                        add_chain($$, nodes, 4);

                        update_region_ast(peek_region(), $$);
                        declaration_func_proc_end();

                        // Keep the declaration in the region
                        $$ = construct_node(A_FUNCTION_DECLARATION, $2, find_declaration_index_by_nature($2, TYPE_FUNC));
                        check_func_prototype($8, $13);
                    } 
;

procedure_declaration: PROCEDURE IDENTIFIER { 
                        construct_func_proc_manager_context($2);
                        declaration_proc_start(); 
                     } OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS START declaration_list statement_list END {
                        $$ = construct_node(A_PROCEDURE_DECLARATION, $2, find_declaration_index_by_nature($2, TYPE_PROC));
                        
                        Node *nodes[] = { $5, $8, $9 };
                        add_chain($$, nodes, 3);

                        update_region_ast(peek_region(), $$);
                        declaration_func_proc_end();

                        // Keep the declaration in the region
                        $$ = construct_node(A_PROCEDURE_DECLARATION, $2, find_declaration_index_by_nature($2, TYPE_PROC));
                     } 
;

type_declaration: TYPE IDENTIFIER TWO_POINTS STRUCT { 
                    construct_structure_manager_context($2);
                } START {
                    declaration_structure_start();
                } complex_type_fields END FSTRUCT SEMICOLON {
                    declaration_structure_end();
                    $$ = construct_node(A_TYPE_STRUCT_DECLARATION, $2, find_declaration_index($2));
                    add_child($$, $8);
                }
                | TYPE IDENTIFIER TWO_POINTS ARRAY {
                    construct_array_manager_context($2);
                    declaration_array_start();
                } dimension OF type SEMICOLON {
                    declaration_array_end($8);
                    $$ = construct_node(A_TYPE_ARRAY_DECLARATION, $2, find_declaration_index($2));
                }
;

argument_list: expression {
                $$ = construct_node_default(A_ARGUMENT_LIST);
                Node* single_argument = construct_node_default(A_ARGUMENT);
                add_child(single_argument, $1);
                add_child($$, single_argument);
            }
            | argument_list COMMA expression {
                Node* single_argument = construct_node_default(A_ARGUMENT);
                add_child(single_argument, $3); 
                add_sibling($1->child, single_argument);
                $$ = $1;
            }
            | { $$ = NULL; }
;


parameter_list: parameter {
                    $$ = construct_node_default(A_PARAMETER_LIST);
                    add_child($$, $1); 
                }
                | parameter_list COMMA parameter {
                    if (!is_node_null($1)) {
                        add_sibling($1->child, $3);
                    }

                    $$ = $1;
                }
                | { $$ = NULL; }
;


parameter: IDENTIFIER TWO_POINTS type { 
            func_proc_add_parameter($1, $3);
            $$ = construct_node(A_PARAMETER, $1, find_declaration_index_by_nature($1, TYPE_PARAM));    
        }
;

dimension: OPEN_BRACKET list_dimensions CLOSE_BRACKET 
;

list_dimensions: one_dimension
               | list_dimensions COMMA one_dimension 
;

one_dimension: INTEGER_VALUE TWO_POINTS INTEGER_VALUE { array_add_dimension($1, $3); }
;

// Arithmetic expressions
expression: expression PLUS expression {
                $$ = construct_node_default(A_ADD_OP);
                add_child($$, $1);
                add_sibling($1, $3);
           }
          | expression MINUS expression {
                $$ = construct_node_default(A_SUB_OP);
                add_child($$, $1);
                add_sibling($1, $3);
          }
          | expression MULTIPLY expression {
                $$ = construct_node_default(A_MUL_OP);
                add_child($$, $1);
                add_sibling($1, $3);
          }
          | expression DIVIDE expression {
                $$ = construct_node_default(A_DIV_OP);
                add_child($$, $1);
                add_sibling($1, $3);
          }
          | expression_atom { $$ = $1; }
          | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS { $$ = $2; }
;

expression_atom: func_proc_call_expression { $$ = $1; }  
               | array_access_statement { $$ = $1; }
               | struct_access_statement { $$ = $1; }
               | IDENTIFIER  { check_variable_definition($1); $$ = construct_node(A_IDENTIFIER, $1, find_declaration_index($1)); }
               | INTEGER_VALUE { $$ = construct_node(A_INTEGER_LITERAL, $1, NULL_VALUE); }
               | FLOAT_VALUE { $$ = construct_node(A_FLOAT_LITERAL, $1, NULL_VALUE); }
               | BOOLEAN_VALUE { $$ = construct_node(A_BOOLEAN_LITERAL, $1, NULL_VALUE); }
               | CHARACTER_VALUE { $$ = construct_node(A_CHARACTER_LITERAL, $1, NULL_VALUE); }
               | STRING_VALUE { $$ = construct_node(A_STRING_LITERAL, $1, NULL_VALUE); }
;

// TODO : is there a better way to assign the lexicographic_index to the base type ?
type: INTEGER_TYPE { $$ = 0; }
    | FLOAT_TYPE   { $$ = 1; }
    | BOOLEAN_TYPE { $$ = 2; }
    | CHARACTER_TYPE { $$ = 3; }
    | STRING_TYPE OPEN_BRACKET INTEGER_VALUE CLOSE_BRACKET // FIXME: HELP I DONT KNOW HOW TO HANDLE THIS
    | IDENTIFIER 
;

complex_type_fields: type_field { 
                        $$ = construct_node_default(A_STRUCT_FIELD_LIST);
                        add_child($$, $1);
                    }
                   | complex_type_fields type_field {
                        if (!is_node_null($1)) {
                            $$ = $1;
                            add_sibling($2, $1);
                            add_child($$, $2);
                        }
                   };
;

type_field: IDENTIFIER TWO_POINTS type SEMICOLON { 
                structure_add_field($1, $3);
                // TODO: struct fields are not added to the declaration table
                // TODO: should we insert in the ast the declaration type ?
                $$ = construct_node(A_STRUCT_FIELD, $1, find_declaration_index($3));
            }
;

func_proc_call_expression: IDENTIFIER { check_func_proc_definition($1); } OPEN_PARENTHESIS argument_list CLOSE_PARENTHESIS {
                            $$ = construct_node(A_FUNC_PROC_CALL_STATEMENT, $1, get_func_proc_declaration_index($1));
                            add_child($$, $4);
                            check_func_proc_argument_list($1, $4);
                        }
;

// Conditions and boolean expressions
condition: OPEN_PARENTHESIS expression_atom comparison_operator expression_atom CLOSE_PARENTHESIS {
            $$ = construct_node_default(A_CONDITION);
            add_child($$, $3);
            add_child($3, $2);
            add_sibling($2, $4);
            check_condition($$);
         }
         | OPEN_PARENTHESIS condition CLOSE_PARENTHESIS {
            $$ = $2;
         }
         | condition AND condition {
            $$ = construct_node_default(A_AND_CONDITION);
            add_child($$, $1);
            add_sibling($1, $3);
         }
         | condition OR condition {
            $$ = construct_node_default(A_OR_CONDITION);
            add_child($$, $1);
            add_sibling($1, $3);
         }
         | NOT condition {
            $$ = construct_node_default(A_NOT_CONDITION);
            add_child($$, $2);
         }
;

comparison_operator: EQUAL { $$ = construct_node_default(A_EQUAL_OP); }
                   | NOT_EQUAL { $$ = construct_node_default(A_NOT_EQUAL_OP); }
                   | LESS_THAN { $$ = construct_node_default(A_LESS_THAN_OP); }
                   | GREATER_THAN { $$ = construct_node_default(A_GREATER_THAN_OP); }
                   | LESS_EQUAL { $$ = construct_node_default(A_LESS_EQUAL_OP); }
                   | GREATER_EQUAL { $$ = construct_node_default(A_GREATER_EQUAL_OP); }
;

// Statements
statement_block: START statement_list END { $$ = $2; }
;

statement_list: statement statement_list {
                    if (!is_node_null($2)) {
                        $$ = $2;
                        add_sibling($1, $2);
                        add_child($$, $1);
                    } else {
                        $$ = construct_node_default(A_STATEMENT_LIST);
                        add_child($$, $1);
                    }
              }
              | statement declaration { 
                set_error_type(&error, SYNTAX_ERROR);
                yyerror("Do not mix declarations and statements! All declarations must be at the beginning of the block.");
                exit(EXIT_FAILURE);
              } 
              | { $$ = NULL; }
;

statement: assignment_statement {
            $$ = construct_node_default(A_ASSIGNMENT_STATEMENT);
            add_child($$, $1);
        }
        | if_statement { $$ = $1; }
        | standalone_func_proc_call_statement { $$ = $1; }
        | loop_statement  { $$ = $1; }
        | print_statement { $$ = $1; }
        | input_statement {
            $$ = construct_node_default(A_INPUT_STATEMENT);
            add_child($$, $1);
        }
;

assignment_statement: IDENTIFIER { check_variable_definition($1); } OPAFF expression SEMICOLON {
                        $$ = construct_node(A_VARIABLE_ASSIGNMENT, $1, get_var_param_declaration_index($1));
                        add_child($$, $4);
                        check_variable_assignment($1, $4);
                    }
                    | array_access_statement OPAFF expression SEMICOLON {
                        $$ = construct_node_default(A_ARRAY_ASSIGNMENT);
                        add_child($$, $1);  
                        add_sibling($1, $3);
                        check_array_assignment($1, $3);
                    }
                    | struct_access_statement OPAFF expression SEMICOLON {
                        $$ = construct_node_default(A_STRUCT_ASSIGNMENT);
                        add_child($$, $1);  
                        add_sibling($1, $3);
                        check_struct_assignment($1, $3);
                    }
;

return_statement: RETURN_VALUE expression SEMICOLON { 
                    $$ = construct_node_default(A_RETURN_STATEMENT);
                    add_child($$, $2);
                }
;

if_statement: IF condition statement_block {
                $$ = construct_node_default(A_IF);
                add_child($$, $2);
            }
            | IF condition statement_block ELSE statement_block {
                $$ = construct_node_default(A_IF_ELSE); 
                add_child($$, $2); 
                add_sibling($2, $3);
                add_sibling($3, $5);
            }
;

loop_statement: WHILE condition statement_block {
                $$ = construct_node_default(A_WHILE); 
                add_child($$, $2); 
                add_sibling($2, $3);
            }
;

standalone_func_proc_call_statement: func_proc_call_expression SEMICOLON { $$ = $1; }
;

array_access_statement: IDENTIFIER OPEN_BRACKET array_indices CLOSE_BRACKET {
                            $$ = construct_node(A_ARRAY_ACCESS, $1, find_declaration_index($1));
                            add_child($$, $3);
                    }
                    | IDENTIFIER OPEN_BRACKET array_indices CLOSE_BRACKET DOT IDENTIFIER {
                            $$ = construct_node(A_ARRAY_ACCESS, $1, find_declaration_index($1));
                            Node *field = construct_node(A_STRUCT_FIELD_ACCESS, $6, find_declaration_index($6));
                            add_child($$, $3);
                            add_sibling($3, field);
                    }
                    | IDENTIFIER OPEN_BRACKET array_indices CLOSE_BRACKET DOT struct_access_statement {
                            $$ = construct_node(A_ARRAY_ACCESS, $1, find_declaration_index($1));
                            add_child($$, $3);
                            add_sibling($3, $6);
                    }
;

array_indices: expression {
                $$ = construct_node_default(A_ARRAY_INDEX_LIST);
                Node* single_index = construct_node_default(A_ARRAY_INDEX);
            
                add_child($$, single_index);
                add_child(single_index, $1);
            }
            | array_indices COMMA expression {
                Node* single_index = construct_node_default(A_ARRAY_INDEX);

                add_child(single_index, $3);  
                add_sibling($1->child, single_index);
                
                $$ = $1; // Preserve the existing list structure
            }
;

struct_access_statement: IDENTIFIER DOT IDENTIFIER {
                            $$ = construct_node(A_STRUCT_FIELD_ACCESS, $1, find_declaration_index($1));
                            Node *field = construct_node(A_STRUCT_FIELD_ACCESS, $3, find_declaration_index($3));
                            add_child($$, field);
                       }
                       | IDENTIFIER DOT struct_access_statement {
                            $$ = construct_node(A_STRUCT_FIELD_ACCESS, $1, find_declaration_index($1));
                            add_child($$, $3);
                       }
                       | IDENTIFIER DOT array_access_statement {
                            $$ = construct_node(A_STRUCT_FIELD_ACCESS, $1, find_declaration_index($1));
                            add_child($$, $3);
                       }
;

print_statement: PRINT OPEN_PARENTHESIS format_string COMMA argument_list CLOSE_PARENTHESIS SEMICOLON {
                    $$ = construct_node_default(A_PRINT_STATEMENT);
                    add_child($$, $3);
                    add_child($$, $5);
                    check_print_proc_argument_list($3, $5);
               }
                | PRINT OPEN_PARENTHESIS format_string CLOSE_PARENTHESIS SEMICOLON {
                      $$ = construct_node_default(A_PRINT_STATEMENT);
                      add_child($$, $3);
                }
;

format_string: STRING_VALUE {
                    $$ = construct_node(A_FORMAT_STRING, $1, find_declaration_index($1));
              }
;

input_statement: INPUT OPEN_PARENTHESIS format_string COMMA input_argument_list CLOSE_PARENTHESIS SEMICOLON {
                    $$ = construct_node_default(A_INPUT_STATEMENT);
                    add_child($$, $3); 
                    add_child($$, $5);
                    check_input_proc_argument_list($3, $5);
               }
               | INPUT OPEN_PARENTHESIS format_string CLOSE_PARENTHESIS SEMICOLON {
                    $$ = construct_node_default(A_INPUT_STATEMENT);
                    add_child($$, $3);
               }
;

input_argument_list: IDENTIFIER {
                        $$ = construct_node_default(A_ARGUMENT_LIST);
                        Node *single_argument = construct_node_default(A_ARGUMENT);

                        add_child(single_argument, construct_node(A_IDENTIFIER, $1, find_declaration_index($1)));
                        add_child($$, single_argument);
                    }
                   | array_access_statement {
                        $$ = construct_node_default(A_ARGUMENT_LIST);
                        Node *single_argument = construct_node_default(A_ARGUMENT);

                        add_child(single_argument, $1);
                        add_child($$, single_argument);
                    }
                   | struct_access_statement {
                        $$ = construct_node_default(A_ARGUMENT_LIST);
                        Node *single_argument = construct_node_default(A_ARGUMENT);

                        add_child(single_argument, $1);
                        add_child($$, single_argument);
                    }
                   | input_argument_list COMMA IDENTIFIER {
                        Node *single_argument = construct_node_default(A_ARGUMENT);
                        add_child(single_argument, construct_node(A_IDENTIFIER, $3, find_declaration_index($3)));
                        add_sibling($1->child, single_argument);
                        $$ = $1;
                   }
                   | input_argument_list COMMA array_access_statement {
                        Node *single_argument = construct_node_default(A_ARGUMENT);
                        add_child(single_argument, $3);
                        add_sibling($1->child, single_argument);
                        $$ = $1;
                   }
                   | input_argument_list COMMA struct_access_statement {
                        Node *single_argument = construct_node_default(A_ARGUMENT);
                        add_child(single_argument, $3);
                        add_sibling($1->child, single_argument);
                        $$ = $1;
                   }
;


%%

static void print_usage(const char *program_name) {
    fprintf(stdout, COLOR_BOLD COLOR_UNDERLINE "\nUsage:" COLOR_RESET);
    fprintf(stdout, COLOR_GREEN " %s -f <file> [-v] [-h]\n" COLOR_RESET, program_name);

    fprintf(stdout, "\n");

    fprintf(stdout, COLOR_BOLD "Options:\n" COLOR_RESET);
    fprintf(stdout, "  " COLOR_YELLOW "-f <file>      " COLOR_RESET "The input file to be processed.\n");
    fprintf(stdout, "  " COLOR_YELLOW "-v             " COLOR_RESET "Enable verbose mode (display symbol table and ast).\n");
    fprintf(stdout, "  " COLOR_YELLOW "-h             " COLOR_RESET "Show help (this usage information).\n");

    fprintf(stdout, "\n");

    fprintf(stdout, COLOR_BOLD "Description:\n" COLOR_RESET);
    fprintf(stdout, "  This program compiles the specified input file.\n\n");

    exit(EXIT_SUCCESS);
}

static void handle_file_error(const char *filename) {
    fprintf(stderr, COLOR_RED "Error: Could not open file '%s'.\n", filename);
    perror("\t> Reason");
    fprintf(stderr, COLOR_RESET);

    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    int opt, verbose = 0;
    
    error.line = 1;
    error.column = 1;
    error.type = NO_ERROR;

    if (argc == 1) {
        print_usage(argv[0]);
    }

    char *input_file = NULL;

    while ((opt = getopt(argc, argv, "f:vh")) != -1) {
        switch (opt) {
            case 'f':
                input_file = optarg;
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

    if (input_file == NULL) {
        fprintf(stderr, COLOR_RED "Error: Missing required '-f <file>' argument.\n" COLOR_RESET);
        print_usage(argv[0]);
    }

    // Open the file for reading
    FILE *file = fopen(input_file, "r");
    if (!file) {
        handle_file_error(input_file);
    }

    error.filename = input_file;
    yyin = file;

    yyrun();
    fclose(file);

    if (verbose) {
        printf(COLOR_GREEN "Verbose mode enabled. Printing tables and ast...\n" COLOR_RESET);
        ydebug(verbose);
    }

    return 0;
}
