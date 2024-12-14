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

"BEGIN_LEXEME_TABLE"          { return BEGIN_LEXEME_TABLE; }
"END_LEXEME_TABLE"            { return END_LEXEME_TABLE; }

"|"                  { error.column += yyleng; return PIPE; }


[-]?{number}+ {
    yylval.integer = atoi(yytext);
    return INTEGER;
}

[-]?{number}+"."{number}+ {
    yylval.real = atof(yytext);
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

.                    { yyerror("Unrecognized character"); }

%%

int yywrap() { return 1; }
