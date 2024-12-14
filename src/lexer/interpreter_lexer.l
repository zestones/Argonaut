%{
    #include "../utils/errors.h"
    #include "y.tab.h"

    #include <string.h>
    #include <stdlib.h>

    extern Error error;
    extern char *yytext;
    extern int yyerror(const char *s);
%}

number [0-9]
letter [a-zA-Z]

%%

\n                              { error.line++; error.column = 1; }
[ \t]+                          { error.column += yyleng; }

"!CPYRR"                        { return CPYRR; }

"BEGIN_LEXEME_TABLE"          { error.column += yyleng; return BEGIN_LEXEME_TABLE; }
"END_LEXEME_TABLE"            { error.column += yyleng; return END_LEXEME_TABLE; }
"BEGIN_DECLARATION_TABLE"     { error.column += yyleng; return BEGIN_DECLARATION_TABLE; }
"END_DECLARATION_TABLE"       { error.column += yyleng; return END_DECLARATION_TABLE; }
"BEGIN_REPRESENTATION_TABLE"  { error.column += yyleng; return BEGIN_REPRESENTATION_TABLE; }
"END_REPRESENTATION_TABLE"    { error.column += yyleng; return END_REPRESENTATION_TABLE; }
"BEGIN_REGION_TABLE"          { error.column += yyleng; return BEGIN_REGION_TABLE; }
"END_REGION_TABLE"            { error.column += yyleng; return END_REGION_TABLE; }

"|"                           { error.column += yyleng; return PIPE; }


"Node("                  { error.column += yyleng; return NODE_LPAREN; }
")"                      { error.column += yyleng; return NODE_RPAREN; }
","                      { error.column += yyleng; return COMMA; }
"["                      { error.column += yyleng; return LBRACKET; }
"]"                      { error.column += yyleng; return RBRACKET; }
"Child:"                 { error.column += yyleng; return CHILD; }
"Sibling:"               { error.column += yyleng; return SIBLING; }


[-]?{number}+ {
    yylval.integer = atoi(yytext);
    error.column += yyleng;
    return INTEGER;
}

[-]?{number}+"."{number}+ {
    yylval.real = atof(yytext);
    error.column += yyleng;
    return FLOAT;
}

{letter}+({letter}|{number}|"_"|"-")* {
    yylval.string = strdup(yytext);
    error.column += yyleng;
    return LEXEME;
}

\"[^\"]*\" { 
    yylval.string = strdup(yytext);
    error.column += yyleng;
    return LEXEME;
}

"'"[^\"]"'" { 
    yylval.string = strdup(yytext);
    error.column += yyleng;
    return LEXEME; 
}

.                    { error.column += yyleng; yyerror("Unrecognized character"); }

%%

int yywrap() { return 1; }
