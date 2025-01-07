%{
    #include "../table_management/func_proc_manager.h"
    #include "../table_management/structure_manager.h"
    #include "../table_management/variable_manager.h"
    #include "../table_management/array_manager.h"

    #include "../symbol_table/declaration/declaration_table.h"
    #include "../symbol_table/lexeme/lexeme_table.h"
    #include "../symbol_table/hash/hash_table.h"
    #include "../symbol_table/utility.h"
    #include "../data/region_table.h"

    #include "parser.h"

    #include "../utils/errors.h"

    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <getopt.h>
    #include <string.h>


    int yylex();

    extern FILE *yyin;
    extern FILE *yyout;
    extern char *yytext;

    Error error;

    void yyerror(const char *s) {
        // TODO: Uncomment this line to prevent the program from exiting on syntax errors
        // TODO: Uncomment only if syntax errors are all handled properly in grammar rules
        // if (error.type == NO_ERROR) return;

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
%token IF ELSE WHILE FOR
%token EQUAL NOT_EQUAL LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL
%token PRINT INPUT

%token <lexicographic_index> BOOLEAN_VALUE STRING_VALUE INTEGER_VALUE CHARACTER_VALUE FLOAT_VALUE
%token <lexicographic_index> INCREMENT DECREMENT

%right NOT
%left AND OR 

%right RETURN_TYPE RETURN_VALUE
%left SEMICOLON COMMA

%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO

%type <lexicographic_index> type 

%type <ast> assignment_statement if_statement loop_statement for_statement standalone_func_proc_call_statement func_proc_call_expression
%type <ast> array_access_statement array_indices struct_access_statement print_statement input_statement

%type <ast> variable_declaration type_declaration complex_type_fields type_field
%type <ast> function_declaration return_statement procedure_declaration parameter_list parameter argument_list

%type <ast> program declaration_list statement_list statement_block declaration statement inc_dec_statement
%type <ast> expression expression_atom condition comparison_operator

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
                        $$ = construct_node(A_VARIABLE_DECLARATION, $2, find_declaration_index_by_nature($2, TYPE_VAR));
                    }
;

function_declaration: FUNCTION IDENTIFIER {
                        construct_func_proc_manager_context($2);
                        declaration_func_start();
                    } OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS RETURN_TYPE type {
                        update_declaration_func_return_type($8);
                    } START declaration_list statement_list END {
                        $$ = construct_node(A_FUNCTION_DECLARATION, $2, find_declaration_index_by_nature($2, TYPE_FUNC));

                        Node *nodes[] = { $5, $11, $12 };
                        add_chain($$, nodes, 3);

                        update_region_ast(peek_region(), $$);
                        declaration_func_proc_end();

                        // Keep the declaration in the region
                        $$ = construct_node(A_FUNCTION_DECLARATION, $2, find_declaration_index_by_nature($2, TYPE_FUNC));
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

one_dimension: INTEGER_VALUE TWO_POINTS INTEGER_VALUE { array_add_dimension(atoi(get_lexeme($1)), atoi(get_lexeme($3))); }
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
          | expression MODULO expression {
                $$ = construct_node_default(A_MODULO_OP);
                add_child($$, $1);
                add_sibling($1, $3);
          }
          | expression_atom { $$ = $1; }
          | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS { $$ = $2; }
;

expression_atom: IDENTIFIER  { check_variable_definition($1); $$ = construct_node(A_IDENTIFIER, $1, find_declaration_index($1)); }
               | INTEGER_VALUE { $$ = construct_node(A_INTEGER_LITERAL, $1, NULL_VALUE); }
               | FLOAT_VALUE { $$ = construct_node(A_FLOAT_LITERAL, $1, NULL_VALUE); }
               | BOOLEAN_VALUE { $$ = construct_node(A_BOOLEAN_LITERAL, $1, NULL_VALUE); }
               | CHARACTER_VALUE { $$ = construct_node(A_CHARACTER_LITERAL, $1, NULL_VALUE); }
               | STRING_VALUE { $$ = construct_node(A_STRING_LITERAL, $1, NULL_VALUE); }
               | func_proc_call_expression { $$ = $1; }  
               | array_access_statement { $$ = $1; }
               | struct_access_statement { $$ = $1; }
;

// TODO : is there a better way to assign the lexicographic_index to the base type ?
type: INTEGER_TYPE { $$ = 0; }
    | FLOAT_TYPE   { $$ = 1; }
    | BOOLEAN_TYPE { $$ = 2; }
    | CHARACTER_TYPE { $$ = 3; }
    | STRING_TYPE  // FIXME: HELP I DONT KNOW HOW TO HANDLE THIS
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
condition: expression comparison_operator expression {
            $$ = construct_node_default(A_CONDITION);
            add_child($$, $2);
            add_child($2, $1);
            add_sibling($1, $3);
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
                    $$ = $1;
                    if (!is_node_null($2)) {
                        $$ = $2;
                        add_sibling($1, $2);
                        add_child($$, $1);
                    } else {
                        $$ = construct_node_default(A_STATEMENT_LIST);
                        append_child($$, $1);
                    }
              }
              | statement declaration { 
                set_error_type(&error, SYNTAX_ERROR);
                yyerror("Do not mix declarations and statements! All declarations must be at the beginning of the block.");
                exit(EXIT_FAILURE);
              } 
              | { $$ = NULL; }
;

statement: assignment_statement SEMICOLON {
            $$ = construct_node_default(A_ASSIGNMENT_STATEMENT);
            add_child($$, $1);
        }
        | if_statement { $$ = $1; }
        | standalone_func_proc_call_statement { $$ = $1; }
        | loop_statement  { $$ = $1; }
        | for_statement   { $$ = $1; }  
        | print_statement { $$ = $1; }
        | input_statement { $$ = $1; }
        | return_statement{ $$ = $1; }
;

assignment_statement: IDENTIFIER { check_variable_definition($1); } OPAFF expression {
                        $$ = construct_node(A_VARIABLE_ASSIGNMENT, $1, get_var_param_declaration_index($1));
                        add_child($$, $4);
                        check_variable_assignment($1, $4);
                    }
                    | array_access_statement OPAFF expression {
                        $$ = construct_node_default(A_ARRAY_ASSIGNMENT);
                        add_child($$, $1);  
                        add_sibling($1, $3);
                        check_array_assignment($1, $3);
                    }
                    | struct_access_statement OPAFF expression {
                        $$ = construct_node_default(A_STRUCT_ASSIGNMENT);
                        add_child($$, $1);  
                        add_sibling($1, $3);
                        check_struct_assignment($1, $3);
                    }
                    | inc_dec_statement { $$ = $1; }
;

inc_dec_statement: IDENTIFIER DECREMENT {
                        $$ = construct_node(A_VARIABLE_ASSIGNMENT, $1, get_var_param_declaration_index($1));
                        
                        // Create the A_SUB_OP node
                        Node *op_node = construct_node_default(A_SUB_OP);
                        Node *identifier_node = construct_node(A_IDENTIFIER, $1, get_var_param_declaration_index($1));
                        Node *literal_node = construct_node(A_INTEGER_LITERAL, $2, NULL_VALUE);

                        // Build the operation tree
                        add_child(op_node, identifier_node);
                        add_sibling(identifier_node, literal_node);

                        // Attach operation tree as the child of the assignment node
                        add_child($$, op_node);

                        check_variable_assignment($1, op_node);
                    }
                    | IDENTIFIER INCREMENT {
                        $$ = construct_node(A_VARIABLE_ASSIGNMENT, $1, get_var_param_declaration_index($1));
                        
                        // Create the A_SUB_OP or A_ADD_OP node
                        Node *op_node = construct_node_default(A_ADD_OP);
                        Node *identifier_node = construct_node(A_IDENTIFIER, $1, get_var_param_declaration_index($1));
                        Node *literal_node = construct_node(A_INTEGER_LITERAL, $2, NULL_VALUE);

                        // Build the operation tree
                        add_child(op_node, identifier_node);
                        add_sibling(identifier_node, literal_node);

                        // Attach operation tree as the child of the assignment node
                        add_child($$, op_node);

                        check_variable_assignment($1, op_node);
                    }
;

return_statement: RETURN_VALUE expression SEMICOLON {
                    if (!peek_region()) {
                        set_error_type(&error, SYNTAX_ERROR);
                        yyerror("Return statement is not allowed outside of a function.");
                    }
                    int index_declaration = find_function_index_by_region(peek_region());
                    // Check if function exist otherwise we are inside a procedure
                    if (index_declaration == NULL_VALUE) {
                        set_error_type(&error, SYNTAX_ERROR);
                        yyerror("Return statement is not allowed outside of a function.");
                    }

                    $$ = construct_node_default(A_RETURN_STATEMENT);
                    add_child($$, $2);
                    check_func_prototype(get_func_return_type(index_declaration), $$);
                }
;

if_statement: IF OPEN_PARENTHESIS condition CLOSE_PARENTHESIS statement_block {
                $$ = construct_node_default(A_IF);
                add_child($$, $3);
                add_sibling($3, $5);
            }
            | IF OPEN_PARENTHESIS condition CLOSE_PARENTHESIS statement_block ELSE statement_block {
                $$ = construct_node_default(A_IF_ELSE); 
                add_child($$, $3); 
                add_sibling($3, $5);
                add_sibling($5, $7);
            }
            | IF OPEN_PARENTHESIS condition CLOSE_PARENTHESIS statement_block ELSE if_statement 
            {
                $$ = construct_node_default(A_IF_ELSE_IF);
                add_child($$, $3); 
                add_sibling($3, $5);
                add_sibling($5, $7);
            }
;

loop_statement: WHILE OPEN_PARENTHESIS condition CLOSE_PARENTHESIS statement_block {
                $$ = construct_node_default(A_WHILE); 
                add_child($$, $3);
                add_sibling($3, $5);
            }
;

for_statement: FOR OPEN_PARENTHESIS assignment_statement SEMICOLON condition SEMICOLON assignment_statement CLOSE_PARENTHESIS statement_block {
        $$ = construct_node_default(A_FOR_LOOP);
        Node *initialization = construct_node_default(A_VARIABLE_ASSIGNMENT);
        add_child($$, initialization);
        add_child(initialization, $3);

        add_sibling(initialization, $5);

        Node *update = construct_node_default(A_VARIABLE_ASSIGNMENT);
        add_sibling($5, update);
        add_child(update, $7);

        add_sibling(update, $9);
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
    // Header
    fprintf(stderr, COLOR_BOLD_YELLOW "\nUsage:\n" COLOR_RESET);
    fprintf(stderr, "  %s -a <input_file> [options]\n\n", program_name);

    // Description
    fprintf(stderr, COLOR_BOLD_YELLOW "Description:\n" COLOR_RESET);
    fprintf(stderr, COLOR_CYAN "  This program compiles the specified file and offers options for generating logs, verbose output,\n");
    fprintf(stderr, "  and exporting tables to an output file.\n");
    fprintf(stderr, "  Use this program to compile Argonaut code and analyze the intermediate steps of the process.\n\n" COLOR_RESET);

    // Arguments section (merged short and long options)
    fprintf(stderr, COLOR_BOLD_YELLOW "Arguments:\n" COLOR_RESET);
    fprintf(stderr, COLOR_GREEN "  -a <input_file>   " COLOR_RESET ": The source file to compile (required).\n");
    fprintf(stderr, COLOR_GREEN "  -v, --verbose     " COLOR_RESET ": Display verbose output (e.g., symbol table and AST).\n");
    fprintf(stderr, COLOR_GREEN "  -l, --log         " COLOR_RESET ": Enable logging during compilation.\n");
    fprintf(stderr, COLOR_GREEN "  -o <output_file>, --output <output_file> " COLOR_RESET ": Specify the output file to save results.\n");
    fprintf(stderr, COLOR_GREEN "  -h, --help        " COLOR_RESET ": Display this help information.\n");

    fprintf(stderr, "\n");

    // Examples section
    fprintf(stderr, COLOR_BOLD_YELLOW "Examples:\n" COLOR_RESET);
    fprintf(stderr, COLOR_CYAN "  %s -a my_program.arg -vl -o output.txt\n" COLOR_RESET, program_name);
    fprintf(stderr, "    - Compile the file 'my_program.arg' with verbose output and logging.\n");
    fprintf(stderr, "    - Results saved to 'output.txt'.\n\n");

    fprintf(stderr, COLOR_CYAN "  %s -a my_program.arg --verbose --log --output output.txt\n" COLOR_RESET, program_name);
    fprintf(stderr, "    - Compile the file 'my_program.arg' with long options.\n");
    fprintf(stderr, "    - Results saved to 'output.txt'.\n\n");

    // Constraints section
    fprintf(stderr, COLOR_BOLD_YELLOW "Constraints:\n" COLOR_RESET);
    fprintf(stderr, COLOR_RED "  - The input file must be specified with the '-a <input_file>' option.\n");
    fprintf(stderr, COLOR_RED "  - If '-o <output_file>' is used, the output file will be overwritten if it exists.\n\n");

    // Exit function
    exit(EXIT_FAILURE);
}

static void handle_file_error(const char *filename) {
    fprintf(stderr, COLOR_RED "Error: Could not open file '%s'.\n", filename);
    perror("\t> Reason");
    fprintf(stderr, COLOR_RESET);

    exit(EXIT_FAILURE);
}

// Struct for storing arguments and options
typedef struct {
    char *input_file;
    char *output_file;
    int verbose;
    int log;
} argonaut_options;

int main(int argc, char **argv) {
    int opt;
    argonaut_options options = {NULL, NULL, 0, 0};

    // Long options array (for --help, --verbose, etc.)
    static struct option long_options[] = {
        {"help",    no_argument,       NULL, 'h'},
        {"verbose", no_argument,       NULL, 'v'},
        {"log",     no_argument,       NULL, 'l'},
        {"output",  required_argument, NULL, 'o'},
        {0, 0, 0, 0}  // End of options
    };

    error.line = 1;
    error.column = 1;
    error.type = NO_ERROR;

    // Process command-line options
    while ((opt = getopt_long(argc, argv, "a:vlo:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 'a':
                options.input_file = optarg;
                break;
            case 'v':
                options.verbose = 1;
                break;
            case 'l':
                options.log = 1;
                break;
            case 'o':
                options.output_file = optarg;
                break;
            case 'h':
                print_usage(argv[0]);
                break;
            default:
                fprintf(stderr, COLOR_RED "Error: Unknown option '-%c'.\n" COLOR_RESET, optopt);
                print_usage(argv[0]);
                return 1;
        }
    }

    // Ensure the input file is provided
    if (options.input_file == NULL) {
        fprintf(stderr, COLOR_RED "Error: Missing required '-a <file>' argument.\n" COLOR_RESET);
        print_usage(argv[0]);
    }

    // Open the input file
    FILE *file = fopen(options.input_file, "r");
    if (!file) {
        handle_file_error(options.input_file);
    }

    // Set up the file for processing
    error.filename = options.input_file;
    yyin = file;

    // Run compilation
    yyrun(COMPILATION);
    fclose(file);

    ydebug(options.verbose);
    ylog(options.log);

    // Handle output file export if specified
    if (options.output_file != NULL) {
        remove(options.output_file);  // Remove existing file before creating

        // Export data to output file (implement your export functions)
        export_lexeme_table(options.output_file);
        export_declaration_table(options.output_file);
        export_representation_table(options.output_file);
        export_region_table(options.output_file);
    }

    return 0;
}
