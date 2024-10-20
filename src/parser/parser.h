#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <string.h>

#include "../../lib/colors.h" 


extern int error_line;
extern int error_column;

extern char *yytext;
extern int yychar;

void yywarn(const char *s);

void yyerror(const char *s);

int run_parser();


#endif // __PARSER_H__