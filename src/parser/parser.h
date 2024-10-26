#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <string.h>

#include "../../lib/colors.h"

#include "../utils/hash.h"
#include "../utils/utils.h"
#include "../lexer/lexeme_table.h"
#include "../symbol_table/declaration_table.h"



extern int error_line;
extern int error_column;

extern char *yytext;
extern int yychar;

void yydebug(int debug);

void yywarn(const char *s);

void yyerror(const char *s);

int yyrun();


#endif // __PARSER_H__