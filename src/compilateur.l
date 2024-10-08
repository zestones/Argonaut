%{
    #include "y.tab.h"
    void yyerror(const char *s);
    int error_line = 1; 
%}

number [0-9]
letter [a-zA-Z]

%%

[ \t]+                          ;  // Ignorer les espaces et tabulations

"//".*						    ;  // Ignorer les commentaires

\n                              { error_line++; } 


"PROG"                          { return PROG; }
"{"                             { return START; }
"}"                             { return END; }
"var"                           { return VARIABLE; }
"proc"                          { return PROCEDURE; }
"func"                          { return FUNCTION; }
"return"                        { return RETURN; }
"type"                          { return TYPE; }
"struct"                        { return STRUCT; }
"fstruct"                       { return FSTRUCT; }
"array"                         { return ARRAY; }
"of"                            { return OF; }
"if"                            { return IF; }
"then"                          { return THEN; }
"else"                          { return ELSE; }
"while"                         { return WHILE; }
"do"                            { return DO; }
"void"                          { return VOID; }
"int"                           { return INTEGER; }
"float"                         { return FLOAT; }
"boolean"                       { return BOOLEAN; }
"char"                          { return CHARACTER; }
"string"                        { return STRING; }

"=="                            { return EQUAL; }
"!="                            { return NOT_EQUAL; }
"<"                             { return LESS_THAN; }
">"                             { return GREATER_THAN; }
"<="                            { return LESS_EQUAL; }
">="                            { return GREATER_EQUAL; }

"&&"                            { return AND; }
"||"                            { return OR; }
"!"                             { return NOT; }

"+"                             { return PLUS; }
"-"                             { return MINUS; }
"*"                             { return MULTIPLY; }
"/"                             { return DIVIDE; }

":="                             { return OPAFF; }

";"                             { return SEMICOLON; }
":"                             { return TWO_POINTS; }
","                             { return COMMA; }
".."                            { return DOT_DOT; }
"["                             { return OPEN_BRACKET; }
"]"                             { return CLOSE_BRACKET; }
"("                             { return OPEN_PARENTHEIFS; }
")"                             { return CLOSE_PARENTHEIFS; }


0|[1-9][0-9]* { 
    yylval = atoi(yytext);
    return INTEGER; 
}

{number}+","{number}+ { 
    yylval = atof(yytext);
    return FLOAT; 
}

{letter}+({letter}|{number}|"_"|"-")* { 
    yylval = yytext[0] - 'a';
    return IDENTIFIER;
}

.                               { yyerror(yytext); }


%%

int yywrap() {
    return 1;
}
