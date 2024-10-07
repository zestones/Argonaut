%{
    #include <stdio.h>
    int yylex();
    int yyerror();
    extern int numligne;
 
%}


%token PROG
%token POINT_VIRGULE DEUX_POINTS VIRGULE POINT_POINT PARENTHESE_OUVRANTE PARENTHESE_FERMANTE
%token DEBUT FIN
%token TABLEAU DE CROCHET_OUVRANT CROCHET_FERMANT
%token IDF VARIABLE OPAFF
%token STRUCT FSTRUCT
%token TYPE CSTE_ENTIERE ENTIER REEL BOOLEEN CARACTERE CHAINE VIDE
%token PROCEDURE FONCTION RETOURNE
%token SI ALORS SINON
%token TANT_QUE FAIRE
%token EGAL DIFFERENT INF SUP INFEGAL SUPEGAL
%token ET OU NON
%token PLUS DIV MOINS MUL



%%
programme: PROG corps ;

corps: liste_declarations liste_instructions
     | liste_instructions ;

liste_declarations: declaration POINT_VIRGULE
                 | liste_declarations declaration POINT_VIRGULE ;

liste_instructions: DEBUT suite_liste_inst FIN ;

suite_liste_inst: instruction POINT_VIRGULE
               | suite_liste_inst instruction POINT_VIRGULE ;

declaration: declaration_type
           | declaration_variable
           | declaration_procedure
           | declaration_fonction ;

declaration_type: TYPE IDF DEUX_POINTS suite_declaration_type ;

suite_declaration_type: STRUCT liste_champs FSTRUCT
                     | TABLEAU dimension DE nom_type ;

dimension: CROCHET_OUVRANT liste_dimensions CROCHET_FERMANT ;

liste_dimensions: une_dimension
                | liste_dimensions VIRGULE une_dimension ;

une_dimension: CSTE_ENTIERE POINT_POINT CSTE_ENTIERE ;

liste_champs: un_champ
            | liste_champs POINT_VIRGULE un_champ ;

un_champ: IDF DEUX_POINTS nom_type ;

nom_type: type_simple
        | IDF ;

type_simple: ENTIER
           | REEL
           | BOOLEEN
           | CARACTERE
           | CHAINE CROCHET_OUVRANT CSTE_ENTIERE CROCHET_FERMANT ;

declaration_variable: VARIABLE IDF DEUX_POINTS nom_type ;

declaration_procedure: PROCEDURE IDF liste_parametres corps ;

declaration_fonction: FONCTION IDF liste_parametres RETOURNE type_simple corps ;

liste_parametres:
                | PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE ;

liste_param: un_param
           | liste_param POINT_VIRGULE un_param ;

un_param: IDF DEUX_POINTS type_simple ;

instruction: affectation
           | condition
           | tant_que
           | appel
           | VIDE
           | RETOURNE resultat_retourne ;

resultat_retourne:
                | expression ;

appel: IDF liste_arguments ;

liste_arguments:
               | PARENTHESE_OUVRANTE liste_args PARENTHESE_FERMANTE ;

liste_args: un_arg
          | liste_args VIRGULE un_arg ;

un_arg: expression ;

condition: SI expression_booleenne ALORS liste_instructions SINON liste_instructions ;

tant_que: TANT_QUE expression_booleenne FAIRE liste_instructions ;

affectation: variable OPAFF expression ;

variable: IDF 
        ;

expression: exp_add POINT_VIRGULE
          ;

exp_add: exp_add PLUS exp_mul
       | exp_add MOINS exp_mul
       | exp_mul
       ;

exp_mul : exp_mul MUL exp_atom
        | exp_mul DIV exp_atom
        | exp_atom
        ;

exp_atom : CSTE_ENTIERE
         | PARENTHESE_OUVRANTE exp_add PARENTHESE_FERMANTE
         ;

expression_booleenne: expression comparaison expression
                   | NON expression
                   | expression logique expression ;

comparaison: EGAL
           | DIFFERENT
           | INF
           | SUP
           | INFEGAL
           | SUPEGAL ;

logique: ET
       | OU ;

%%




int yyerror() {
    printf("Erreur de syntaxe en ligne %d\n", numligne);
    return 1;
}

int main() {
    return yyparse();
}
