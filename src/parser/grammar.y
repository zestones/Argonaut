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

    #include "../syntax_analysis/loop_statement.h"

    #include "../ast/access_chain.h"

    #include "../utils/errors.h"
    #include "../utils/ir_version.h"

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>


    int yylex(void);
    void yyerror(const char *s);
    int is_in_loop = 0;

    /* a[i]++ / s.f-- : the target is the access chain, the value is a copy of it plus or minus 1. */
    static Node *build_access_inc_dec(Node *access, NodeType operation, int one_lexeme) {
        Node *assignment = construct_node_default(access->type == A_ARRAY_ACCESS ? A_ARRAY_ASSIGNMENT : A_STRUCT_ASSIGNMENT);
        Node *op_node = construct_node_default(operation);
        Node *copy = copy_ast(access);
        Node *one = construct_node(A_INTEGER_LITERAL, one_lexeme, NULL_VALUE);

        add_child(op_node, copy);
        add_sibling(copy, one);
        add_child(assignment, access);
        add_sibling(access, op_node);

        check_access_assignment(access, op_node);
        return assignment;
    }

    /* A binary node: the operands are the two children (left first). */
    static Node *build_binary(NodeType type, Node *left, Node *right) {
        Node *node = construct_node_default(type);
        add_child(node, left);
        add_sibling(left, right);
        return node;
    }

    extern FILE *yyin;
    extern FILE *yyout;
    extern char *yytext;

    void yyerror(const char *s) {
        fatal_syntax_error(yytext, strcmp(s, "syntax error") == 0 ? NULL : s);
    }
%}

%code requires {
    #include "../ast/ast.h"
}

%union {
    AST ast;
	int lexicographic_index;
}


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
%token BREAK CONTINUE

%token <lexicographic_index> BOOLEAN_VALUE STRING_VALUE INTEGER_VALUE CHARACTER_VALUE FLOAT_VALUE
%token <lexicographic_index> INCREMENT DECREMENT

%right RETURN_TYPE RETURN_VALUE
%left SEMICOLON COMMA

/* Lowest to highest: || , && , ! , comparisons, + - , * / % , unary minus. */
%left OR
%left AND
%right NOT
%nonassoc EQUAL NOT_EQUAL LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL
%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO
%right UMINUS

%type <lexicographic_index> type bound

%type <ast> assignment_statement if_head if_statement standalone_func_proc_call_statement func_proc_call_expression
%type <ast> access array_indices print_statement input_statement
%type <ast> loop_statement while_statement for_statement

%type <ast> variable_declaration type_declaration complex_type_fields type_field
%type <ast> function_declaration return_statement procedure_declaration parameter_list parameter argument_list

%type <ast> program declaration_list statement_list statement_block declaration statement inc_dec_statement
%type <ast> expression expression_atom

%type <ast> input_argument_list format_string 

%start program
%debug
%%

program: declaration_list statement_list { 
            $$ = construct_node_default(A_PROGRAM); 

            if (is_node_null($1)) {
                $1 = construct_node_default(A_DECLARATION_LIST);
            }
            
            add_child($$, $1);
            add_sibling($1, $2);

            update_region_ast(peek_region(), $$);
        }
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

                        check_function_returns($2, $12);
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

one_dimension: bound TWO_POINTS bound { array_add_dimension($1, $3); }
;

bound: INTEGER_VALUE { $$ = (int) strtol(get_lexeme($1), NULL, 10); }
     | MINUS INTEGER_VALUE { $$ = -(int) strtol(get_lexeme($2), NULL, 10); }
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
          | MINUS expression %prec UMINUS {
                $$ = construct_node_default(A_NEG_OP);
                add_child($$, $2);
          }
          | expression EQUAL expression         { $$ = build_binary(A_EQUAL_OP, $1, $3); }
          | expression NOT_EQUAL expression     { $$ = build_binary(A_NOT_EQUAL_OP, $1, $3); }
          | expression LESS_THAN expression     { $$ = build_binary(A_LESS_THAN_OP, $1, $3); }
          | expression GREATER_THAN expression  { $$ = build_binary(A_GREATER_THAN_OP, $1, $3); }
          | expression LESS_EQUAL expression    { $$ = build_binary(A_LESS_EQUAL_OP, $1, $3); }
          | expression GREATER_EQUAL expression { $$ = build_binary(A_GREATER_EQUAL_OP, $1, $3); }
          | expression AND expression           { $$ = build_binary(A_AND_CONDITION, $1, $3); }
          | expression OR expression            { $$ = build_binary(A_OR_CONDITION, $1, $3); }
          | NOT expression {
                $$ = construct_node_default(A_NOT_CONDITION);
                add_child($$, $2);
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
               | access { $$ = $1; }
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

// Statements
statement_block: START statement_list END {
                    // An empty block must still be a node: the if/else and loop rules chain
                    // their blocks with add_sibling, which ignores NULL (the else was lost).
                    $$ = is_node_null($2) ? construct_node_default(A_STATEMENT_LIST) : $2;
                }
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
        | loop_statement { $$ = $1; }
        | print_statement { $$ = $1; }
        | input_statement { $$ = $1; }
        | return_statement{ $$ = $1; }
;

assignment_statement: IDENTIFIER { check_variable_definition($1); } OPAFF expression {
                        $$ = construct_node(A_VARIABLE_ASSIGNMENT, $1, get_var_param_declaration_index($1));
                        add_child($$, $4);
                        check_variable_assignment($1, $4);
                    }
                    | access OPAFF expression {
                        $$ = construct_node_default($1->type == A_ARRAY_ACCESS ? A_ARRAY_ASSIGNMENT : A_STRUCT_ASSIGNMENT);
                        add_child($$, $1);  
                        add_sibling($1, $3);
                        check_access_assignment($1, $3);
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
                    | access INCREMENT { $$ = build_access_inc_dec($1, A_ADD_OP, $2); }
                    | access DECREMENT { $$ = build_access_inc_dec($1, A_SUB_OP, $2); }
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
                | RETURN_VALUE SEMICOLON {
                    // 'return;' leaves a procedure early; a function must return a value.
                    if (!peek_region()) {
                        set_error_type(&error, SYNTAX_ERROR);
                        yyerror("Return statement is not allowed outside of a function or procedure.");
                    }
                    if (find_function_index_by_region(peek_region()) != NULL_VALUE) {
                        set_error_type(&error, SYNTAX_ERROR);
                        yyerror("A function must return a value: write 'return expression;'.");
                    }

                    $$ = construct_node_default(A_RETURN_STATEMENT);
                }
;

// The condition is checked as soon as it is parsed (mid-rule action), so that the error
// location points at the condition rather than at the end of the block. The head is shared
// by the three forms so that the parser has a single action to reduce before ')'.
if_head: IF OPEN_PARENTHESIS expression { check_condition($3); } CLOSE_PARENTHESIS { $$ = $3; }
;

if_statement: if_head statement_block {
                $$ = construct_node_default(A_IF);
                add_child($$, $1);
                add_sibling($1, $2);
            }
            | if_head statement_block ELSE statement_block {
                $$ = construct_node_default(A_IF_ELSE); 
                add_child($$, $1); 
                add_sibling($1, $2);
                add_sibling($2, $4);
            }
            | if_head statement_block ELSE if_statement 
            {
                $$ = construct_node_default(A_IF_ELSE_IF);
                add_child($$, $1); 
                add_sibling($1, $2);
                add_sibling($2, $4);
            }
;

loop_statement: while_statement { $$ = $1; }
              | for_statement   { $$ = $1; }
              | BREAK SEMICOLON {
                if (!loop_context_active()) {
                    yyerror("Do not use 'break' outside of a loop !");
                }
                $$ = construct_node_default(A_BREAK);
              }
              | CONTINUE SEMICOLON {
                if (!loop_context_active()) {
                    yyerror("Do not use 'continue' outside of a loop !");
                }
                $$ = construct_node_default(A_CONTINUE);
              }
;

while_statement: WHILE OPEN_PARENTHESIS expression { check_condition($3); } CLOSE_PARENTHESIS { activate_loop_context(); } statement_block {
                $$ = construct_node_default(A_WHILE);
                add_child($$, $3);
                add_sibling($3, $7);

                deactivate_loop_context();
            }
;

for_statement: FOR OPEN_PARENTHESIS assignment_statement SEMICOLON expression { check_condition($5); } SEMICOLON assignment_statement CLOSE_PARENTHESIS { activate_loop_context(); } statement_block {
        $$ = construct_node_default(A_FOR_LOOP);
        Node *initialization = construct_node_default(A_VARIABLE_ASSIGNMENT);
        add_child($$, initialization);
        add_child(initialization, $3);

        add_sibling(initialization, $5);

        Node *update = construct_node_default(A_VARIABLE_ASSIGNMENT);
        add_sibling($5, update);
        add_child(update, $8);

        add_sibling(update, $11);
}
;

standalone_func_proc_call_statement: func_proc_call_expression SEMICOLON { $$ = $1; }
;

// Access chains: t[i], s.f, t[i].f, s.t[i], g[i][j], t[i].f[j]... (see ast/access_chain.h)
access: IDENTIFIER OPEN_BRACKET array_indices CLOSE_BRACKET {
            $$ = construct_node(A_ARRAY_ACCESS, $1, find_declaration_index($1));
            add_child($$, $3);
      }
      | IDENTIFIER DOT IDENTIFIER {
            $$ = construct_node(A_STRUCT_FIELD_ACCESS, $1, find_declaration_index($1));
            add_child($$, construct_node(A_STRUCT_FIELD_ACCESS, $3, NULL_VALUE));
      }
      | access OPEN_BRACKET array_indices CLOSE_BRACKET { access_chain_index($1, $3); $$ = $1; }
      | access DOT IDENTIFIER { access_chain_field($1, $3); $$ = $1; }
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

print_statement: PRINT OPEN_PARENTHESIS format_string COMMA argument_list CLOSE_PARENTHESIS SEMICOLON {
                    $$ = construct_node_default(A_PRINT_STATEMENT);
                    add_child($$, $3);
                    add_child($$, $5);
                    check_print_proc_argument_list($3, $5);
               }
                | PRINT OPEN_PARENTHESIS format_string CLOSE_PARENTHESIS SEMICOLON {
                      $$ = construct_node_default(A_PRINT_STATEMENT);
                      add_child($$, $3);
                      Node *no_arguments = construct_node_default(A_ARGUMENT_LIST);
                      check_print_proc_argument_list($3, no_arguments);
                      free_ast(no_arguments);
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
                    Node *no_arguments = construct_node_default(A_ARGUMENT_LIST);
                    check_print_proc_argument_list($3, no_arguments);
                    free_ast(no_arguments);
               }
;

input_argument_list: IDENTIFIER {
                        $$ = construct_node_default(A_ARGUMENT_LIST);
                        Node *single_argument = construct_node_default(A_ARGUMENT);

                        add_child(single_argument, construct_node(A_IDENTIFIER, $1, find_declaration_index($1)));
                        add_child($$, single_argument);
                    }
                   | access {
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
                   | input_argument_list COMMA access {
                        Node *single_argument = construct_node_default(A_ARGUMENT);
                        add_child(single_argument, $3);
                        add_sibling($1->child, single_argument);
                        $$ = $1;
                   }
;


%%
