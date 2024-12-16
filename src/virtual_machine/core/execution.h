#ifndef __VIRTUAL_MACHINE_H__
#define __VIRTUAL_MACHINE_H__

#include <stdio.h>
#include <stdlib.h>
#include "../../ast/ast.h"

void declare_variable(int type, int execution_size);

void declare_array_variable(int array_type, int execution_size);

void handle_declaration(int type, int index_lexicographic, int index_declaration);

#endif // __VIRTUAL_MACHINE_H__