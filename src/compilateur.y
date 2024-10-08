%{
    #include <stdio.h>
    
    int yylex();
    void yyerror(const char *s);

    extern int error_line;
    extern char *yytext;
 
%}


%token PROG
%token SEMICOLON TWO_POINTS COMMA DOT_DOT OPEN_PARENTHEIFS CLOSE_PARENTHEIFS
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
%token PLUS DIVIDE MINUS MULTIPLY



%%
programme: 
         | PROG corps 
         | error { yyerror("Unexpected input in program structure."); } ;

corps: liste_declarations liste_instructions
     | liste_instructions ;

liste_declarations: declaration SEMICOLON
                 | liste_declarations declaration SEMICOLON ;

liste_instructions: START {printf("ici\n"); } suite_liste_inst END ;

suite_liste_inst:
               | instruction_list ; 

instruction_list: instruction SEMICOLON
               | instruction_list instruction SEMICOLON ;

declaration: declaration_type
           | declaration_function
           | declaration_procedure
           | declaration_variable ;

declaration_type: TYPE IDENTIFIER TWO_POINTS suite_declaration_type ;

suite_declaration_type: STRUCT liste_champs FSTRUCT
                     | ARRAY dimension OF nom_type ;

dimension: OPEN_BRACKET liste_dimensions CLOSE_BRACKET ;

liste_dimensions: une_dimension
                | liste_dimensions COMMA une_dimension ;

une_dimension: INTEGER_CONST DOT_DOT INTEGER_CONST ;

liste_champs: un_champ
            | liste_champs SEMICOLON un_champ ;

un_champ: IDENTIFIER TWO_POINTS nom_type ;

nom_type: type_simple
        | IDENTIFIER ;

type_simple: INTEGER
           | FLOAT
           | BOOLEAN
           | CHARACTER
           | STRING OPEN_BRACKET INTEGER_CONST CLOSE_BRACKET ;

declaration_variable: VARIABLE IDENTIFIER {printf("laaaa\n");} TWO_POINTS nom_type ;

declaration_procedure: PROCEDURE IDENTIFIER liste_parametres corps ;

declaration_function: FUNCTION IDENTIFIER liste_parametres RETURN type_simple corps ;

liste_parametres:
                | OPEN_PARENTHEIFS liste_param CLOSE_PARENTHEIFS ;

liste_param: un_param
           | liste_param SEMICOLON un_param ;

un_param: IDENTIFIER TWO_POINTS type_simple ;

instruction: affectation {printf("instruction\n");} 
           | condition
           | tant_que
           | appel
           | VOID
           | RETURN resultat_retourne ;

resultat_retourne:
                | expression ;

appel: IDENTIFIER liste_arguments ;

liste_arguments:
               | OPEN_PARENTHEIFS liste_args CLOSE_PARENTHEIFS ;

liste_args: un_arg
          | liste_args COMMA un_arg ;

un_arg: expression ;

condition: IF expression_bool THEN liste_instructions ELSE liste_instructions ;

tant_que: WHILE expression_bool DO liste_instructions ;

// TODO: change exp_add to expression
affectation: variable OPAFF exp_add ;

variable: IDENTIFIER ;

// TODO: change to expression_arithmetique and expression_bool
expression: exp_add SEMICOLON ;

exp_add: exp_add PLUS exp_mul
       | exp_add MINUS exp_mul
       | exp_mul
       ;

exp_mul : exp_mul MULTIPLY exp_atom 
        | exp_mul DIVIDE exp_atom
        | exp_atom
        ;

exp_atom : INTEGER
         | FLOAT
         | OPEN_PARENTHEIFS exp_add CLOSE_PARENTHEIFS
         ;

expression_bool: expression comparaison expression
                   | NOT expression
                   | expression logique expression ;

comparaison: EQUAL
           | NOT_EQUAL
           | LESS_THAN
           | GREATER_THAN
           | LESS_EQUAL
           | GREATER_EQUAL ;

logique: AND
       | OR ;

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
