#ifndef __VIRTUAL_MACHINE_H__
#define __VIRTUAL_MACHINE_H__

#include <stdio.h>
#include <stdlib.h>
#include "../../ast/ast.h"

void handle_variable_affectation(int type, int index_lexicographic, int index_declaration);

void handle_variable_declaration(int type, int index_lexicographic, int index_declaration);

#endif // __VIRTUAL_MACHINE_H__