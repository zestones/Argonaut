%{
    #include "../parser/parser.h"
    #include <string.h>

    #include "../utils/errors.h"
    #include "y.tab.h"

    extern Error error;
    extern char *yytext;
    extern int yyerror(const char *s);
%}

%option noinput nounput

/* Inside the lexeme table every row is read as a single token: its text is the lexeme
 * itself (an identifier, a literal...) and must not be tokenised as IR syntax. */
%x LEXTAB

number [0-9]

%%

\n                              { error.line++; error.column = 1; }
[ \t]+                          { error.column += yyleng; }

"BEGIN_LEXEME_TABLE"          { error.column += yyleng; BEGIN(LEXTAB); return BEGIN_LEXEME_TABLE; }
<LEXTAB>^"END_LEXEME_TABLE"   { error.column += yyleng; BEGIN(INITIAL); return END_LEXEME_TABLE; }
<LEXTAB>^"|"{number}+"|".*"|"-?{number}+"|"-?{number}+"|"$ {
    yylval.string = strdup(yytext);
    error.column += yyleng;
    return TOKEN_LEXEME_ROW;
}
<LEXTAB>\n                    { error.line++; error.column = 1; }
<LEXTAB>[ \t]+                { error.column += yyleng; }
<LEXTAB>.                     { error.column += yyleng; yyerror("Malformed lexeme table row"); }

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
    yylval.integer = (int) strtol(yytext, NULL, 10);
    error.column += yyleng;
    return TOKEN_INTEGER;
}

.                    { error.column += yyleng; yyerror("Unrecognized character"); }

%%

int yywrap(void) { return 1; }
