%{
    #include <stdio.h>
    int yylex();

    extern int error_line;
    extern char *yytext;
    void yyerror(const char *s);
%}


%token PROG
%token SEMICOLON TWO_POINTS COMMA DOT_DOT OPEN_PARENTHESIS CLOSE_PARENTHESIS
%token START END
%token ARRAY OF OPEN_BRACKET CLOSE_BRACKET
%token IDENTIFIER VARIABLE OPAFF
%token STRUCT FSTRUCT
%token TYPE INTEGER_CONST INTEGER FLOAT BOOLEAN CHARACTER STRING 
%token PROCEDURE FUNCTION RETURN_TYPE RETURN_VALUE
%token IF ELSE
%token WHILE DO
%token EQUAL NOT_EQUAL LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL

%left AND OR 
%right NOT

%left PLUS MINUS
%left MULTIPLY DIVIDE
%%

program: PROG declaration_list statement_list
    | error { yyerror("Unexpected input in program structure."); } 
    | ;
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
           | procedure_declaration ;

variable_declaration: VARIABLE IDENTIFIER TWO_POINTS type SEMICOLON ;

function_declaration: FUNCTION IDENTIFIER OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS RETURN_TYPE type START declaration_list statement_list return_statement END ;

procedure_declaration: PROCEDURE IDENTIFIER OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS START declaration_list statement_list END ;

type_declaration: TYPE IDENTIFIER TWO_POINTS STRUCT START complex_type_fields END FSTRUCT SEMICOLON
                | TYPE IDENTIFIER TWO_POINTS ARRAY dimension OF type_name SEMICOLON ;



argument_list: argument_list COMMA expression
    | expression
    | ;

parameter_list: parameter_list COMMA parameter
    | parameter
    | ;

parameter: IDENTIFIER TWO_POINTS type ;



dimension: OPEN_BRACKET list_dimensions CLOSE_BRACKET ;

list_dimensions: one_dimension
               | list_dimensions COMMA one_dimension ;

one_dimension: INTEGER DOT_DOT INTEGER ;// Arithmetic expressions
expression:
    expression PLUS expression
    | expression MINUS expression
    | expression MULTIPLY expression
    | expression DIVIDE expression
    | function_call_expression  
    | IDENTIFIER
    | INTEGER ;

type: INTEGER
    | FLOAT
    | BOOLEAN
    | CHARACTER
    | STRING OPEN_BRACKET INTEGER CLOSE_BRACKET ;

complex_type_fields: type_field
                  | complex_type_fields type_field ;

type_field: IDENTIFIER TWO_POINTS type_name SEMICOLON ;

type_name: type
         | IDENTIFIER ;

function_call_expression: IDENTIFIER OPEN_PARENTHESIS argument_list CLOSE_PARENTHESIS ;

// Statements
statement_block: START statement_list END ;

statement_list: statement_list statement
              | ;

statement: assignment_statement
    | if_statement
    | standalone_function_call_statement
    | loop_statement ;

assignment_statement: IDENTIFIER OPAFF expression SEMICOLON ;

return_statement: RETURN_VALUE IDENTIFIER SEMICOLON;

if_statement: IF condition statement_block
            | IF condition statement_block ELSE statement_block;

loop_statement: loop;

standalone_function_call_statement: function_call_expression SEMICOLON ;



%%

void yyerror(const char *s) {
    if (yychar == 0) {
        fprintf(stderr, "\033[1;31m<error> Syntax error at line %d: %s\033[0m\n", error_line, s);
    } 
    else {
        fprintf(stderr, "\033[1;31m<error> Syntax error at line %d: %s near token '%s'\033[0m\n", error_line, s, yytext);
    }
}

int main() {
    return yyparse();
}