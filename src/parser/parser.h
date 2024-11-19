#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <string.h>

#include "../ast/ast.h"

#include "../../lib/colors.h"
#include "../utils/utils.h"

#include "../lexer/lexeme_table.h"

#include "../symbol_table/declaration_table.h"
#include "../symbol_table/representation_table.h"
#include "../symbol_table/hash_table.h"

#include "../data/region_table.h"

void ydebug(int debug);

int yyrun();


#endif // __PARSER_H__