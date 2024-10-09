%{
    #include <stdio.h>
    int yylex();

    extern int error_line;
    extern char *yytext;
    void yyerror(const char *s);
%}

