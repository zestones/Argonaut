%{
#include "y.tab.h"
int numligne = 1; 
%}


%%

0|[1-9][0-9]* {yylval=atoi(yytext); return CSTE_ENTIERE;}
[a-z][0-9]* {yylval = yytext[0] - 'a' ; return IDF;}

"prog"                          { return PROG; }
"{"                             { return DEBUT; }
"}"                             { return FIN; }
"var"                           { printf("var"); return VARIABLE; }
"procedure"                     { return PROCEDURE; }
"fonction"                      { return FONCTION; }
"return"                        { return RETOURNE; }
"type"                          { return TYPE; }
"struct"                        { return STRUCT; }
"fstruct"                       { return FSTRUCT; }
"tableau"                       { return TABLEAU; }
"of"                            { return DE; }
"if"                            { return SI; }
"then"                          { return ALORS; }
"else"                          { return SINON; }
"while"                         { return TANT_QUE; }
"do"                            { return FAIRE; }
"void"                          { return VIDE; }
"int"                           { return ENTIER; }
"float"                         { return REEL; }
"boolean"                       { return BOOLEEN; }
"char"                          { return CARACTERE; }
"string"                        { return CHAINE; }

"=="                            { return EGAL; }
"!="                            { return DIFFERENT; }
"<"                             { return INF; }
">"                             { return SUP; }
"<="                            { return INFEGAL; }
">="                            { return SUPEGAL; }

"&&"                            { return ET; }
"||"                            { return OU; }
"!"                             { return NON; }

"+"                             { return PLUS; }
"-"                             { return MOINS; }
"*"                             { return MUL; }
"/"                             { return DIV; }

":="                             { return OPAFF; }

";"                             { return POINT_VIRGULE; }
":"                             { return DEUX_POINTS; }
","                             { return VIRGULE; }
".."                            { return POINT_POINT; }
"["                             { return CROCHET_OUVRANT; }
"]"                             { return CROCHET_FERMANT; }
"("                             { return PARENTHESE_OUVRANTE; }
")"                             { return PARENTHESE_FERMANTE; }

[ \t]+                          ;  // Ignorer les espaces et tabulations

\n                              { numligne++; } 

.                               { printf("Caractère inconnu : %s en ligne %d\n", yytext, numligne); }

%%

int yywrap() {
    return 1;
}
