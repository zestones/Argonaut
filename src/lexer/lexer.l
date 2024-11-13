%{
    #include "../lexer/lexeme_table.h"
    #include "../parser/parser.h"

    #include "../utils/errors.h"
    #include "y.tab.h"

    extern Error error;
    extern char *yytext;
%}

number [0-9]
letter [a-zA-Z]

%%

[ \t]+                          { error.column += yyleng; }  

"//".*						    { error.column += yyleng; }  

\n                              { error.line++; error.column = 1; } // Reset column number on new line

"PROG"                          { error.column += yyleng; return PROG; }
"{"                             { error.column += yyleng; return START; }
"}"                             { error.column += yyleng; return END; }

"var"                           { error.column += yyleng; return VARIABLE; }
"proc"                          { error.column += yyleng; return PROCEDURE; }
"func"                          { error.column += yyleng; return FUNCTION; }

"return"                        { error.column += yyleng; return RETURN_VALUE; }
"->"                            { error.column += yyleng; return RETURN_TYPE; }

"type"                          { error.column += yyleng; return TYPE; }
"struct"                        { error.column += yyleng; return STRUCT; }
"fstruct"                       { error.column += yyleng; return FSTRUCT; }
"array"                         { error.column += yyleng; return ARRAY; }
"of"                            { error.column += yyleng; return OF; }

"if"                            { error.column += yyleng; return IF; }
"else"                          { error.column += yyleng; return ELSE; }
"while"                         { error.column += yyleng; return WHILE; }

"int"                           { error.column += yyleng; return INTEGER; }
"float"                         { error.column += yyleng; return FLOAT; }
"bool"                          { error.column += yyleng; return BOOLEAN; }
"char"                          { error.column += yyleng; return CHARACTER; }
"string"                        { error.column += yyleng; return STRING; }

"=="                            { error.column += yyleng; return EQUAL; }
"!="                            { error.column += yyleng; return NOT_EQUAL; }
"<"                             { error.column += yyleng; return LESS_THAN; }
">"                             { error.column += yyleng; return GREATER_THAN; }
"<="                            { error.column += yyleng; return LESS_EQUAL; }
">="                            { error.column += yyleng; return GREATER_EQUAL; }

"&&"                            { error.column += yyleng; return AND; }
"||"                            { error.column += yyleng; return OR; }
"!"                             { error.column += yyleng; return NOT; }

"+"                             { error.column += yyleng; return PLUS; }
"-"                             { error.column += yyleng; return MINUS; }
"*"                             { error.column += yyleng; return MULTIPLY; }
"/"                             { error.column += yyleng; return DIVIDE; }

":="                            { error.column += yyleng; return OPAFF; }

";"                             { error.column += yyleng; return SEMICOLON; }
":"                             { error.column += yyleng; return TWO_POINTS; }
","                             { error.column += yyleng; return COMMA; }
".."                            { error.column += yyleng; return DOT_DOT; }
"["                             { error.column += yyleng; return OPEN_BRACKET; }
"]"                             { error.column += yyleng; return CLOSE_BRACKET; }
"("                             { error.column += yyleng; return OPEN_PARENTHESIS; }
")"                             { error.column += yyleng; return CLOSE_PARENTHESIS; }

0|[1-9][0-9]* { 
    yylval.ival = atoi(yytext);
    error.column += yyleng;
    return INTEGER; 
}

{number}+","{number}+ { 
    yylval.ival = atoi(yytext);
    error.column += yyleng;
    return FLOAT; 
}

{letter}+({letter}|{number}|"_"|"-")* { 
    yylval.ival = yytext[0] - 'a';
    error.column += yyleng;
    
    yylval.lexicographic_index = insert_lexeme(yytext);
    return IDENTIFIER;
}

. { 
    set_error_message(&error, "Unknown character '%s'", yytext);
    yerror(error);  
    error.column += yyleng;
} 

%%

int yywrap() {
    return 1;
}