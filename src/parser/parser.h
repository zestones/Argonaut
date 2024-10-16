#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>

extern int error_line;
extern char *yytext;
extern int yychar;


void yyerror(const char *s);

int run_parser();


#endif // __PARSER_H__