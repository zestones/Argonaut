%{
    #include "../lexer/lexeme_table.h"
    #include "../parser/parser.h"
    #include "y.tab.h"

    int error_line = 1; 
    int error_column = 1; 

    extern char *yytext;
%}

number [0-9]
letter [a-zA-Z]

%%

[ \t]+                          { error_column += yyleng; }  

"//".*						    { error_column += yyleng; }  

\n                              { error_line++; error_column = 1; } // Reset column number on new line

"PROG"                          { error_column += yyleng; return PROG; }
"{"                             { error_column += yyleng; return START; }
"}"                             { error_column += yyleng; return END; }

"var"                           { error_column += yyleng; return VARIABLE; }
"proc"                          { error_column += yyleng; return PROCEDURE; }
"func"                          { error_column += yyleng; return FUNCTION; }

"return"                        { error_column += yyleng; return RETURN_VALUE; }
"->"                            { error_column += yyleng; return RETURN_TYPE; }

"type"                          { error_column += yyleng; return TYPE; }
"struct"                        { error_column += yyleng; return STRUCT; }
"fstruct"                       { error_column += yyleng; return FSTRUCT; }
"array"                         { error_column += yyleng; return ARRAY; }
"of"                            { error_column += yyleng; return OF; }

"if"                            { error_column += yyleng; return IF; }
"else"                          { error_column += yyleng; return ELSE; }
"while"                         { error_column += yyleng; return WHILE; }

"int"                           { error_column += yyleng; return INTEGER; }
"float"                         { error_column += yyleng; return FLOAT; }
"bool"                          { error_column += yyleng; return BOOLEAN; }
"char"                          { error_column += yyleng; return CHARACTER; }
"string"                        { error_column += yyleng; return STRING; }

"=="                            { error_column += yyleng; return EQUAL; }
"!="                            { error_column += yyleng; return NOT_EQUAL; }
"<"                             { error_column += yyleng; return LESS_THAN; }
">"                             { error_column += yyleng; return GREATER_THAN; }
"<="                            { error_column += yyleng; return LESS_EQUAL; }
">="                            { error_column += yyleng; return GREATER_EQUAL; }

"&&"                            { error_column += yyleng; return AND; }
"||"                            { error_column += yyleng; return OR; }
"!"                             { error_column += yyleng; return NOT; }

"+"                             { error_column += yyleng; return PLUS; }
"-"                             { error_column += yyleng; return MINUS; }
"*"                             { error_column += yyleng; return MULTIPLY; }
"/"                             { error_column += yyleng; return DIVIDE; }

":="                            { error_column += yyleng; return OPAFF; }

";"                             { error_column += yyleng; return SEMICOLON; }
":"                             { error_column += yyleng; return TWO_POINTS; }
","                             { error_column += yyleng; return COMMA; }
".."                            { error_column += yyleng; return DOT_DOT; }
"["                             { error_column += yyleng; return OPEN_BRACKET; }
"]"                             { error_column += yyleng; return CLOSE_BRACKET; }
"("                             { error_column += yyleng; return OPEN_PARENTHESIS; }
")"                             { error_column += yyleng; return CLOSE_PARENTHESIS; }

0|[1-9][0-9]* { 
    yylval.ival = atoi(yytext);
    error_column += yyleng;
    return INTEGER; 
}

{number}+","{number}+ { 
    yylval.ival = atoi(yytext);
    error_column += yyleng;
    return FLOAT; 
}

{letter}+({letter}|{number}|"_"|"-")* { 
    yylval.ival = yytext[0] - 'a';
    error_column += yyleng;
    
    yylval.lexicographic_index = insert_lexeme(yytext);
    return IDENTIFIER;
}

.                               { yywarn(yytext); error_column += yyleng; } 

%%

int yywrap() {
    return 1;
}