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
%token TYPE INTEGER_CONST INTEGER FLOAT BOOLEAN CHARACTER STRING VOID
%token PROCEDURE FUNCTION RETURN
%token IF THEN ELSE
%token WHILE DO
%token EQUAL NOT_EQUAL LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL
%token AND OR NOT

%left PLUS MINUS
%left MULTIPLY DIVIDE
%%

program: PROG declaration_list statement_list
    | error { yyerror("Unexpected input in program structure."); } 
    | ;
// Conditions and boolean expressions

condition: expression comparison_operator expression ;

comparison_operator:
    EQUAL
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
           | type_declaration;

variable_declaration: VARIABLE IDENTIFIER TWO_POINTS type SEMICOLON ;

function_declaration: FUNCTION IDENTIFIER OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS RETURN type START declaration_list statement_list END ;

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
statement:
    assignment_statement
    | if_statement
    | standalone_function_call_statement ;

assignment_statement: IDENTIFIER OPAFF expression SEMICOLON ;

if_statement: IF condition THEN statement_block
            | IF condition THEN statement_block ELSE statement_block;

standalone_function_call_statement: function_call_expression SEMICOLON
    {
        printf("> Standalone function call\n");
    };



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