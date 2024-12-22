#ifndef __VIRTUAL_MACHINE_H__
#define __VIRTUAL_MACHINE_H__

#include <stdio.h>
#include <stdlib.h>

#include "../../ast/ast.h"
#include "vm_cell.h"

vm_cell get_variable_cell(int index_declaration);

int get_index_variable(int index_declaration);

void handle_variable_affectation(int index_declaration, vm_cell cell);

void handle_variable_declaration(int type, int index_lexicographic, int index_declaration);

#endif // __VIRTUAL_MACHINE_H__